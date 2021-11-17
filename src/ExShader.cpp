#include"ExShader.h"

namespace ExRenderer
{
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

    FragmentData DemoShader::VertexShader(const VertexData &vertex)
    {
        FragmentData result;
        result.color=vertex.color;
        result.position=MVPMatrix*vertex.position;
        return result;
    }

    Vector4 DemoShader::FragmentShader(const FragmentData &fragment)
    {
        return Vector4(fragment.color);
    }

    FragmentData PureColorShader::VertexShader(const VertexData &vertex)
    {
        FragmentData result;
        result.position=MVPMatrix*vertex.position;
        return result;
    }

    Vector4 PureColorShader::FragmentShader(const FragmentData &fragment)
    {
        return color;
    }

     
}