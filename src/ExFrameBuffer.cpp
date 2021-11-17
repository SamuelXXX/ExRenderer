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

    void FrameBuffer::SetPixel(uint32_t x, uint32_t y, const Color& color)
    {
        if(x<m_width&&y<m_height)
        {
            uint32_t idx=(y*m_width+x)*4;
            m_buffer[idx]=color.b;
            m_buffer[idx+1]=color.g;
            m_buffer[idx+2]=color.r;
            m_buffer[idx+3]=color.a;
        }
    }

    void FrameBuffer::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b,uint8_t a)
    {
        if(x<m_width&&y<m_height)
        {
            uint32_t idx=(y*m_width+x)*4;
            m_buffer[idx]=b;
            m_buffer[idx+1]=g;
            m_buffer[idx+2]=r;
            m_buffer[idx+3]=a;
        }
    }

    Color FrameBuffer::GetPixel(uint32_t x, uint32_t y)
    {
        uint32_t idx=(y*m_width+x)*4;
        return Color(m_buffer[idx+2],m_buffer[idx+1],m_buffer[idx],m_buffer[idx+3]);
    }

    void FrameBuffer::MixPixel(uint32_t x, uint32_t y, const Color &color, uint32_t alpha)
    {
        if(x<m_width&&y<m_height)
        {
            uint32_t idx=(y*m_width+x)*4;
            m_buffer[idx]=color.b*alpha/255+m_buffer[idx]*(255-alpha)/255;
            m_buffer[idx+1]=color.g*alpha/255+m_buffer[idx+1]*(255-alpha)/255;
            m_buffer[idx+2]=color.r*alpha/255+m_buffer[idx+2]*(255-alpha)/255;
        }
    }

    void FrameBuffer::DrawLine(int32_t xBegin, int32_t yBegin,int32_t xEnd, int32_t yEnd, const Color &color)
    {
        int dx = xEnd-xBegin, sx = xBegin<xEnd ? 1 : -1;
        int dy = yEnd-yBegin, sy = yBegin<yEnd ? 1 : -1; 
        if(dx<0) dx=-dx;
        if(dy<0) dy=-dy;
        int err = (dx>dy ? dx : -dy)/2, e2;
        
        for(;;){
            SetPixel(xBegin,yBegin,color);
            if (xBegin==xEnd && yBegin==yEnd) break;
            e2 = err;
            if (e2 >-dx) { err -= dy; xBegin += sx; }
            if (e2 < dy) { err += dx; yBegin += sy; }
        }
    }

    void FrameBuffer::DrawCircle(uint32_t x,uint32_t y, uint32_t radius, const Color &color)
    {
        int dx=radius,dy=0;
        int radius_squre=radius*radius;

        while(dx>dy)
        {
            SetPixel(x+dx,y+dy,color);
            SetPixel(x+dx,y-dy,color);
            SetPixel(x-dx,y+dy,color);
            SetPixel(x-dx,y-dy,color);

            SetPixel(x+dy,y+dx,color);
            SetPixel(x+dy,y-dx,color);
            SetPixel(x-dy,y+dx,color);
            SetPixel(x-dy,y-dx,color);

            ++dy;
            if(dx*dx+dy*dy>radius_squre)
            {
                --dx;
            }
        }
    }

    void FrameBuffer::DrawFilledCircle(uint32_t x,uint32_t y, uint32_t radius, const Color &color)
    {
        int dx=radius,dy=0;
        int stdErr=8*radius;
        int cErr=4*radius*radius-4*radius+1;
        int valDx=dx;

        while(dx>=dy)
        {
            dx=valDx;
            int error=4*(dx*dx+dy*dy)-cErr;
            while (error>stdErr)
            {
                --dx;
                error=4*(dx*dx+dy*dy)-cErr;
            }
            valDx=dx;
            while (error>0)
            {
                uint32_t norError=(stdErr-error)*255/stdErr;
                MixPixel(x+dx,y+dy,color,norError);
                MixPixel(x+dx,y-dy,color,norError);
                MixPixel(x-dx,y+dy,color,norError);
                MixPixel(x-dx,y-dy,color,norError);

                MixPixel(x+dy,y+dx,color,norError);
                MixPixel(x+dy,y-dx,color,norError);
                MixPixel(x-dy,y+dx,color,norError);
                MixPixel(x-dy,y-dx,color,norError);
                --dx;
                error=4*(dx*dx+dy*dy)-cErr;
            }

            for(int i=-dx;i<=dx;++i)
            {
                SetPixel(x+i,y+dy,color);
                SetPixel(x+i,y-dy,color);
            }

            for(int i=-dy;i<=dy;++i)
            {
                SetPixel(x+i,y+dx,color);
                SetPixel(x+i,y-dx,color);
            }

            ++dy;
            
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