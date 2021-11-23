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

    enum BlendType{
            Zero=0,
            One,
            SrcAlpha,
            OneMinusSrcAlpha
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
        BlendType srcBlend;
        BlendType dstBlend;

        Shader<VT,FT>()
        {
            zTest=ZTestType::LessEqual;
            zWrite=true;
            doubleSide=false;
            srcBlend=BlendType::One;
            dstBlend=BlendType::Zero;
        }

        Vector4 Blend(const Vector4 &srcColor,const Vector4 &dstColor)
        {
            if(srcBlend==BlendType::One&&dstBlend==BlendType::Zero)
            {
                return srcColor;
            }

            float srcRatio=1;
            float dstRatio=0;
            switch(srcBlend)
            {
                case BlendType::Zero:srcRatio=0;break;
                case BlendType::One:srcRatio=1;break;
                case BlendType::SrcAlpha:srcRatio=srcColor.w;break;
                case BlendType::OneMinusSrcAlpha:srcRatio=1-srcColor.w;break;
            }
            switch(dstBlend)
            {
                case BlendType::Zero:dstRatio=0;break;
                case BlendType::One:dstRatio=1;break;
                case BlendType::SrcAlpha:dstRatio=srcColor.w;break;
                case BlendType::OneMinusSrcAlpha:dstRatio=1-srcColor.w;break;
            }

            return (srcColor*srcRatio+dstColor*dstRatio);
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