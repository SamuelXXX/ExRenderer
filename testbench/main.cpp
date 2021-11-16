#include <SDL2/SDL.h>
#include "../src/ExFrameBuffer.h"
#include "../src/ExRendererCore.h"
#include <iostream>

using namespace ExRenderer;

FrameBuffer GenFrame(uint32_t w, uint32_t h)
{
    FrameBuffer frameBuffer(w, h);
    frameBuffer.Clear(Color(0, 0, 0, 255));
    frameBuffer.SetPixel(100, 200, Color(0, 255, 0, 255));
    frameBuffer.DrawLine(0, 0, 800, 600, Color(255, 255, 0, 255));
    frameBuffer.DrawCircle(300, 400, 100, Color(255, 100, 200, 255));
    frameBuffer.DrawFilledCircle(200, 200, 150, Color(0, 255, 0, 255));
    return frameBuffer;
}

void UpdateFrame(FrameBuffer &frameBuffer, int frameIndex)
{
    // std::cout<<frameIndex<<std::endl;
    frameBuffer.Clear(Color(0, 0, 0, 255));
    // frameBuffer.SetPixel(100,200,Color(0,255,0,255));
    frameBuffer.DrawLine(0, 0, 800, 600, Color(255, 255, 0, 255));
    frameBuffer.DrawCircle(300, 400, 100, Color(255, 100, 200, 255));
    frameBuffer.DrawFilledCircle(200 + frameIndex, 200, 150, Color(0, 255, 0, 255));
}

void AddQuad(MeshBuilder &mBuilder,uint32_t index0,uint32_t index1,uint32_t index2,uint32_t index3)
{
    mBuilder.AddTriangle(index0, index1, index2);
    mBuilder.AddTriangle(index0, index3, index2);
}

void UpdateRenderer(ForwardPipelineRenderer &renderer, int frameIndex)
{
    renderer.Clear(Color(0, 0, 0, 255));
    renderer.SetCameraFov(3.1415/2,0.05,100);
    renderer.SetCameraTransform(Vector3(0,0,-3),Vector3::zero());
    renderer.SetModelTransform(Vector3::zero(),Vector3(0,(float)frameIndex/100,0));
    
    Mesh cubeMesh=MeshBuilder::Cube();
    renderer.DrawWireMesh(cubeMesh, Color(255, 0, 0, 255));
    // renderer.DrawLine(Vector3(0,1,0),Vector3(0,-1,-1),Color(255, 0, 0, 255));
}

int main(int argc, char *args[])
{
    MathTestBench();

    int w = 1280, h = 720;
    SDL_Init(SDL_INIT_EVERYTHING);

    auto window = SDL_CreateWindow("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);
    // FrameBuffer frame=GenFrame(w,h);
    ForwardPipelineRenderer fRenderer = ForwardPipelineRenderer(w, h);

    bool quit = false;
    uint32_t frameIndex = 0;
    while (!quit)
    {
        UpdateRenderer(fRenderer, frameIndex);
        SDL_UpdateTexture(texture, nullptr, fRenderer.GetFrameBuffer(), w * 4);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        ++frameIndex;

        SDL_Event event;
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
