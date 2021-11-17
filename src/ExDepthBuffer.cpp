#include"ExDepthBuffer.h"

namespace ExRenderer
{
    DepthBuffer::DepthBuffer(uint32_t w,uint32_t h)
    {
        m_width=w;
        m_height=h;
        m_buffer=new float[w*h];
    }

    DepthBuffer::DepthBuffer(const DepthBuffer &other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=new float[this->m_width*this->m_height];

        memcpy(this->m_buffer,other.m_buffer,this->m_width*this->m_height);
    }

    DepthBuffer::DepthBuffer(DepthBuffer &&other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=other.m_buffer;

        other.m_buffer=nullptr;
    }

    DepthBuffer::~DepthBuffer()
    {
        delete [] m_buffer;
    }

    

    DepthBuffer& DepthBuffer::operator=(const DepthBuffer& other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=new float[this->m_width*this->m_height];

        memcpy(this->m_buffer,other.m_buffer,this->m_width*this->m_height);
        return *this;
    }

    DepthBuffer& DepthBuffer::operator=(DepthBuffer&& other)
    {
        this->m_width=other.m_width;
        this->m_height=other.m_height;
        this->m_buffer=other.m_buffer;

        other.m_buffer=nullptr;
        return *this;
    }

    void DepthBuffer::SetDepth(int32_t x,int32_t y,const float depth)
    {
        if(x<m_width&&y<m_height)
        {
            m_buffer[y*m_width+x]=depth;
        }
    }

    float DepthBuffer::TestDepth(int32_t x, int32_t y, const float depth)
    {
        if(x>=0&&x<m_width&&y>0&&y<m_height)
        {
            float d=m_buffer[y*m_width+x];
            return depth-d;
        }
        return depth-1;
    }

    float DepthBuffer::GetDepth(int32_t x, int32_t y)
    {
        if(x>=0&&x<m_width&&y>0&&y<m_height)
        {
            return m_buffer[y*m_width+x];
        }
        return 1;
    }

    void DepthBuffer::Clear()
    {
        for(uint32_t i=0;i<m_width*m_height;++i)
        {
            m_buffer[i]=1;
        }
    }
}