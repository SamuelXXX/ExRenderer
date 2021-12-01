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
#include <time.h>
#include "ExLight.h"

namespace ExRenderer
{
    class ForwardPipelineRenderer
    {
        SDL_Texture *sdlTexture;
        SDL_Window *sdlWindow;
        SDL_Renderer *sdlRenderer;
        JobSystem::JobScheduler jobScheduler;

        uint32_t m_width, m_height;
        FrameBuffer m_frame;
        DepthBuffer m_depth;
        Matrix4x4 modelMatrix;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 mvpMatrix;

        LightPackManager lightManager;

        void updateMvpMatrix() { mvpMatrix = projectionMatrix * viewMatrix * modelMatrix; }

    private:
        uint64_t currentFrame;
        clock_t lastUpdateTime;
        clock_t lastFpsStatTime;
        uint64_t lastFpsStatFrame;
        void updateDeltaTime()
        {
            clock_t curTime=clock();
            deltaTime=(float)(curTime-lastUpdateTime)/CLOCKS_PER_SEC;
            lastUpdateTime=curTime;

            clock_t fpsStatTimePassed=curTime-lastFpsStatTime;
            if(fpsStatTimePassed>=CLOCKS_PER_SEC)
            {
                fpsStat=(float)(currentFrame-lastFpsStatFrame)/((float)fpsStatTimePassed/CLOCKS_PER_SEC);
                std::cout<<"FPS:"<<fpsStat<<std::endl;
                lastFpsStatFrame=currentFrame;
                lastFpsStatTime=curTime;
            }

            currentFrame++;
        }

    public:
        float deltaTime;
        float fpsStat;

    public:
        ForwardPipelineRenderer() = delete;
        ForwardPipelineRenderer(const char *, uint32_t, uint32_t);
        ForwardPipelineRenderer(const ForwardPipelineRenderer &) = delete;
        ForwardPipelineRenderer(ForwardPipelineRenderer &&) = delete;
        ForwardPipelineRenderer& operator=(const ForwardPipelineRenderer&)=delete;
        ForwardPipelineRenderer& operator=(ForwardPipelineRenderer&&)=delete;
        ~ForwardPipelineRenderer();

    public:
        bool enableRenderBoost;
        bool UpdateEnv();

    public:
        void SetCameraParams(float, float, float);
        void SetCameraTransform(const Vector3 &, const Vector3 &);
        void SetModelTransform(const Vector3 &, const Vector3 &);
        Vector2 NormalToScreen(const Vector3 &nPos)
        {
            number_t x = (nPos.x + 1) * m_width / 2;
            number_t y = (1 - nPos.y) * m_height / 2;
            return Vector2(x, y);
        }
    
    public:
    void ClearLights()
    {
        lightManager.ClearLights();
    }
    template<class LT, typename ...Args>
    void AddLight(Args... args)
    {
        lightManager.AddLight<LT>(args...);
    }
    Light *FirstLight()
    {
        if(lightManager.Size()==0)
        {
            return nullptr;
        }
        return (*(lightManager.begin()));
    }
    

    public:
        void Clear(const Color &);
        void ClearRow(const Color &, int32_t);
        void DrawLineNormalize(const Vector3 &, const Vector3 &, const Color &);
        void DrawLine(const Vector3 &, const Vector3 &, const Color &);
        void RenderDepth();
        void RenderCoordinate();

        template <class VT, class FT>
        void RenderFragment(Shader<VT, FT> &, const Matrix3x3 &, int32_t, int32_t, FT &, FT &, FT &);
        template <class VT>
        void DrawWireMesh(Mesh<VT> &, const Color &);
        template <class VT, class FT>
        void Rasterization(Shader<VT, FT> &, FT &, FT &, FT &, bool);

        template <class VT, class FT>
        void RenderTriangle(Shader<VT, FT> &, const VT &, const VT &, const VT &);
        template <class VT, class FT>
        void RenderMesh(Mesh<VT> &, Shader<VT, FT> &);
        template <class VT, class FT>
        void RenderTriangleMultiLightPass(Shader<VT, FT> &, const VT &, const VT &, const VT &);
        template <class VT, class FT>
        void RenderMeshMultiLightPass(Mesh<VT> &, Shader<VT, FT> &);

