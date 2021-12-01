#include"ExTexture.h"
#include<iostream>
namespace ExRenderer
{
    Texture::Texture(const char* filePath)
    {
        m_rawImage=IMG_Load_RW(SDL_RWFromFile(filePath,"rb"),1);
    }

    Texture::~Texture()
    {
        SDL_FreeSurface(m_rawImage);
        m_rawImage=nullptr;
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

    SDL_Surface *Texture::GetRawImage()
    {
        return m_rawImage;
    }
}