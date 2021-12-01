#ifndef __EXTEXTURE_H__
#define __EXTEXTURE_H__

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

namespace ExRenderer
{
    class Texture
    {
        public:
        SDL_Surface *m_rawImage;
        public:
        Texture(const char *);
        Texture(const Texture&)=delete;
        Texture(Texture&&)=delete;
        ~Texture();
        Texture& operator=(const Texture&)=delete;
        Texture& operator=(Texture&&)=delete;
        public:
        void Info();
        SDL_Surface *GetRawImage();
    };
}

#endif