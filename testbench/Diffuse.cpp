#include"ExRendererTestbench.h"
#include<math.h>

namespace ExRenderer::Testbench::Diffuse
{
    struct VertexData
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
    };

    struct FragmentData
    {
        Vector3 position;
        Vector3 normal;
        Vector3 lightDir;
        Vector3 worldPos;

        FragmentData operator*(number_t ratio)
        {
            FragmentData result;
            result.position=position*ratio;
            result.normal=normal*ratio;
            result.lightDir=lightDir*ratio;
            result.worldPos=worldPos*ratio;
            return result;
        }

        FragmentData operator+(const FragmentData &other)
        {
            FragmentData result;
            result.position=position+other.position;
            result.normal=normal+other.normal;
            result.lightDir=lightDir+other.lightDir;
            result.worldPos=worldPos+other.worldPos;
            return result;
        }
    };

    class DiffuseShader:public Shader<VertexData,FragmentData>
    {
        Vector4 baseColor;

    public:
        DiffuseShader()
        {
            zTest = ZTestType::LessEqual;
            zWrite = true;
            doubleSide=false;
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
            result.worldPos=modelMatrix*vertex.position;
            result.lightDir= GetLightDirection(result.worldPos);
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            Vector3 normal=-fragment.normal.Normalize();
            // normal=normal.Normalize();
            number_t diffuse=normal*fragment.lightDir.Normalize();
            if(diffuse<0)
                diffuse=0;
            
            Vector4 color=GetLightColor(fragment.worldPos)*diffuse+Vector4(0.5,0.5,0.5,1);
            color.x*=baseColor.x;
            color.y*=baseColor.y;
            color.z*=baseColor.z;
            
            return color;
        }
    };

    void UpdateRenderer(ForwardPipelineRenderer &renderer)
    {
        static DiffuseShader diffShader;
        static Mesh<VertexData> cubeMesh=MeshBuilder<VertexData>::Cube();
        static Mesh<VertexData> sphereMesh=MeshBuilder<VertexData>::Sphere();
        static Vector3 position1(0,0,0);
        static Vector3 rotation1(0,0,0);
        static Vector3 lightPosition=Vector3(1,1,1);
        static float angle=0;
        angle+=renderer.deltaTime;
        lightPosition=Vector3(3*cos(angle),1,3*sin(angle));

        renderer.ClearLights();
        renderer.AddLight<PointLight>(lightPosition,Vector4(15,15,15,5));
        lightPosition=Vector3(3*cos(angle+3.1415926),1,3*sin(angle+3.1415926));
        renderer.AddLight<PointLight>(lightPosition,Vector4(15,0,0,5));
        
        diffShader.SetBaseColor(Vector4(0.3,0.7,0.2,1));

        renderer.Clear(Color(128, 128, 128, 255));
        renderer.SetCameraParams(3.1415/2,0.5,100);
        renderer.SetCameraTransform(Vector3(0,0,-1),Vector3::zero());

        renderer.SetModelTransform(position1,rotation1); // Cube1
        renderer.RenderMesh(sphereMesh,diffShader);
    }

    void Test()
    {
        ForwardPipelineRenderer fRenderer("Diffuse",1920, 1080);
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
