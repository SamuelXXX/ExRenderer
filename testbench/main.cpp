#include <SDL2/SDL.h>
#include "../src/ExFrameBuffer.h"
#include "../src/ExRendererCore.h"
#include<iostream>

using namespace ExRenderer;

FrameBuffer GenFrame(uint32_t w, uint32_t h)
{
	FrameBuffer frameBuffer(w,h);
	frameBuffer.Clear(Color(0,0,0,255));
	frameBuffer.SetPixel(100,200,Color(0,255,0,255));
	frameBuffer.DrawLine(0,0,800,600,Color(255,255,0,255));
    frameBuffer.DrawCircle(300,400,100,Color(255,100,200,255));
    frameBuffer.DrawFilledCircle(200,200,150,Color(0,255,0,255));
	return frameBuffer;
}

void UpdateFrame(FrameBuffer &frameBuffer,int frameIndex)
{
    // std::cout<<frameIndex<<std::endl;
    frameBuffer.Clear(Color(0,0,0,255));
	// frameBuffer.SetPixel(100,200,Color(0,255,0,255));
	frameBuffer.DrawLine(0,0,800,600,Color(255,255,0,255));
    frameBuffer.DrawCircle(300,400,100,Color(255,100,200,255));
    frameBuffer.DrawFilledCircle(200+frameIndex,200,150,Color(0,255,0,255));
}

void UpdateRenderer(ForwardPipelineRenderer &renderer, int frameIndex)
{
    MeshBuilder mBuilder;
    VerticeType v1,v2,v3,v4;
    renderer.Clear(Color(0,0,0,255));
    v1.position=Vector3(-0.5,0.5,0);
    v2.position=Vector3(0.5,0.5,0);
    v3.position=Vector3(0.5,-0.5,0);
    v4.position=Vector3(-0.5,-0.5,0);
    mBuilder.AppendVertex(v1);
    mBuilder.AppendVertex(v2);
    mBuilder.AppendVertex(v3);
    mBuilder.AppendVertex(v4);

    mBuilder.AppendTriangle(0,1,2);
    mBuilder.AppendTriangle(0,3,2);

    Mesh m=mBuilder.GenerateMesh();
    renderer.DrawWireMesh(m,Color(255,0,0,255));
}

int main(int argc, char* args[])
{
	int w = 1280, h = 720;
    SDL_Init(SDL_INIT_EVERYTHING);

    auto window = SDL_CreateWindow("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);
	// FrameBuffer frame=GenFrame(w,h);
    ForwardPipelineRenderer fRenderer=ForwardPipelineRenderer(w,h);

    bool quit = false;
    uint32_t frameIndex=0;
    while (!quit) {
		UpdateRenderer(fRenderer,frameIndex);
        SDL_UpdateTexture(texture, nullptr, fRenderer.GetFrameBuffer(), w*4);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        ++frameIndex;

        SDL_Event event;
        {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
