#ifndef __EXRENDERER_CORE_H__
#define __EXRENDERER_CORE_H__

#include "ExMath.h"
#include "ExFrameBuffer.h"
#include "ExDepthBuffer.h"
#include "ExMesh.h"
#include <SDL2/SDL.h>
#include "ExShader.h"
#include "ExUtils.h"
#include "ExJobScheduler.h"

namespace ExRenderer
{
    struct ScreenPosition
    {
        int32_t x;
        int32_t y;
        ScreenPosition() = default;
        ScreenPosition(int32_t x, int32_t y) : x(x), y(y) {}
    };

    class ForwardPipelineRenderer
    {
        SDL_Texture *sdlTexture;
        SDL_Window *sdlWindow;
        SDL_Renderer *sdlRenderer;
        JobScheduler jobScheduler;

        uint32_t m_width, m_height;
        FrameBuffer m_frame;
        DepthBuffer m_depth;
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 mvpMatrix;

        void updateMvpMatrix();

    public:
        ForwardPipelineRenderer() = default;
        ForwardPipelineRenderer(uint32_t, uint32_t);

    public:
        void InitializeEnv(const char *);
        bool UpdateEnv();
        void FinalizeEnv();

    public:
        void SetCameraParams(float, float, float);
        void SetCameraTransform(const Vector3 &, const Vector3 &);
        void SetModelTransform(const Vector3 &, const Vector3 &);
        ScreenPosition NormalToScreen(const Vector3 &);

    public:
        void Clear(const Color &);
        void DrawLineNormalize(const Vector3 &, const Vector3 &, const Color &);
        void DrawLine(const Vector3 &, const Vector3 &, const Color &);
        void RenderDepth();
        void RenderCoordinate();

        template <class VT,class FT>
        void RenderFragment(Shader<VT,FT> &shader,const Matrix3x3 &mulMatrix, int32_t x,int32_t y, FT &f1,FT &f2,FT &f3)
        {
            Vector3 weight = mulMatrix * Vector3(x, y, 1);
            if (weight.x >= -0.001 && weight.y >= -0.001 && weight.z >= -0.001)
            {
                FT rf = f1 * weight.x + f2 * weight.y + f3 * weight.z;
                if (rf.position.z < -1 || rf.position.z > 1)
                    return;
                float d = m_depth.TestDepth(x, y, rf.position.z);
                bool zTestPassed = false;
                switch (shader.zTest)
                {
                case ZTestType::Always:
                    zTestPassed = true;
                    break;
                case ZTestType::Less:
                    zTestPassed = (d < 0);
                    break;
                case ZTestType::LessEqual:
                    zTestPassed = (d <= 0);
                    break;
                case ZTestType::Equal:
                    zTestPassed = (d == 0);
                    break;
                case ZTestType::GreatEqual:
                    zTestPassed = (d >= 0);
                    break;
                case ZTestType::Great:
                    zTestPassed = (d > 0);
                    break;
                case ZTestType::NotEqual:
                    zTestPassed = (d != 0);
                    break;
                }
                if (!zTestPassed)
                {
                    return;
                }

                Vector4 color = shader.FragmentShader(rf);
                Color trueColor = Utils::ConvertColor(color);

                m_frame.SetPixel(x, y, trueColor);
                if (zTestPassed && shader.zWrite)
                {
                    m_depth.SetDepth(x, y, rf.position.z);
                }
            }
        }

        template <class VT>
        void DrawWireMesh(Mesh<VT> &, const Color &);
        template <class VT, class FT>
        void Rasterization(Shader<VT, FT> &, FT &, FT &, FT &);
        template <class VT, class FT>
        void RenderTriangle(Shader<VT, FT> &, const VT &, const VT &, const VT &);
        template <class VT, class FT>
        void RenderMesh(Mesh<VT> &, Shader<VT, FT> &);
    };
    template<class VT,class FT>
    struct FragmentRenderJobData:public JobData
    {
        static ForwardPipelineRenderer *renderer;
        static Shader<VT,FT> *shaderPtr;
        static Matrix3x3 *mulMatrixPtr;
        static FT *f1Ptr;
        static FT *f2Ptr;
        static FT *f3Ptr;
        static uint32_t xMin;
        static uint32_t xMax;
        static uint32_t yMin;
        static uint32_t yMax;

        uint32_t startIndex;
        uint32_t endIndex;

        FragmentRenderJobData(uint32_t startIndex,uint32_t endIndex):startIndex(startIndex),endIndex(endIndex)
        {

        }

        void Run() override
        {
            int rowLength=xMax-xMin+1;
            int length=(xMax-xMin+1)*(yMax-yMin+1);
            for(int i=startIndex;i<endIndex&&i<length;++i)
            {
                int row=i/rowLength;
                int col=i%rowLength;
                int x=col+xMin;
                int y=row+yMin;
                renderer->RenderFragment(*shaderPtr,*mulMatrixPtr,x,y,*f1Ptr,*f2Ptr,*f3Ptr);
            }
            
        }
    };

