#include"ExFrameBuffer.h"

#include<assert.h>
#include<iostream>

namespace ExRenderer
{
    FrameBuffer::FrameBuffer(uint32_t w,uint32_t h)
    {
        m_width=w;
        m_height=h;
        m_buffer=new uint8_t[w*h*4];
    }

    FrameBuffer::FrameBuffer(const FrameBuffer &other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=new uint8_t[this->m_width*this->m_height*4];

        memcpy(this->m_buffer,other.m_buffer,this->m_width*this->m_height*4);
    }

    FrameBuffer::FrameBuffer(FrameBuffer &&other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=other.m_buffer;

        other.m_buffer=nullptr;
    }

    FrameBuffer::~FrameBuffer()
    {
        delete [] m_buffer;
    }

    

    FrameBuffer& FrameBuffer::operator=(const FrameBuffer& other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=new uint8_t[this->m_width*this->m_height*4];

        memcpy(this->m_buffer,other.m_buffer,this->m_width*this->m_height*4);
        return *this;
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=other.m_buffer;

        other.m_buffer=nullptr;
        return *this;
    }

    void FrameBuffer::DrawPixel(uint32_t x, uint32_t y, const Color& color)
    {
        uint32_t idx=(y*m_width+x)*4;
        m_buffer[idx]=color.b;
        m_buffer[idx+1]=color.g;
        m_buffer[idx+2]=color.r;
        m_buffer[idx+3]=color.a;
    }

    void FrameBuffer::DrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b,uint8_t a)
    {
        uint32_t idx=(y*m_width+x)*4;
        m_buffer[idx]=b;
        m_buffer[idx+1]=g;
        m_buffer[idx+2]=r;
        m_buffer[idx+3]=a;
    }

    void FrameBuffer::DrawLine(uint32_t xBegin, uint32_t yBegin,uint32_t xEnd, uint32_t yEnd, const Color &color)
    {
        int dx = xEnd-xBegin, sx = xBegin<xEnd ? 1 : -1;
        int dy = yEnd-yBegin, sy = yBegin<yEnd ? 1 : -1; 
        if(dx<0) dx=-dx;
        if(dy<0) dy=-dy;
        int err = (dx>dy ? dx : -dy)/2, e2;
        
        for(;;){
            DrawPixel(xBegin,yBegin,color);
            if (xBegin==xEnd && yBegin==yEnd) break;
            e2 = err;
            if (e2 >-dx) { err -= dy; xBegin += sx; }
            if (e2 < dy) { err += dx; yBegin += sy; }
        }
    }

    void FrameBuffer::Clear(const Color& color)
    {
        for(uint32_t i=0;i<m_width*m_height*4;i+=4)
        {
            m_buffer[i]=color.b;
            m_buffer[i+1]=color.g;
            m_buffer[i+2]=color.r;
            m_buffer[i+3]=color.a;
        }
    }
}