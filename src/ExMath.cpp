#include"ExMath.h"

namespace ExRenderer
{
    std::string fFormat(number_t f)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << f;
        return stream.str();
    }

    std::ostream &operator<<(std::ostream &os, const Vector2 &vec)
    {
        os << "(" << fFormat(vec.x) << "," << fFormat(vec.y) << ")";
        return os;
    }
    std::ostream &operator<<(std::ostream &os, const Vector3 &vec)
    {
        os << "(" << fFormat(vec.x) << "," << fFormat(vec.y) << "," << fFormat(vec.z) << ")";
        return os;
    }
    std::ostream &operator<<(std::ostream &os, const Vector4 &vec)
    {
        os << "(" << fFormat(vec.x) << "," << fFormat(vec.y) << "," << fFormat(vec.z) << "," << fFormat(vec.w) << ")";
        return os;
    }
    std::ostream &operator<<(std::ostream &os, const Quaternion &r)
    {
        os << "(" << r.v << "," << fFormat(r.w) << ")";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Matrix3x3 &matrix)
    {
        os << std::endl;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                os << fFormat(matrix.data[i * 3 + j]) << "\t\t";
            }
            os << std::endl;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Matrix4x4 &matrix)
    {
        os << std::endl;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                os << fFormat(matrix.data[i * 4 + j]) << "\t\t";
            }
            os << std::endl;
        }
        return os;
    }
}