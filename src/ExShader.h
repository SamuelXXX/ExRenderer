#ifndef __EXSHADER_H__
#define __EXSHADER_H__

#include "ExFrameBuffer.h"
#include "ExMath.h"

namespace ExRenderer
{
    struct VertexData
    {
        Vector3 position;
        Vector3 color;
    };

    struct FragmentData
    {
        Vector3 position;
        Vector3 color;

        Vector3 GetPosition();
        FragmentData operator*(number_t);
        FragmentData operator+(const FragmentData&);
    };

    class Shader
    {
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 MVPMatrix;
        Matrix4x4 VPMatrix;

    public:
        void InjectConsts(const Matrix4x4&,const Matrix4x4&,const Matrix4x4&);

    public:
        FragmentData VertexShader(const VertexData&);
        Vector4 FragmentShader(const FragmentData&);
    };
}
#endif