    template<class VT,class FT>
    ForwardPipelineRenderer *FragmentRenderJobData<VT,FT>::renderer=nullptr;
    template<class VT,class FT>
    Shader<VT,FT> *FragmentRenderJobData<VT,FT>::shaderPtr=nullptr;
    template<class VT,class FT>
    Matrix3x3 *FragmentRenderJobData<VT,FT>::mulMatrixPtr=nullptr;
    template<class VT,class FT>
    FT *FragmentRenderJobData<VT,FT>::f1Ptr=nullptr;
    template<class VT,class FT>
    FT *FragmentRenderJobData<VT,FT>::f2Ptr=nullptr;
    template<class VT,class FT>
    FT *FragmentRenderJobData<VT,FT>::f3Ptr=nullptr;
    template<class VT,class FT>
    uint32_t FragmentRenderJobData<VT,FT>::xMin=0;
    template<class VT,class FT>
    uint32_t FragmentRenderJobData<VT,FT>::xMax=0;
    template<class VT,class FT>
    uint32_t FragmentRenderJobData<VT,FT>::yMin=0;
    template<class VT,class FT>
    uint32_t FragmentRenderJobData<VT,FT>::yMax=0;


    template <class VT>
    void ForwardPipelineRenderer::DrawWireMesh(Mesh<VT> &mesh, const Color &color)
    {
        for (auto &m : mesh)
        {
            VT *v1 = m.vertex1;
            VT *v2 = m.vertex2;
            VT *v3 = m.vertex3;
            DrawLine(v1->position, v2->position, color);
            DrawLine(v2->position, v3->position, color);
            DrawLine(v3->position, v1->position, color);
        }
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::Rasterization(Shader<VT, FT> &shader, FT &f1, FT &f2, FT &f3)
    {
        ScreenPosition sp1, sp2, sp3;
        if (!shader.doubleSide)
        {
            Vector3 faceDir = Utils::CalTriangleFaceDir(f1.position, f2.position, f3.position);
            if (faceDir.z > 0) // Cull backface
                return;
        }

        sp1 = NormalToScreen(f1.position);
        sp2 = NormalToScreen(f2.position);
        sp3 = NormalToScreen(f3.position);

        int32_t min_sx, max_sx, min_sy, max_sy;
        min_sx = Utils::Min(sp1.x - 1, sp2.x - 1, sp3.x - 1);
        max_sx = Utils::Max(sp1.x + 1, sp2.x + 1, sp3.x + 1);
        min_sy = Utils::Min(sp1.y - 1, sp2.y - 1, sp3.y - 1);
        max_sy = Utils::Max(sp1.y + 1, sp2.y + 1, sp3.y + 1);
        min_sx = Utils::Clamp(min_sx, 0, m_width);
        max_sx = Utils::Clamp(max_sx, 0, m_width);
        min_sy = Utils::Clamp(min_sy, 0, m_height);
        max_sy = Utils::Clamp(max_sy, 0, m_height);

        Matrix3x3 mulMatrix(Vector3(sp1.x, sp1.y, 1),
                            Vector3(sp2.x, sp2.y, 1),
                            Vector3(sp3.x, sp3.y, 1));
        mulMatrix = mulMatrix.Inverse();
        
        
    
        FragmentRenderJobData<VT,FT>::renderer=this;
        FragmentRenderJobData<VT,FT>::shaderPtr=&shader;
        FragmentRenderJobData<VT,FT>::mulMatrixPtr=&mulMatrix;
        FragmentRenderJobData<VT,FT>::f1Ptr=&f1;
        FragmentRenderJobData<VT,FT>::f2Ptr=&f2;
        FragmentRenderJobData<VT,FT>::f3Ptr=&f3;
        FragmentRenderJobData<VT,FT>::xMin=min_sx;
        FragmentRenderJobData<VT,FT>::xMax=max_sx;
        FragmentRenderJobData<VT,FT>::yMin=min_sy;
        FragmentRenderJobData<VT,FT>::yMax=max_sy;

        uint32_t length=(max_sy-min_sy+1)*(max_sx-min_sx+1);
        size_t requireSize=length*sizeof(FragmentRenderJobData<VT,FT>);
        jobScheduler.PrepareScheduler(requireSize);
        
        int segCount=length/MAX_THREADS+1;
        for(int i=0;i<length;i+=segCount)
        {
            FragmentRenderJobData<VT,FT> *rawPtr=(FragmentRenderJobData<VT,FT> *)jobScheduler.GetAllocatedData(sizeof(FragmentRenderJobData<VT,FT>));
            FragmentRenderJobData<VT,FT> *data=new (rawPtr) FragmentRenderJobData<VT,FT>(i,i+segCount);
            //data->Run();
            jobScheduler.PushJob(data);
        }

        jobScheduler.Schedule();
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderTriangle(Shader<VT, FT> &shader, const VT &v1, const VT &v2, const VT &v3)
    {
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);
        FT f1 = shader.VertexShader(v1);
        FT f2 = shader.VertexShader(v2);
        FT f3 = shader.VertexShader(v3);

        Rasterization(shader, f1, f2, f3);
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderMesh(Mesh<VT> &mesh, Shader<VT, FT> &shader)
    {
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);
        uint32_t vertexCount = mesh.VertexCount();
        const VT *vertices = mesh.GetVerticeBuffer();
        FT *fragments = new FT[vertexCount];
        for (int i = 0; i < vertexCount; i++)
        {
            fragments[i] = shader.VertexShader(vertices[i]);
        }
        for (auto &m : mesh)
        {
            Rasterization(shader, fragments[m.index1], fragments[m.index2], fragments[m.index3]);
        }
        delete[] fragments;
    }

    // Forward Render Pipeline

}

#endif