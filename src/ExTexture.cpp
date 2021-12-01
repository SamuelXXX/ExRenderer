#include"ExTexture.h"
#include<iostream>
#include<math.h>
namespace ExRenderer
{
    Texture::Texture(const char* filePath)
    {
        m_rawImage=IMG_Load_RW(SDL_RWFromFile(filePath,"rb"),1);
    }

    Texture::Texture(const Texture& other)
    {
        m_rawImage=nullptr;
        safeAassignImage(other.m_rawImage);
    }

    Texture::Texture(Texture&& other)
    {
        m_rawImage=other.m_rawImage;
        other.m_rawImage=nullptr;
    }

    Texture& Texture::operator=(const Texture& other)
    {
        safeAassignImage(other.m_rawImage);
        return *this;
    }

    Texture& Texture::operator=(Texture&& other)
    {
        if(m_rawImage!=nullptr)
        {
            m_rawImage->refcount--;
            if(m_rawImage->refcount==0)
            {
                std::cout<<"Free Surface"<<std::endl;
                SDL_FreeSurface(m_rawImage);
            }
        }

        m_rawImage=other.m_rawImage;
        other.m_rawImage=nullptr;

        return *this;
    }

    Texture::~Texture()
    {
        safeAassignImage(nullptr);
    }

    void Texture::Info()
    {
        std::cout<<"Width:"<<m_rawImage->w<<std::endl;
        std::cout<<"Height:"<<m_rawImage->h<<std::endl;
        std::cout<<"Pitch:"<<m_rawImage->pitch<<std::endl;
        std::cout<<"Format:"<<m_rawImage->format->format<<std::endl;
        
        
        std::cout<<"BitsPerPixel:"<<(int)m_rawImage->format->BitsPerPixel<<std::endl;
        std::cout<<"BytesPerPixel:"<<(int)m_rawImage->format->BytesPerPixel<<std::endl;
        std::cout<<"Rmask:"<<std::hex<<m_rawImage->format->Rmask<<std::endl;
        std::cout<<"Gmask:"<<m_rawImage->format->Gmask<<std::endl;
        std::cout<<"Bmask:"<<m_rawImage->format->Bmask<<std::endl;
        std::cout<<"Amask:"<<m_rawImage->format->Amask<<std::endl;
        std::cout<<"Rshift:"<<std::dec<<(int)m_rawImage->format->Rshift<<std::endl;
        std::cout<<"Gshift:"<<(int)m_rawImage->format->Gshift<<std::endl;
        std::cout<<"Bshift:"<<(int)m_rawImage->format->Bshift<<std::endl;
        std::cout<<"Ashift:"<<(int)m_rawImage->format->Ashift<<std::endl;
    }

    void *Texture::GetPixels()
    {
        if(m_rawImage==nullptr)
        {
            return nullptr;
        }

        return m_rawImage->pixels;
    }

    int Texture::GetWidth()
    {
        if(m_rawImage==nullptr)
        {
            return 0;
        }

        return m_rawImage->w;
    }

    int Texture::GetHeight()
    {
        if(m_rawImage==nullptr)
        {
            return 0;
        }

        return m_rawImage->h;
    }

    Uint32 Texture::GetFormat()
    {
        if(m_rawImage==nullptr)
        {
            return 0;
        }

        return m_rawImage->format->format;
    }

    int Texture::GetPitch()
    {
        if(m_rawImage==nullptr)
        {
            return 0;
        }

        return m_rawImage->pitch;
    }

    Vector2 Texture::ClampUV(Vector2 uv)
    {
        Vector2 ret=uv;
        if(ret.x<0)
        {
            ret.x=0;
        }

        if(ret.x>1)
        {
            ret.x=1;
        }

        if(ret.y<0)
        {
            ret.y=0;
        }

        if(ret.y>1)
        {
            ret.y=1;
        }
        
        return ret;
    }

    Vector2 Texture::RepeatUV(Vector2 uv)
    {
        Vector2 ret=uv;
        ret.x-=floor(ret.x);
        ret.y-=floor(ret.y);
        
        return ret;
    }



    number_t Texture::SampleA(Vector2 uv)
    {
        if(m_rawImage==nullptr||m_rawImage->format->Rmask==0)
        {
            return 0;
        }

        uv=ClampUV(uv);
        uint32_t x=uv.x*m_rawImage->w;
        uint32_t y=uv.y*m_rawImage->h;
        if(x>=m_rawImage->w)
        {
            x=m_rawImage->w-1;
        }

        if(y>=m_rawImage->h)
        {
            y=m_rawImage->h-1;
        }

        uint8_t *targetPixel=getPixelStartAddress(x,y);

        return samplePixelChannel(targetPixel,m_rawImage->format->Rmask,m_rawImage->format->Rshift);
    }

