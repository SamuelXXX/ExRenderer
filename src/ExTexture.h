#ifndef __EXTEXTURE_H__
#define __EXTEXTURE_H__

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>

namespace ExRenderer
{
    class Texture
    {
        SDL_Surface *m_rawImage;

        void safeAassignImage(SDL_Surface *newImage)
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

            m_rawImage=newImage;

            if(m_rawImage!=nullptr)
            {
                m_rawImage->refcount++;
            }
        }

        public:
        Texture(const char *);
        Texture(const Texture&);
        Texture(Texture&&);
        ~Texture();
        Texture& operator=(const Texture&);
        Texture& operator=(Texture&&);
        public:
        void Info();
        SDL_Surface *GetRawImage();
    };
}

#endif