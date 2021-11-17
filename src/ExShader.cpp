#include"ExShader.h"

namespace ExRenderer
{
    Vector3 FragmentData::GetPosition()
    {
        return position;
    }

    FragmentData FragmentData::operator*(number_t ratio)
    {
        FragmentData result;
        result.position=position*ratio;
        result.color=color*ratio;
        return result;
    }

    FragmentData FragmentData::operator+(const FragmentData& other)
    {
        FragmentData result;
        result.position=position+other.position;
        result.color=color+other.color;
        return result;
    }

    void Shader::InjectConsts(const Matrix4x4 &m,const Matrix4x4 &v,const Matrix4x4 &p)
    {
        modelMatrix=m;
        viewMatrix=v;
        projectionMatrix=p;
        MVPMatrix=p*v*m;
        VPMatrix=p*v;
    }

    FragmentData Shader::VertexShader(const VertexData &vertex)
    {
        FragmentData result;
        result.color=vertex.color;
        result.position=MVPMatrix*vertex.position;
        return result;
    }

    Vector4 Shader::FragmentShader(const FragmentData &fragment)
    {
        return Vector4(fragment.color);
    }

     
}