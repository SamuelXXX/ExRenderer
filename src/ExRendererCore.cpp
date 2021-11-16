#include"ExRendererCore.h"

namespace ExRenderer
{
    ForwardPipelineRenderer::ForwardPipelineRenderer(uint32_t w, uint32_t h):m_width(w),m_height(h)
    {
        m_frame=FrameBuffer(w,h);
    }

    const uint8_t *ForwardPipelineRenderer::GetFrameBuffer()
    {
        return m_frame.GetData();
    }

    void ForwardPipelineRenderer::SetCameraFov(float fov)
    {

    }

    void ForwardPipelineRenderer::SetCameraTransform(const Vector3& position, const Vector3 &rotation)
    {
        Matrix4x4 matrix(position,rotation);
        viewMatrix=matrix.Inverse();
    }

    void ForwardPipelineRenderer::SetModelTransform(const Vector3 &position, const Vector3 &rotation)
    {
        modelMatrix=Matrix4x4(position,rotation);
    }

    ScreenPosition ForwardPipelineRenderer::NormalToScreen(const Vector3 &nPos)
    {
        uint32_t x=(nPos.x+1)*m_width/2;
        uint32_t y=(1-nPos.y)*m_height/2;
        return ScreenPosition(x,y);
    }

    void ForwardPipelineRenderer::Clear(const Color &color)
    {
        m_frame.Clear(color);
    }

    void ForwardPipelineRenderer::DrawLineNormalize(const Vector3 &p1,const Vector3 &p2, const Color &color)
    {
        ScreenPosition sp1=NormalToScreen(p1);
        ScreenPosition sp2=NormalToScreen(p2);

        m_frame.DrawLine(sp1.x,sp1.y,sp2.x,sp2.y,color);
    }

    void ForwardPipelineRenderer::DrawWireMeshNormalize(Mesh &mesh,const Color &color)
    {
        for(auto &m:mesh)
        {
            VerticeType* v1=m.first;
            VerticeType* v2=m.second;
            VerticeType* v3=m.third;
            DrawLineNormalize(v1->position,v2->position,color);
            DrawLineNormalize(v2->position,v3->position,color);
            DrawLineNormalize(v3->position,v1->position,color);
        }
    }
}