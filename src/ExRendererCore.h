#ifndef __EXRENDERER_CORE_H__
#define __EXRENDERER_CORE_H__

#include"ExMath.h"
#include"ExFrameBuffer.h"
#include<SDL2/SDL.h>

namespace ExRenderer
{
    class ForwardPipelineRenderer
    {
        FrameBuffer m_frame;
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;

        public:
        ForwardPipelineRenderer()=default;

        public:
        void SetModelMatrix(const Matrix4x4&);
        void SetViewMatrix(const Matrix4x4&);
        void SetProjectionMatrix(const Matrix4x4&);

    };

    // Forward Render Pipeline

}

#endif