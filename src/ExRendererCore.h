#ifndef __EXRENDERER_CORE_H__
#define __EXRENDERER_CORE_H__

#include "ExMath.h"
#include "ExFrameBuffer.h"
#include "ExMesh.h"
#include <SDL2/SDL.h>
#include "ExShader.h"
#include "ExUtils.h"

namespace ExRenderer
{
    struct ScreenPosition
    {
        int32_t x;
        int32_t y;
        ScreenPosition() = default;
        ScreenPosition(int32_t x, int32_t y) : x(x), y(y) {}
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

        uint32_t m_width, m_height;
        FrameBuffer m_frame;
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 mvpMatrix;

        void updateMvpMatrix();

    public:
        ForwardPipelineRenderer() = default;
        ForwardPipelineRenderer(uint32_t, uint32_t);

    public:
        void InitializeEnv(const char *);
        bool UpdateEnv();
        void FinalizeEnv();

    public:
        void SetCameraParams(float, float, float);
        void SetCameraTransform(const Vector3 &, const Vector3 &);
        void SetModelTransform(const Vector3 &, const Vector3 &);
        ScreenPosition NormalToScreen(const Vector3 &);

    public:
        void Clear(const Color &);
        void DrawLineNormalize(const Vector3 &, const Vector3 &, const Color &);
        void DrawLine(const Vector3 &, const Vector3 &, const Color &);

        template <class VT>
        void DrawWireMesh(Mesh<VT> &, const Color &);
        template <class VT, class FT>
        void RenderTriangle(Shader<VT, FT> &, const VT &, const VT &, const VT &);
        template <class VT, class FT>
        void RenderMesh(Mesh<VT> &, Shader<VT, FT> &);
    };

    template <class VT>
    void ForwardPipelineRenderer::DrawWireMesh(Mesh<VT> &mesh, const Color &color)
    {
        for (auto &m : mesh)
        {
            VT *v1 = m.vertex1;
            VT *v2 = m.vertex2;
            VT *v3 = m.vertex3;
            DrawLine(v1->position, v2->position, color);
            DrawLine(v2->position, v3->position, color);
            DrawLine(v3->position, v1->position, color);
        }
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderTriangle(Shader<VT, FT> &shader, const VT &v1, const VT &v2, const VT &v3)
    {
        ScreenPosition sp1, sp2, sp3;
        FT f1, f2, f3;
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);
        f1 = shader.VertexShader(v1);
        f2 = shader.VertexShader(v2);
        f3 = shader.VertexShader(v3);

        sp1 = NormalToScreen(f1.position);
        sp2 = NormalToScreen(f2.position);
        sp3 = NormalToScreen(f3.position);

        int32_t min_sx, max_sx, min_sy, max_sy;
        min_sx = Utils::Min(sp1.x, sp2.x, sp3.x);
        max_sx = Utils::Max(sp1.x, sp2.x, sp3.x);
        min_sy = Utils::Min(sp1.y, sp2.y, sp3.y);
        max_sy = Utils::Max(sp1.y, sp2.y, sp3.y);
        min_sx = Utils::Clamp(min_sx, 0, m_width);
        max_sx = Utils::Clamp(max_sx, 0, m_width);
        min_sy = Utils::Clamp(min_sy, 0, m_height);
        max_sy = Utils::Clamp(max_sy, 0, m_height);

        Matrix3x3 mulMatrix(Vector3(sp1.x, sp1.y, 1),
                            Vector3(sp2.x, sp2.y, 1),
                            Vector3(sp3.x, sp3.y, 1));
        mulMatrix = mulMatrix.Inverse();

        for (int32_t y = min_sy; y <= max_sy; ++y)
        {
            for (int32_t x = min_sx; x <= max_sx; ++x)
            {
                Vector3 weight = mulMatrix * Vector3(x, y, 1);
                if (weight.x > 0 && weight.y > 0 && weight.z > 0)
                {
                    FT rf = f1 * weight.x + f2 * weight.y + f3 * weight.z;
                    Vector4 color = shader.FragmentShader(rf);
                    Color trueColor = Utils::ConvertColor(color);
                    m_frame.SetPixel(x, y, trueColor);
                }
            }
        }
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderMesh(Mesh<VT> &mesh, Shader<VT, FT> &shader)
    {
        for (auto &m : mesh)
        {
            VT *v1 = m.vertex1;
            VT *v2 = m.vertex2;
            VT *v3 = m.vertex3;
            RenderTriangle(shader, *v1, *v2, *v3);
        }
    }

    // Forward Render Pipeline

}

#endif