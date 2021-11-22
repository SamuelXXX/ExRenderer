#ifndef __EXSHADER_H__
#define __EXSHADER_H__

#include "ExFrameBuffer.h"
#include "ExMath.h"
#include "ExLight.h"

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
        const Light *light;

    public:
        ZTestType zTest;
        bool zWrite;
        bool doubleSide;
        bool multiLightPass;

        Shader<VT,FT>()
        {
            zTest=ZTestType::LessEqual;
            zWrite=true;
            doubleSide=false;
            multiLightPass=false;
        }


    public:
        void InjectConsts(const Matrix4x4 &m, const Matrix4x4 &v, const Matrix4x4 &p)
        {
            modelMatrix = m;
            viewMatrix = v;
            projectionMatrix = p;
            MVPMatrix = p * v * m;
            VPMatrix = p * v;
        }

        void InjectLight(const Light *lightPtr)
        {
            light=lightPtr;
        }

        Vector3 GetLightDirection(const Vector3 &worldPos)
        {
            if(light)
                return light->GetDirection(worldPos);
            return Vector3::zero();
        }

        Vector4 GetLightColor(const Vector3 &worldPos)
        {
            if(light)
                return light->GetColor(worldPos);
            return Vector4::zero();
        }

    public:
        virtual FT VertexShader(const VT &) = 0;
        virtual Vector4 FragmentShader(const FT &) = 0;
    };
}
#endif