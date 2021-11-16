#ifndef __EXRENDERER_CORE_H__
#define __EXRENDERER_CORE_H__

#include "ExMath.h"
#include "ExFrameBuffer.h"
#include "ExMesh.h"
#include <SDL2/SDL.h>

namespace ExRenderer
{
    struct ScreenPosition
    {
        uint32_t x;
        uint32_t y;
        ScreenPosition(uint32_t x,uint32_t y):x(x),y(y){}
    };
    class ForwardPipelineRenderer
    {
        uint32_t m_width,m_height;
        FrameBuffer m_frame;
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;

    public:
        ForwardPipelineRenderer() = default;
        ForwardPipelineRenderer(uint32_t,uint32_t);

    public:
        void SetCameraFov(float);
        void SetCameraTransform(const Vector3&,const Vector3&);
        void SetModelTransform(const Vector3&,const Vector3&);
        ScreenPosition NormalToScreen(const Vector3 &);

    public:
        const uint8_t *GetFrameBuffer();

    public:
        void Clear(const Color &);
        void DrawWireMeshNormalize(Mesh &,const Color &);
        void DrawLineNormalize(const Vector3&,const Vector3&,const Color &);
    };

    // Forward Render Pipeline

}

#endif