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

    void MathTestBench()
    {
        Vector3 v1(0.5, 0.6, 0.7);
        Vector3 v2(0.1, 0.6, 0.7);
        Vector3 v = Cross(v1, v2);

        number_t t1 = v * v1;
        number_t t2 = v * v2;
        // std::cout<<t1<<std::endl;
        // std::cout<<t2<<std::endl;
        number_t PI = 3.1415926f;

        Vector3 euler(PI / 2 + 1, PI / 2 + 2, PI / 2 + 3);
        Quaternion q = Quaternion::FromEuler(euler.x, euler.y, euler.z);
        Vector3 vecToRotate(0, 1, 0);
        v = q * vecToRotate;
        std::cout << "euler=" << euler.x << "," << euler.y << "," << euler.z << std::endl;
        std::cout << "vecToRotate=" << vecToRotate.x << "," << vecToRotate.y << "," << vecToRotate.z << std::endl;
        std::cout << "result1=" << v << std::endl;
        Matrix4x4 matrix(Vector3(1, 1, 1), euler);
        v = matrix * vecToRotate;
        std::cout << "matrix=" << matrix << std::endl;
        std::cout << "result2=" << v << std::endl;

        Matrix3x3 m3(Vector3(12, 5, 39), Vector3(15, 89, 124), Vector3(2, 3, 8));
        Matrix3x3 m3inv=m3.Inverse();
        std::cout << m3inv*m3;

        Matrix4x4 m4(Vector4(12,3,5,7),Vector4(11,5,1,-7),Vector4(-12,0,0,7),Vector4(850,3,-5,7));
        Matrix4x4 m4inv=m4.Inverse();
        std::cout<<m4inv*m4;

        std::cout << "Math Testbench Finished" << std::endl;
    }
}