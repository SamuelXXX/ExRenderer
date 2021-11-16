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

    void ForwardPipelineRenderer::SetCameraFov(float fov, float nearPlane, float farPlane)
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
        uint32_t x=(nPos.x+1)*m_width/2;
        uint32_t y=(1-nPos.y)*m_height/2;
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
        Vector4 np1=mvpMatrix*Vector4(p1);
        Vector4 np2=mvpMatrix*Vector4(p2);

        np1=np1/np1.w;
        np2=np2/np2.w;

        DrawLineNormalize(Vector3(np1.x,np1.y,np1.z),Vector3(np2.x,np2.y,np2.z),color);
    }
}