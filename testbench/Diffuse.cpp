#include"ExRendererTestbench.h"
#include<math.h>

namespace ExRenderer::Testbench::Diffuse
{
    struct VertexData
    {
        Vector3 position;
        Vector3 normal;
    };

    struct FragmentData
    {
        Vector3 position;
        Vector3 normal;

        FragmentData operator*(number_t ratio)
        {
            FragmentData result;
            result.position=position*ratio;
            result.normal=normal*ratio;
            return result;
        }

        FragmentData operator+(const FragmentData &other)
        {
            FragmentData result;
            result.position=position+other.position;
            result.normal=normal+other.normal;
            return result;
        }
    };

    class DiffuseShader:public Shader<VertexData,FragmentData>
    {
        Vector4 baseColor;
        Vector3 worldLightDir;
        Vector4 worldLightColor;

    public:
        DiffuseShader()
        {
            zTest = ZTestType::LessEqual;
            zWrite = true;
            doubleSide=false;
        }

        void SetLight(const Vector3& dir,const Vector4&color)
        {
            worldLightDir=dir.Normalize();
            worldLightColor=color;
        }

        void SetBaseColor(const Vector4& color)
        {
            baseColor=color;
        }

    public:
        FragmentData VertexShader(const VertexData &vertex) override
        {
            FragmentData result;
            
            result.position=MVPMatrix*vertex.position;
            result.normal = (modelMatrix*Vector4(vertex.normal,0)).xyz();
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            Vector3 normal=-fragment.normal;
            // normal=normal.Normalize();
            number_t diffuse=normal*worldLightDir;
            if(diffuse<0)
                diffuse=0;
            
            Vector4 color=worldLightColor*diffuse;
            color.x*=baseColor.x;
            color.y*=baseColor.y;
            color.z*=baseColor.z;
            
            return color;
        }
    };

    void UpdateRenderer(ForwardPipelineRenderer &renderer, float deltaTime)
    {
        static DiffuseShader diffShader;
        static Mesh<VertexData> cubeMesh=MeshBuilder<VertexData>::Cube();
        static Mesh<VertexData> sphereMesh=MeshBuilder<VertexData>::Sphere();
        static Vector3 position1(0,0,0);
        static Vector3 rotation1(0,0,0);
        
        diffShader.SetLight(Vector3(1,-1,3),Vector4(1,1,1,1));
        diffShader.SetBaseColor(Vector4(0.3,0.7,0.2,1));

        renderer.Clear(Color(200, 200, 200, 255));
        renderer.SetCameraParams(3.1415/2,0.5,10);
        renderer.SetCameraTransform(Vector3(0,0,-2),Vector3::zero());

        rotation1=rotation1+Vector3(0,deltaTime,deltaTime);

        // position1=position1+Vector3(deltaTime*0.1,0,0);

        renderer.SetModelTransform(position1,rotation1); // Cube1
        renderer.RenderMesh(sphereMesh,diffShader);

        // renderer.RenderCoordinate();
        // renderer.RenderDepth();
    }

    void Test()
    {
        MathTestBench();
        ForwardPipelineRenderer fRenderer = ForwardPipelineRenderer(1920, 1080);
        fRenderer.InitializeEnv("Demo");
        uint32_t frameIndex = 0;
        clock_t lastTime=clock();
        uint32_t lastFrame=0;

        float deltaTime=0;
        clock_t lastTickTime=clock();
        
        while (true)
        {
            deltaTime=(float)(clock()-lastTickTime)/CLOCKS_PER_SEC;
            lastTickTime=clock();
            UpdateRenderer(fRenderer, deltaTime);
            frameIndex++;

            clock_t curTime=clock();
            if(curTime-lastTime>CLOCKS_PER_SEC)
            {
                uint32_t framePassed=frameIndex-lastFrame;
                float fps=(float)(framePassed)/(curTime-lastTime)*CLOCKS_PER_SEC;

                std::cout<<"FPS:"<<framePassed<<std::endl;
                lastTime=curTime;
                lastFrame=frameIndex;
            }

            if(fRenderer.UpdateEnv())
                break;
        }
        fRenderer.FinalizeEnv();
    }
}
