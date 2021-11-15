#ifndef __EXFRAMEBUFFER_H__
#define __EXFRAMEBUFFER_H__

#include <stdint.h>

namespace ExRenderer
{
    struct Color
    {
        uint8_t b,g,r,a;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
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
        void DrawPixel(uint32_t, uint32_t, const Color &);
        void DrawPixel(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t);
        void DrawLine(uint32_t, uint32_t,uint32_t, uint32_t, const Color &);
        void Clear(const Color&);
    };
}

#endif