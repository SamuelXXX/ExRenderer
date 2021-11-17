#ifndef __EXRENDERER_CORE_H__
#define __EXRENDERER_CORE_H__

#include "ExMath.h"
#include "ExFrameBuffer.h"
#include "ExMesh.h"
#include <SDL2/SDL.h>
#include "ExShader.h"

namespace ExRenderer
{
    struct ScreenPosition
    {
        int32_t x;
        int32_t y;
        ScreenPosition()=default;
        ScreenPosition(int32_t x,int32_t y):x(x),y(y){}
    };

    struct RasterizeFragment
    {
        ScreenPosition screenPos;
        FragmentData fragment;
    };

    class ForwardPipelineRenderer
    {
        SDL_Texture *sdlTexture;
        SDL_Window *sdlWindow;
        SDL_Renderer *sdlRenderer;

        uint32_t m_width,m_height;
        FrameBuffer m_frame;
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 mvpMatrix;

        void updateMvpMatrix();

    public:
        ForwardPipelineRenderer() = default;
        ForwardPipelineRenderer(uint32_t,uint32_t);
    
    public:
        void InitializeEnv(const char *);
        bool UpdateEnv();
        void FinalizeEnv();

    public:
        void SetCameraParams(float,float,float);
        void SetCameraTransform(const Vector3&,const Vector3&);
        void SetModelTransform(const Vector3&,const Vector3&);
        ScreenPosition NormalToScreen(const Vector3 &);

    public:
        const uint8_t *GetFrameBuffer();

    public:
        void RendererTriangle(Shader&,const VertexData &,const VertexData &,const VertexData &);

    public:
        void Clear(const Color &);
        void DrawLineNormalize(const Vector3&,const Vector3&,const Color &);
        void DrawLine(const Vector3&,const Vector3&,const Color &);

        template <class VT>
        void DrawWireMesh(Mesh<VT> &,const Color &);
        template <class VT>
        void DrawWireMeshNormalize(Mesh<VT> &,const Color &);
    };

    template <class VT>
    void ForwardPipelineRenderer::DrawWireMeshNormalize(Mesh<VT> &mesh,const Color &color)
    {
        for(auto &m:mesh)
        {
            VT* v1=m.vertex1;
            VT* v2=m.vertex2;
            VT* v3=m.vertex3;
            DrawLineNormalize(v1->position,v2->position,color);
            DrawLineNormalize(v2->position,v3->position,color);
            DrawLineNormalize(v3->position,v1->position,color);
        }
    }

    template <class VT>
    void ForwardPipelineRenderer::DrawWireMesh(Mesh<VT> &mesh,const Color &color)
    {
        for(auto &m:mesh)
        {
            VT* v1=m.vertex1;
            VT* v2=m.vertex2;
            VT* v3=m.vertex3;
            DrawLine(v1->position,v2->position,color);
            DrawLine(v2->position,v3->position,color);
            DrawLine(v3->position,v1->position,color);
        }
    }

    // Forward Render Pipeline

}

#endif