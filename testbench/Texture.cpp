#include"ExRendererTestbench.h"

namespace ExRenderer::Testbench::Texture
{
    void Test()
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        ExRenderer::Texture tex("res/textures/Architecture.jpg");
        ExRenderer::Texture another=tex;
        tex.Info();
        tex=nullptr;

        SDL_Window* sdlWindow = SDL_CreateWindow("Texture", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, another.GetWidth(), another.GetHeight(), 0);
        SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, another.GetFormat(), SDL_TEXTUREACCESS_STREAMING, another.GetWidth(), another.GetHeight());

        // SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer,image);
        
        SDL_UpdateTexture(sdlTexture, nullptr, another.GetPixels(), another.GetPitch());

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