    private:
        template <class VT, class FT>
        void _renderTriangle(Shader<VT, FT> &, const VT &, const VT &, const VT &);
        template <class VT, class FT>
        void _renderMesh(Mesh<VT> &, Shader<VT, FT> &);

    };

    template <class VT, class FT>
    struct VertRenderJob : public JobSystem::JobData
    {
        static ForwardPipelineRenderer *renderer;
        static Shader<VT, FT> *shaderPtr;

        static const VT *vertexBuffer;
        static FT *fragmentBuffer;

        uint32_t startIndex;
        uint32_t endIndex;

        VertRenderJob(uint32_t startIndex, uint32_t endIndex) : startIndex(startIndex), endIndex(endIndex)
        {
        }

        void Run() override
        {
            for (int i = startIndex; i < endIndex; i++)
            {
                fragmentBuffer[i] = shaderPtr->VertexShader(vertexBuffer[i]);
            }
        }
    };
    template <class VT, class FT>
    ForwardPipelineRenderer *VertRenderJob<VT, FT>::renderer = nullptr;
    template <class VT, class FT>
    Shader<VT, FT> *VertRenderJob<VT, FT>::shaderPtr = nullptr;
    template <class VT, class FT>
    const VT *VertRenderJob<VT, FT>::vertexBuffer = nullptr;
    template <class VT, class FT>
    FT *VertRenderJob<VT, FT>::fragmentBuffer = nullptr;

    template <class VT, class FT>
    struct RasterizationJob : public JobSystem::JobData
    {
        static ForwardPipelineRenderer *renderer;
        static Shader<VT, FT> *shaderPtr;
        static FT *fragments;

        uint32_t index1, index2, index3;

        RasterizationJob(uint32_t index1, uint32_t index2, uint32_t index3) : index1(index1), index2(index2), index3(index3)
        {
        }

        void Run() override
        {
            renderer->Rasterization(*shaderPtr, fragments[index1], fragments[index2], fragments[index3], false);
        }
    };
    template <class VT, class FT>
    ForwardPipelineRenderer *RasterizationJob<VT, FT>::renderer = nullptr;
    template <class VT, class FT>
    Shader<VT, FT> *RasterizationJob<VT, FT>::shaderPtr = nullptr;
    template <class VT, class FT>
    FT *RasterizationJob<VT, FT>::fragments = nullptr;

    template <class VT, class FT>
    struct FragRenderJob : public JobSystem::JobData
    {
        static ForwardPipelineRenderer *renderer;
        static Shader<VT, FT> *shaderPtr;
        static Matrix3x3 *weightMatrixPtr;
        static FT *f1Ptr;
        static FT *f2Ptr;
        static FT *f3Ptr;
        static uint32_t xMin;
        static uint32_t xMax;
        static uint32_t yMin;
        static uint32_t yMax;

        uint32_t startIndex;
        uint32_t endIndex;

        FragRenderJob(uint32_t startIndex, uint32_t endIndex) : startIndex(startIndex), endIndex(endIndex)
        {
        }

