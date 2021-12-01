#include"ExRendererTestbench.h"

namespace ExRenderer::Testbench::Texture
{
    void Test()
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        ExRenderer::Texture tex("res/textures/Architecture.jpg");
        ExRenderer::Texture another=tex;
        SDL_Surface *image=another.GetRawImage();

        SDL_Window* sdlWindow = SDL_CreateWindow("Texture", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image->w, image->h, 0);
        SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, image->format->format, SDL_TEXTUREACCESS_STREAMING, image->w, image->h);

        // SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer,image);
        
        SDL_UpdateTexture(sdlTexture, nullptr, image->pixels, image->pitch);
        std::cout<<"Refcount:"<<image->refcount<<std::endl;

        
        tex.Info();
        while (true)
        {

            SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
            SDL_RenderPresent(sdlRenderer);

            SDL_Event event;
            SDL_PollEvent(&event);
            if(event.type==SDL_QUIT) break;
        }
        
        


        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyWindow(sdlWindow);
    }
}