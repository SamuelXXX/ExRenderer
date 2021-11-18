#ifndef __EXSHADER_H__
#define __EXSHADER_H__

#include "ExFrameBuffer.h"
#include "ExMath.h"

namespace ExRenderer
{
    enum ZTestType{
            Always=0,
            Less,
            LessEqual,
            Equal,
            GreatEqual,
            Great,
            NotEqual
        };

    template <class VT, class FT>
    class Shader
    {   
    protected:
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 MVPMatrix;
        Matrix4x4 VPMatrix;

    public:
        ZTestType zTest;
        bool zWrite;

    public:
        void InjectConsts(const Matrix4x4 &m, const Matrix4x4 &v, const Matrix4x4 &p)
        {
            modelMatrix = m;
            viewMatrix = v;
            projectionMatrix = p;
            MVPMatrix = p * v * m;
            VPMatrix = p * v;
        }

    public:
        virtual FT VertexShader(const VT &) = 0;
        virtual Vector4 FragmentShader(const FT &) = 0;
    };
}
#endif