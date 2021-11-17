#ifndef __EXDEPTH_BUFFER_H__
#define __EXDEPTH_BUFFER_H__

#include"ExMath.h"

namespace ExRenderer
{
    class DepthBuffer
    {
        uint32_t m_width, m_height;
        float *m_buffer;

    public:
        DepthBuffer() = default;
        DepthBuffer(uint32_t, uint32_t);
        DepthBuffer(const DepthBuffer&);
        DepthBuffer(DepthBuffer&&);
        ~DepthBuffer();
        DepthBuffer& operator=(const DepthBuffer&);
        DepthBuffer& operator=(DepthBuffer&&);
    

        const float *GetData(){return m_buffer;}

    public:
        void SetDepth(int32_t, int32_t, const float);
        float TestDepth(int32_t, int32_t,const float);
        float GetDepth(int32_t, int32_t);
        void Clear();
    };
}

#endif