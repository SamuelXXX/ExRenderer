#include"ExTexture.h"
#include<iostream>
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
}