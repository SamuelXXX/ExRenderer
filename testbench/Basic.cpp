#include"ExRendererTestbench.h"
#include<time.h>

namespace ExRenderer::Testbench::Basic
{
    struct VertexData
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
        Vector3 color;
    };

    struct FragmentData
    {
        Vector3 position;
        Vector3 color;

        FragmentData operator*(number_t ratio)
        {
            FragmentData result;
            result.position=position*ratio;
            result.color=color*ratio;
            return result;
        }

        FragmentData operator+(const FragmentData &other)
        {
            FragmentData result;
            result.position=position+other.position;
            result.color=color+other.color;
            return result;
        }
    };

    class ColorfulShader : public Shader<VertexData, FragmentData>
    {
    public:
        ColorfulShader()
        {
            zTest = ZTestType::LessEqual;
            zWrite = true;
            doubleSide=true;
        }

    public:
        FragmentData VertexShader(const VertexData &vertex) override
        {
            FragmentData result;
            result.color=vertex.color;
            result.position=MVPMatrix*vertex.position;
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            return Vector4(fragment.color);
        }
    };

    class CullShader : public Shader<VertexData, FragmentData>
    {
    public:
        CullShader()
        {
            zTest = ZTestType::Great;
            zWrite = false;
            doubleSide=true;
        }

    public:
        FragmentData VertexShader(const VertexData &vertex) override
        {
            FragmentData result;
            result.color=vertex.color;
            result.position=MVPMatrix*vertex.position;
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            return Vector4(0.505,0.84,0.81,1);
        }
    };

    class PureColorShader : public Shader<VertexData, FragmentData>
    {
    public:
        PureColorShader()
        {
            zTest = ZTestType::LessEqual;
            zWrite = true;
        }

    public:
        FragmentData VertexShader(const VertexData &vertex) override
        {
            FragmentData result;
            result.position=MVPMatrix*vertex.position;
            float r=vertex.position.x+0.5;
            float g=vertex.position.y+0.5;
            float b=vertex.position.z+0.5;
            result.color=Vector3(r,g,b);
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            return Vector4(fragment.color);
        }
    };

    class DepthShader : public Shader<VertexData, FragmentData>
    {
    public:
        DepthShader()
        {
            zTest = ZTestType::LessEqual;
            zWrite = true;
        }

    public:
        FragmentData VertexShader(const VertexData &vertex) override
        {
            FragmentData result;
            result.position=MVPMatrix*vertex.position;
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            float depth = 1-(fragment.position.z+1)/2;
            return Vector4(depth,depth,depth,1);
        }
    };

    void UpdateRenderer(ForwardPipelineRenderer &renderer)
    {
        static ColorfulShader colorShader;
        static PureColorShader psShader;
        static CullShader cullShader;
        static DepthShader depthShader;
        static Mesh<VertexData> cubeMesh=MeshBuilder<VertexData>::Sphere();
        static Vector3 rotation1(0,0,0),rotation2(0,0,0),rotation3(0,0,0);
        static Vector3 position1(0,0,0),position2(1,1,2),position3(0,0.6,0.5);
        static VertexData v1,v2,v3;
        static float totalTime=0;
        v1.position=Vector3(0,1,0);v1.color=Vector3(0,1,0);
        v2.position=Vector3(0,0,0);v2.color=Vector3(0,0,0);
        v3.position=Vector3(1,0,0);v3.color=Vector3(1,0,0);

        float deltaTime=renderer.deltaTime;

        totalTime+=deltaTime;
        if(totalTime>5)
        {
            renderer.enableRenderBoost=!renderer.enableRenderBoost;
            totalTime=0;
            if(renderer.enableRenderBoost)
            {
                std::cout<<"Enable Render Boosting"<<std::endl;
            }
            else
            {
                std::cout<<"Disable Render Boosting"<<std::endl;
            }
        }
        

        renderer.Clear(Color(200, 200, 200, 255));
        renderer.SetCameraParams(3.1415/2,0.5,10);
        renderer.SetCameraTransform(Vector3(0,0,-1),Vector3::zero());

        rotation1=rotation1+Vector3(0,deltaTime,deltaTime);
        rotation2=rotation2+Vector3(0,deltaTime,0);
        rotation3=rotation3+Vector3(0,deltaTime,0);

        position1=position1+Vector3(deltaTime*0.1,0,0);

        renderer.SetModelTransform(position1,rotation1); // Cube1
        renderer.RenderMesh(cubeMesh,depthShader);

        renderer.SetModelTransform(position2,rotation2); // Cube2
        renderer.RenderMesh(cubeMesh,psShader);

        renderer.SetModelTransform(position3,rotation3); // Colorful Triangle
        renderer.RenderTriangle(colorShader,v1,v2,v3);
        renderer.RenderTriangle(cullShader,v1,v2,v3);
        // renderer.RenderCoordinate();
        // renderer.RenderDepth();
    }

    void Test()
    {
        ForwardPipelineRenderer fRenderer("Basic",1920, 1080);
        while (true)
        {
            UpdateRenderer(fRenderer);
            if(fRenderer.UpdateEnv())
            {
                break;
            }
        }
    }
}