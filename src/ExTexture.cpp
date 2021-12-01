#include"ExTexture.h"
#include<iostream>
namespace ExRenderer
{
    Texture::Texture(const char* filePath)
    {
        SDL_RWops *rwop=SDL_RWFromFile(filePath,"rb");
        m_rawImage=IMG_LoadJPG_RW(rwop);
        std::cout<<"Image RefCount"<<m_rawImage->refcount<<std::endl;

        SDL_RWclose(rwop);
        rwop=nullptr;
    }

    Texture::~Texture()
    {
        SDL_FreeSurface(m_rawImage);
        m_rawImage=nullptr;
    }

    void Texture::Info()
    {
        Uint32 format;
        int access,w,h;
        std::cout<<"Format:"<<m_rawImage->format<<std::endl;
        std::cout<<"Width:"<<m_rawImage->w<<std::endl;
        std::cout<<"Height:"<<m_rawImage->h<<std::endl;
    }

    SDL_Surface *Texture::GetRawImage()
    {
        return m_rawImage;
    }
}