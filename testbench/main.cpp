#include <SDL2/SDL.h>
#include "../src/ExFrameBuffer.h"
#include "../src/ExRendererCore.h"
#include <iostream>

using namespace ExRenderer;

void UpdateRenderer(ForwardPipelineRenderer &renderer, int frameIndex)
{
    static Mesh<BasicVertex> cubeMesh=MeshBuilder<BasicVertex>::Cube();

    renderer.Clear(Color(0, 0, 0, 255));
    renderer.SetCameraParams(3.1415/3,0.05,100);
    renderer.SetCameraTransform(Vector3(0,0,-3),Vector3::zero());
    renderer.SetModelTransform(Vector3::zero(),Vector3(0,(float)frameIndex/100,(float)frameIndex/200));
    
    renderer.DrawWireMesh(cubeMesh, Color(255, 0, 0, 255));
}

int main(int argc, char *args[])
{
    MathTestBench();
    ForwardPipelineRenderer fRenderer = ForwardPipelineRenderer(1280, 720);
    fRenderer.InitializeEnv("Demo");
    uint32_t frameIndex = 0;
    while (true)
    {
        UpdateRenderer(fRenderer, frameIndex++);
        if(fRenderer.UpdateEnv())
            break;
    }
    fRenderer.FinalizeEnv();
    return 0;
}
