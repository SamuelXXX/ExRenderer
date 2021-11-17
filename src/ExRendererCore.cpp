#include"ExRendererCore.h"

namespace ExRenderer
{
    ForwardPipelineRenderer::ForwardPipelineRenderer(uint32_t w, uint32_t h):m_width(w),m_height(h)
    {
        m_frame=FrameBuffer(w,h);
    }

    void ForwardPipelineRenderer::InitializeEnv(const char *title)
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, 0);
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
    }

    bool ForwardPipelineRenderer::UpdateEnv()
    {
        SDL_UpdateTexture(sdlTexture, nullptr, this->GetFrameBuffer(), m_width * 4);
        SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);

        SDL_Event event;
        SDL_PollEvent(&event);
        return event.type==SDL_QUIT;
    }

    void ForwardPipelineRenderer::FinalizeEnv()
    {
        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyWindow(sdlWindow);
    }

    const uint8_t *ForwardPipelineRenderer::GetFrameBuffer()
    {
        return m_frame.GetData();
    }

    void ForwardPipelineRenderer::updateMvpMatrix()
    {
        mvpMatrix=projectionMatrix*viewMatrix*modelMatrix;
    }

    void ForwardPipelineRenderer::SetCameraParams(float fov, float nearPlane, float farPlane)
    {
        float aspect=(float)m_width/m_height;
        float cotHalfFov=cos(fov/2)/sin(fov/2);
        projectionMatrix=Matrix4x4(Vector4(cotHalfFov/aspect,0,0,0),
                                    Vector4(0,cotHalfFov,0,0),
                                    Vector4(0,0,-(farPlane+nearPlane)/(farPlane-nearPlane),-1),
                                    Vector4(0,0,-2*nearPlane*farPlane/(farPlane-nearPlane),0));
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

    ScreenPosition ForwardPipelineRenderer::NormalToScreen(const Vector3 &nPos)
    {
        int32_t x=(nPos.x+1)*m_width/2;
        int32_t y=(1-nPos.y)*m_height/2;
        return ScreenPosition(x,y);
    }

    void ForwardPipelineRenderer::Clear(const Color &color)
    {
        m_frame.Clear(color);
    }

    void ForwardPipelineRenderer::DrawLineNormalize(const Vector3 &p1,const Vector3 &p2, const Color &color)
    {
        ScreenPosition sp1=NormalToScreen(p1);
        ScreenPosition sp2=NormalToScreen(p2);

        m_frame.DrawLine(sp1.x,sp1.y,sp2.x,sp2.y,color);
    }

    void ForwardPipelineRenderer::DrawLine(const Vector3 &p1,const Vector3 &p2, const Color &color)
    {
        Vector3 np1=mvpMatrix*p1;
        Vector3 np2=mvpMatrix*p2;

        DrawLineNormalize(np1,np2,color);
    }

    inline int32_t Min(int32_t v1, int32_t v2, int32_t v3)
    {
        int32_t min=v1;
        if(v2<min)
            min=v2;
        if(v3<min)
            min=v3;
        return min;
    }

    inline int32_t Max(int32_t v1, int32_t v2, int32_t v3)
    {
        int32_t max=v1;
        if(v2>max)
            max=v2;
        if(v3>max)
            max=v3;
        return max;
    }

    inline int32_t Clamp(int32_t value,int32_t min,int32_t max)
    {
        if(value<min)
            return min;
        if(value>max)
            return max;
        return value;
    }

    inline Color ConvertColor(const Vector4 &vcolor)
    {
        Vector4 color(vcolor*255);
        if(color.x>255)
        {
            color.x=255;
        }
        if(color.x<0)
        {
            color.x=0;
        }
        if(color.y>255)
        {
            color.y=255;
        }
        if(color.y<0)
        {
            color.y=0;
        }
        if(color.z>255)
        {
            color.z=255;
        }
        if(color.z<0)
        {
            color.z=0;
        }
        if(color.w>255)
        {
            color.w=255;
        }
        if(color.w<0)
        {
            color.w=0;
        }

        return Color(color.x,color.y,color.z,color.w);
    }

    void ForwardPipelineRenderer::RenderTriangle(Shader &shader,const VertexData &v1,const VertexData &v2,const VertexData &v3)
    {
        RasterizeFragment f1,f2,f3;
        shader.InjectConsts(modelMatrix,viewMatrix,projectionMatrix);
        f1.fragment=shader.VertexShader(v1);
        f2.fragment=shader.VertexShader(v2);
        f3.fragment=shader.VertexShader(v3);    

        f1.screenPos=NormalToScreen(f1.fragment.GetPosition());
        f2.screenPos=NormalToScreen(f2.fragment.GetPosition());
        f3.screenPos=NormalToScreen(f3.fragment.GetPosition());

        int32_t min_sx,max_sx,min_sy,max_sy;
        min_sx=Min(f1.screenPos.x,f2.screenPos.x,f3.screenPos.x);
        max_sx=Max(f1.screenPos.x,f2.screenPos.x,f3.screenPos.x);
        min_sy=Min(f1.screenPos.y,f2.screenPos.y,f3.screenPos.y);
        max_sy=Max(f1.screenPos.y,f2.screenPos.y,f3.screenPos.y);
        min_sx=Clamp(min_sx,0,m_width);
        max_sx=Clamp(max_sx,0,m_width);
        min_sy=Clamp(min_sy,0,m_height);
        max_sy=Clamp(max_sy,0,m_height);
        

        Matrix3x3 mulMatrix(Vector3(f1.screenPos.x,f1.screenPos.y,1),
                            Vector3(f2.screenPos.x,f2.screenPos.y,1),
                            Vector3(f3.screenPos.x,f3.screenPos.y,1));
        mulMatrix=mulMatrix.Inverse();

        std::vector<RasterizeFragment> fragments;

        for(int32_t y=min_sy;y<=max_sy;++y)
        {
            for(int32_t x=min_sx;x<=max_sx;++x)
            {
                Vector3 weight=mulMatrix*Vector3(x,y,1);
                if(weight.x>0&&weight.y>0&&weight.z>0)
                {
                    FragmentData rf=f1.fragment*weight.x+f2.fragment*weight.y+f3.fragment*weight.z;
                    Vector4 color=shader.FragmentShader(rf);
                    Color trueColor=ConvertColor(color);
                    m_frame.SetPixel(x,y,trueColor);
                }
            }
        }
    }
}