        void Run() override
        {
            int rowLength = xMax - xMin + 1;
            int fragsCount = (xMax - xMin + 1) * (yMax - yMin + 1);
            for (int i = startIndex; i < endIndex && i < fragsCount; ++i)
            {
                int row = i / rowLength;
                int col = i % rowLength;
                int x = col + xMin;
                int y = row + yMin;
                renderer->RenderFragment(*shaderPtr, *weightMatrixPtr, x, y, *f1Ptr, *f2Ptr, *f3Ptr);
            }
        }
    };
    template <class VT, class FT>
    ForwardPipelineRenderer *FragRenderJob<VT, FT>::renderer = nullptr;
    template <class VT, class FT>
    Shader<VT, FT> *FragRenderJob<VT, FT>::shaderPtr = nullptr;
    template <class VT, class FT>
    Matrix3x3 *FragRenderJob<VT, FT>::weightMatrixPtr = nullptr;
    template <class VT, class FT>
    FT *FragRenderJob<VT, FT>::f1Ptr = nullptr;
    template <class VT, class FT>
    FT *FragRenderJob<VT, FT>::f2Ptr = nullptr;
    template <class VT, class FT>
    FT *FragRenderJob<VT, FT>::f3Ptr = nullptr;
    template <class VT, class FT>
    uint32_t FragRenderJob<VT, FT>::xMin = 0;
    template <class VT, class FT>
    uint32_t FragRenderJob<VT, FT>::xMax = 0;
    template <class VT, class FT>
    uint32_t FragRenderJob<VT, FT>::yMin = 0;
    template <class VT, class FT>
    uint32_t FragRenderJob<VT, FT>::yMax = 0;

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
    void ForwardPipelineRenderer::RenderFragment(Shader<VT, FT> &shader, const Matrix3x3 &weightMatrix, int32_t x, int32_t y, FT &f1, FT &f2, FT &f3)
    {
        Vector3 weight = weightMatrix * Vector3(x + 0.5, y + 0.5, 1);

        // Correction of perspective interplotation
        number_t w1b=weight.x;
        number_t w2b=weight.y;
        number_t w3b=weight.z;

        Matrix4x4 inverseP=projectionMatrix.Inverse();
        number_t z1=(inverseP*f1.position).z;
        number_t z2=(inverseP*f2.position).z;
        number_t z3=(inverseP*f3.position).z;

        number_t sum=w1b*z2*z3+w2b*z1*z3+w3b*z1*z2;
        number_t w1=w1b*z2*z3/sum;
        number_t w2=w2b*z1*z3/sum;
        number_t w3=w3b*z2*z1/sum;

        if (w1 >= -0.00 && w2 >= -0.00 && w3 >= -0.00)
        {
            FT rf = f1 * w1 + f2 * w2 + f3 * w3;

            if (rf.position.z < -1 || rf.position.z > 1)
                return;

            float d = m_depth.TestDepth(x, y, rf.position.z); // Early-Z pass
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

            Vector4 srcColor = shader.FragmentShader(rf);
            Vector4 dstColor=m_frame.GetPixelVec4(x,y);
            Vector4 finalColor=shader.Blend(srcColor,dstColor);

            m_frame.SetPixelVec4(x, y, finalColor);
            if (zTestPassed && shader.zWrite)
            {
                m_depth.SetDepth(x, y, rf.position.z);
            }
        }
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::Rasterization(Shader<VT, FT> &shader, FT &f1, FT &f2, FT &f3, bool boost)
    {
        Vector2 sp1, sp2, sp3;
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

        Matrix3x3 weightMatrix(Vector3(sp1.x, sp1.y, 1),
                               Vector3(sp2.x, sp2.y, 1),
                               Vector3(sp3.x, sp3.y, 1));
        weightMatrix = weightMatrix.Inverse();

        uint32_t fragCount = (max_sy - min_sy + 1) * (max_sx - min_sx + 1);
        int batchCount = fragCount / MAX_THREADS + 1;

        if (boost && batchCount >= 100)
        {
            FragRenderJob<VT, FT>::renderer = this;
            FragRenderJob<VT, FT>::shaderPtr = &shader;
            FragRenderJob<VT, FT>::weightMatrixPtr = &weightMatrix;
            FragRenderJob<VT, FT>::f1Ptr = &f1;
            FragRenderJob<VT, FT>::f2Ptr = &f2;
            FragRenderJob<VT, FT>::f3Ptr = &f3;
            FragRenderJob<VT, FT>::xMin = min_sx;
            FragRenderJob<VT, FT>::xMax = max_sx;
            FragRenderJob<VT, FT>::yMin = min_sy;
            FragRenderJob<VT, FT>::yMax = max_sy;

            jobScheduler.PrepareScheduler<FragRenderJob<VT, FT>>(fragCount);
            for (int i = 0; i < fragCount; i += batchCount)
            {
                jobScheduler.PushJob<FragRenderJob<VT, FT>>(i, i + batchCount);
            }
            jobScheduler.Schedule();
        }
        else
        {
            for (int x = min_sx; x <= max_sx; x++)
            {
                for (int y = min_sy; y <= max_sy; y++)
                {
                    RenderFragment(shader, weightMatrix, x, y, f1, f2, f3);
                }
            }
        }
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderTriangle(Shader<VT, FT> &shader, const VT &v1, const VT &v2, const VT &v3)
    {
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);
        shader.InjectLight(FirstLight());

        _renderTriangle(shader,v1,v2,v3);
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderTriangleMultiLightPass(Shader<VT, FT> &shader, const VT &v1, const VT &v2, const VT &v3)
    {
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);

        for(auto light:lightManager)
        {
            if(light==FirstLight())
                continue;

            shader.InjectLight(light);
            _renderTriangle(shader,v1,v2,v3);
        }
        
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::_renderTriangle(Shader<VT, FT> &shader, const VT &v1, const VT &v2, const VT &v3)
    {
        FT f1 = shader.VertexShader(v1);
        FT f2 = shader.VertexShader(v2);
        FT f3 = shader.VertexShader(v3);
        Rasterization(shader, f1, f2, f3, enableRenderBoost);
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderMesh(Mesh<VT> &mesh, Shader<VT, FT> &shader)
    {
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);
        shader.InjectLight(FirstLight());

        _renderMesh(mesh,shader);
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::RenderMeshMultiLightPass(Mesh<VT> &mesh, Shader<VT, FT> &shader)
    {
        shader.InjectConsts(modelMatrix, viewMatrix, projectionMatrix);

        for(auto light:lightManager)
        {
            if(light==FirstLight())
                continue;

            shader.InjectLight(light);
            _renderMesh(mesh,shader);
        }
    }

    template <class VT, class FT>
    void ForwardPipelineRenderer::_renderMesh(Mesh<VT> &mesh, Shader<VT, FT> &shader)
    {
        uint32_t vertexCount = mesh.VertexCount();
        const VT *vertices = mesh.GetVerticeBuffer();
        FT *fragments = new FT[vertexCount];

        VertRenderJob<VT, FT>::renderer = this;
        VertRenderJob<VT, FT>::shaderPtr = &shader;
        VertRenderJob<VT, FT>::vertexBuffer = vertices;
        VertRenderJob<VT, FT>::fragmentBuffer = fragments;

        if (!enableRenderBoost)
        {
            for (int i = 0; i < vertexCount; i++)
            {
                fragments[i] = shader.VertexShader(vertices[i]);
            }
        }
        else
        {
            jobScheduler.PrepareScheduler<VertRenderJob<VT, FT>>(vertexCount);
            int batchCount = vertexCount / MAX_THREADS + 1;
            for (int i = 0; i < vertexCount; i += batchCount)
            {
                int endIndex = i + batchCount;
                if (endIndex > vertexCount)
                    endIndex = vertexCount;
                jobScheduler.PushJob<VertRenderJob<VT, FT>>(i, endIndex);
            }
            jobScheduler.Schedule();
        }

        if (!enableRenderBoost)
        {
            for (auto &m : mesh)
            {
                Rasterization(shader, fragments[m.index1], fragments[m.index2], fragments[m.index3], false);
            }
        }
        else
        {
            if (mesh.TriangleCount() <= MAX_THREADS)
            {
                for (auto &m : mesh)
                {
                    Rasterization(shader, fragments[m.index1], fragments[m.index2], fragments[m.index3], true);
                }
            }
            else
            {
                RasterizationJob<VT, FT>::renderer = this;
                RasterizationJob<VT, FT>::fragments = fragments;
                RasterizationJob<VT, FT>::shaderPtr = &shader;

                jobScheduler.PrepareScheduler<RasterizationJob<VT, FT>>(mesh.TriangleCount());
                for (auto &m : mesh)
                {
                    jobScheduler.PushJob<RasterizationJob<VT, FT>>(m.index1, m.index2, m.index3);
                }
                jobScheduler.Schedule();
            }
        }

        delete[] fragments;
    }


}

#endif