#include"ExForwardRenderPipeline.h"

namespace ExRenderer
{
    struct ClearScreenJob:public JobData
    {
        static ForwardPipelineRenderer *renderer;
        static Color color;
        int rowIndex;
        ClearScreenJob(int rowIndex):rowIndex(rowIndex){}

        void Run() override
        {
            renderer->ClearRow(color,rowIndex);
        }
    };
    ForwardPipelineRenderer *ClearScreenJob::renderer=nullptr;
    Color ClearScreenJob::color(0,0,0,0);

    ForwardPipelineRenderer::ForwardPipelineRenderer(const char *title,uint32_t w, uint32_t h):m_width(w),m_height(h)
    {
        m_frame=FrameBuffer(w,h);
        m_depth=DepthBuffer(w,h);
        enableRenderBoost=true;

        SDL_Init(SDL_INIT_EVERYTHING);

        sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, 0);
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
    }

    ForwardPipelineRenderer::~ForwardPipelineRenderer()
    {
        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyWindow(sdlWindow);
    }

    bool ForwardPipelineRenderer::UpdateEnv()
    {
        SDL_UpdateTexture(sdlTexture, nullptr, this->m_frame.GetData(), m_width * 4);
        SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);

        SDL_Event event;
        SDL_PollEvent(&event);
        return event.type==SDL_QUIT;
    }

    void ForwardPipelineRenderer::SetCameraParams(float fov, float nearPlane, float farPlane)
    {
        float aspect=(float)m_width/m_height;
        float cotHalfFov=cos(fov/2)/sin(fov/2);
        projectionMatrix=Matrix4x4(Vector4(-cotHalfFov/aspect,0,0,0),
                                    Vector4(0,-cotHalfFov,0,0),
                                    Vector4(0,0,-(farPlane+nearPlane)/(farPlane-nearPlane),-1),
                                    Vector4(0,0,2*nearPlane*farPlane/(farPlane-nearPlane),0));
        updateMvpMatrix();
        
    }

    void ForwardPipelineRenderer::SetCameraTransform(const Vector3& position, const Vector3 &rotation)
    {
        Matrix4x4 matrix(position,rotation);
        viewMatrix=matrix.Inverse();
        updateMvpMatrix();
    }

    void ForwardPipelineRenderer::SetModelTransform(const Vector3 &position, const Vector3 &rotation)
    {
        modelMatrix=Matrix4x4(position,rotation);
        updateMvpMatrix();
    }

    void ForwardPipelineRenderer::Clear(const Color &color)
    {
        if(!enableRenderBoost)
        {
            m_frame.Clear(color);
            m_depth.Clear();
        }
        else
        {
            ClearScreenJob::renderer=this;
            ClearScreenJob::color=color;

            jobScheduler.PrepareScheduler<ClearScreenJob>(m_height);
            for(int i=0;i<m_height;++i)
            {
                jobScheduler.PushJob<ClearScreenJob>(i);
            }

            jobScheduler.Schedule();
        }
        
    }

    void ForwardPipelineRenderer::ClearRow(const Color &color,int32_t row)
    {
        m_frame.ClearRow(color,row);
        m_depth.ClearRow(row);
    }

    void ForwardPipelineRenderer::DrawLineNormalize(const Vector3 &p1,const Vector3 &p2, const Color &color)
    {
        Vector2 sp1=NormalToScreen(p1);
        Vector2 sp2=NormalToScreen(p2);

        m_frame.DrawLine(sp1.x,sp1.y,sp2.x,sp2.y,color);
    }

    void ForwardPipelineRenderer::DrawLine(const Vector3 &p1,const Vector3 &p2, const Color &color)
    {
        Vector3 np1=mvpMatrix*p1;
        Vector3 np2=mvpMatrix*p2;

        DrawLineNormalize(np1,np2,color);
    }

    void ForwardPipelineRenderer::RenderDepth()
    {
        for(uint32_t i=0;i<m_width;++i)
        {
            for(uint32_t j=0;j<m_height;++j)
            {
                float d=m_depth.GetDepth(i,j);
                d=1-(d+1)/2;
                m_frame.SetPixel(i,j,Utils::ConvertColor(Vector4(d,d,d,1)));
            }
        }
    }

    void ForwardPipelineRenderer::RenderCoordinate()
    {
        DrawLine(Vector3(0,0,0),Vector3(1,0,0),Color(255,0,0,255));
        DrawLine(Vector3(0,0,0),Vector3(0,1,0),Color(0,255,0,255));
        DrawLine(Vector3(0,0,0),Vector3(0,0,1),Color(0,0,255,255));
    }

    
}