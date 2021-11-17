#include <SDL2/SDL.h>
#include "../src/ExFrameBuffer.h"
#include "../src/ExRendererCore.h"
#include <iostream>

using namespace ExRenderer;

void UpdateRenderer(ForwardPipelineRenderer &renderer, int frameIndex)
{
    static Mesh<VertexData> cubeMesh=MeshBuilder<VertexData>::Cube();
    Shader shader;
    std::cout<<frameIndex<<std::endl;
    renderer.Clear(Color(255, 255, 255, 255));
    renderer.SetCameraParams(3.1415/3,0.05,100);
    renderer.SetCameraTransform(Vector3(0,0,-5),Vector3::zero());
    renderer.SetModelTransform(Vector3::zero(),Vector3(0,(float)frameIndex/100,(float)frameIndex/200));
    
    
    // renderer.RenderMesh(cubeMesh,shader);
    renderer.DrawWireMesh(cubeMesh, Color(255, 0, 0, 255));


    VertexData v1,v2,v3;
    v1.position=Vector3(1,0,0);v1.color=Vector3(1,0,0);
    v2.position=Vector3(0,0,0);v2.color=Vector3(0,1,0);
    v3.position=Vector3(0,1,0);v3.color=Vector3(0,0,1);
    
    renderer.RenderTriangle(shader,v1,v2,v3);
}

int main(int argc, char *args[])
{
    MathTestBench();
    ForwardPipelineRenderer fRenderer = ForwardPipelineRenderer(800, 600);
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