    number_t Texture::SampleG(Vector2 uv)
    {
        if(m_rawImage==nullptr||m_rawImage->format->Gmask==0)
        {
            return 0;
        }

        uv=ClampUV(uv);
        uint32_t x=uv.x*m_rawImage->w;
        uint32_t y=uv.y*m_rawImage->h;
        if(x>=m_rawImage->w)
        {
            x=m_rawImage->w-1;
        }

        if(y>=m_rawImage->h)
        {
            y=m_rawImage->h-1;
        }

        uint8_t *targetPixel=getPixelStartAddress(x,y);

        return samplePixelChannel(targetPixel,m_rawImage->format->Gmask,m_rawImage->format->Gshift);
    }

    number_t Texture::SampleB(Vector2 uv)
    {
        if(m_rawImage==nullptr||m_rawImage->format->Bmask==0)
        {
            return 0;
        }

        uv=ClampUV(uv);
        uint32_t x=uv.x*m_rawImage->w;
        uint32_t y=uv.y*m_rawImage->h;
        if(x>=m_rawImage->w)
        {
            x=m_rawImage->w-1;
        }

        if(y>=m_rawImage->h)
        {
            y=m_rawImage->h-1;
        }

        uint8_t *targetPixel=getPixelStartAddress(x,y);

        return samplePixelChannel(targetPixel,m_rawImage->format->Bmask,m_rawImage->format->Bshift);
    }

    Vector3 Texture::SampleRGB(Vector2 uv)
    {
        if(m_rawImage==nullptr)
        {
            return Vector3::zero();
        }

        uv=ClampUV(uv);
        uint32_t x=uv.x*m_rawImage->w;
        uint32_t y=uv.y*m_rawImage->h;
        if(x>=m_rawImage->w)
        {
            x=m_rawImage->w-1;
        }

        if(y>=m_rawImage->h)
        {
            y=m_rawImage->h-1;
        }

        uint8_t *targetPixel=getPixelStartAddress(x,y);

        number_t r=0;
        number_t g=0;
        number_t b=0;
        if(m_rawImage->format->Rmask!=0)
        {
            r=samplePixelChannel(targetPixel,m_rawImage->format->Rmask,m_rawImage->format->Rshift);
        }
        if(m_rawImage->format->Gmask!=0)
        {
            g=samplePixelChannel(targetPixel,m_rawImage->format->Gmask,m_rawImage->format->Gshift);
        }
        if(m_rawImage->format->Bmask!=0)
        {
            b=samplePixelChannel(targetPixel,m_rawImage->format->Bmask,m_rawImage->format->Bshift);
        }

        return Vector3(r,g,b);
    }

    Vector4 Texture::SampleRGBA(Vector2 uv)
    {
        if(m_rawImage==nullptr)
        {
            return Vector3::zero();
        }

        uv=ClampUV(uv);
        uint32_t x=uv.x*m_rawImage->w;
        uint32_t y=uv.y*m_rawImage->h;
        if(x>=m_rawImage->w)
        {
            x=m_rawImage->w-1;
        }

        if(y>=m_rawImage->h)
        {
            y=m_rawImage->h-1;
        }

        uint8_t *targetPixel=getPixelStartAddress(x,y);

        number_t r=0;
        number_t g=0;
        number_t b=0;
        number_t a=0;
        if(m_rawImage->format->Rmask!=0)
        {
            r=samplePixelChannel(targetPixel,m_rawImage->format->Rmask,m_rawImage->format->Rshift);
        }
        if(m_rawImage->format->Gmask!=0)
        {
            g=samplePixelChannel(targetPixel,m_rawImage->format->Gmask,m_rawImage->format->Gshift);
        }
        if(m_rawImage->format->Bmask!=0)
        {
            b=samplePixelChannel(targetPixel,m_rawImage->format->Bmask,m_rawImage->format->Bshift);
        }
        if(m_rawImage->format->Amask!=0)
        {
            b=samplePixelChannel(targetPixel,m_rawImage->format->Amask,m_rawImage->format->Ashift);
        }

        return Vector4(r,g,b,a);
    }
}