#ifndef __EXUTILS_H__
#define __EXUTILS_H__

#include<stdint.h>
#include"ExMath.h"

namespace ExRenderer::Utils
{
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

    inline Vector3 CalTriangleFaceDir(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3)
    {
        Vector3 dir1=v2-v1;
        Vector3 dir2=v3-v2;
        return Cross(dir2,dir1);
    }
}

#endif