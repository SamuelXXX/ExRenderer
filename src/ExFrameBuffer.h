#ifndef __EXFRAMEBUFFER_H__
#define __EXFRAMEBUFFER_H__

#include <stdint.h>

namespace ExRenderer
{
    struct Color
    {
        uint8_t b,g,r,a;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

        Color AntiAlisingMix(const Color &back, uint32_t error, uint32_t stdError)
        {
            int offError=stdError-error;
            return Color(this->r*error/stdError+back.r*offError/stdError,
                            this->g*error/stdError+back.g*offError/stdError,
                            this->b*error/stdError+back.b*offError/stdError,
                            255);
        }
    };

    class FrameBuffer
    {
        uint32_t m_width, m_height;
        uint8_t *m_buffer;

    public:
        FrameBuffer() = default;
        FrameBuffer(uint32_t, uint32_t);
        FrameBuffer(const FrameBuffer&);
        FrameBuffer(FrameBuffer&&);
        ~FrameBuffer();
        FrameBuffer& operator=(const FrameBuffer&);
        FrameBuffer& operator=(FrameBuffer&&);
    

        const uint8_t *GetData(){return m_buffer;}

    public:
        void SetPixel(uint32_t, uint32_t, const Color &);
        void SetPixel(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t);
        Color GetPixel(uint32_t, uint32_t);
        void MixPixel(uint32_t, uint32_t, const Color &, uint32_t);

    public: // Primitive
        void DrawLine(int32_t, int32_t,int32_t, int32_t, const Color &);
        void DrawCircle(uint32_t,uint32_t,uint32_t,const Color &);
        void DrawFilledCircle(uint32_t,uint32_t,uint32_t,const Color &);
        void Clear(const Color&);
    };
}

#endif