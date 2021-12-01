#ifndef __EXTEXTURE_H__
#define __EXTEXTURE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "ExMath.h"

namespace ExRenderer
{
    class Texture
    {
        SDL_Surface *m_rawImage;

        void safeAassignImage(SDL_Surface *newImage)
        {
            if (m_rawImage != nullptr)
            {
                m_rawImage->refcount--;
                if (m_rawImage->refcount == 0)
                {
                    std::cout << "Free Surface" << std::endl;
                    SDL_FreeSurface(m_rawImage);
                }
            }

            m_rawImage = newImage;

            if (m_rawImage != nullptr)
            {
                m_rawImage->refcount++;
            }
        }

    public:
        Texture()=default;
        Texture(const char *);
        Texture(const Texture &);
        Texture(Texture &&);
        ~Texture();
        Texture &operator=(const Texture &);
        Texture &operator=(Texture &&);

    public:
        void Info();
        void *GetPixels();
        int GetWidth();
        int GetHeight();
        Uint32 GetFormat();
        int GetPitch();
    
    private:
        number_t samplePixelChannel(uint8_t *p,Uint32 mask,Uint8 shift);
        uint8_t *getPixelStartAddress(uint32_t x,uint32_t y);

    public:
        Vector2 ClampUV(Vector2 uv);
        Vector2 RepeatUV(Vector2 uv);

        number_t SampleR(Vector2 uv);
        number_t SampleG(Vector2 uv);
        number_t SampleB(Vector2 uv);
        number_t SampleA(Vector2 uv);
        Vector3 SampleRGB(Vector2 uv);
        Vector4 SampleRGBA(Vector2 uv);
    };

    inline uint8_t *Texture::getPixelStartAddress(uint32_t x,uint32_t y)
    {
        uint8_t *pixels=(uint8_t *)GetPixels();
        return pixels+GetPitch()*y+x*m_rawImage->format->BytesPerPixel;
    }

    inline number_t Texture::samplePixelChannel(uint8_t *p,Uint32 mask,Uint8 shift)
    {
        uint32_t val=0;
        uint32_t pixel=*(uint32_t *)p;
        val=(pixel&mask)>>shift;

        return (float)val/(mask>>shift);
    }
}

#endif