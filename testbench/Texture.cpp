#include"ExRendererTestbench.h"

namespace ExRenderer::Testbench::Texture
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
        Vector2 uv;

        FragmentData operator*(number_t ratio)
        {
            FragmentData result;
            result.position=position*ratio;
            result.uv=uv*ratio;
            return result;
        }

        FragmentData operator+(const FragmentData &other)
        {
            FragmentData result;
            result.position=position+other.position;
            result.uv=uv+other.uv;
            return result;
        }
    };

    class UnlitTexShader : public Shader<VertexData, FragmentData>
    {
        ExRenderer::Texture m_texture;
    public:
        UnlitTexShader()
        {
            zTest = ZTestType::LessEqual;
            zWrite = true;
            doubleSide=true;
            srcBlend=BlendType::SrcAlpha;
            dstBlend=BlendType::OneMinusSrcAlpha;
        }

    public:
        void SetTexture(ExRenderer::Texture tex)
        {
            m_texture=tex;
        }

        FragmentData VertexShader(const VertexData &vertex) override
        {
            FragmentData result;
            result.position=MVPMatrix*vertex.position;
            result.uv=vertex.uv;
            return result;
        }
        Vector4 FragmentShader(const FragmentData &fragment) override
        {
            // number_t x=fragment.uv.x;
            // number_t y=fragment.uv.y;
            // x=(int)(x/0.1)*0.1;
            // y=(int)(y/0.1)*0.1;
            // return Vector4(x,1-x,0,1);
            return m_texture.SampleRGBA(fragment.uv);
        }
    };

    void UpdateRenderer(ForwardPipelineRenderer &renderer)
    {
        static UnlitTexShader texShader;
        static Mesh<VertexData> cubeMesh=MeshBuilder<VertexData>::Quad();
        static ExRenderer::Texture tex("res/textures/Earth.png");

        static Vector3 position1(0,0,0);
        static Vector3 rotation1(0,0,0);

        float deltaTime=renderer.deltaTime;
        rotation1=rotation1+Vector3(0,deltaTime/5,deltaTime/5);
        
        texShader.SetTexture(tex);

        renderer.Clear(Color(128, 128, 128, 255));
        renderer.SetCameraParams(3.1415/2,0.5,100);
        renderer.SetCameraTransform(Vector3(0,0,-2),Vector3::zero());

        renderer.SetModelTransform(position1,rotation1); // Cube1
        renderer.RenderMesh(cubeMesh,texShader);
        renderer.DrawWireMesh(cubeMesh,Color(0,0,255,255));
        // renderer.RenderCoordinate();
        // renderer.RenderDepth();
    }

    void Test()
    {
        ForwardPipelineRenderer fRenderer("Texture",800, 800);
        // fRenderer.enableRenderBoost=false;
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