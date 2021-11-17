#ifndef __EXMATH_H__
#define __EXMATH_H__

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
namespace ExRenderer
{
    using number_t = float;
    struct Vector2
    {
        number_t x, y;

        Vector2() : Vector2(0, 0) {}
        Vector2(number_t x, number_t y) : x(x), y(y) {}

        number_t Magnitude() const
        {
            return sqrtf(x * x + y * y);
        }

        Vector2 Normalize() const
        {
            number_t m = sqrtf(x * x + y * y);
            return Vector2(x / m, y / m);
        }

        Vector2 operator+(const Vector2 &other) const
        {
            Vector2 result;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            return result;
        }

        Vector2 operator-(const Vector2 &other) const
        {
            Vector2 result;
            result.x = this->x - other.x;
            result.y = this->y - other.y;
            return result;
        }

        Vector2 operator*(const number_t ratio) const
        {
            Vector2 result;
            result.x = this->x * ratio;
            result.y = this->y * ratio;
            return result;
        }

        Vector2 operator/(const number_t ratio) const
        {
            Vector2 result;
            result.x = this->x / ratio;
            result.y = this->y / ratio;
            return result;
        }

        Vector2 operator-() const
        {
            Vector2 result;
            result.x = -this->x;
            result.y = -this->y;
            return result;
        }

        number_t operator*(const Vector2 &other) const
        {
            return this->x * other.x + this->y * other.y;
        }

        static Vector2 zero()
        {
            return Vector2(0, 0);
        }

        static Vector2 right()
        {
            return Vector2(1, 0);
        }

        static Vector2 up()
        {
            return Vector2(0, 1);
        }
    };
    struct Vector3
    {
        number_t x, y, z;

        Vector3() : Vector3(0, 0, 0) {}
        Vector3(number_t x, number_t y, number_t z) : x(x), y(y), z(z) {}

        number_t Magnitude() const
        {
            return sqrtf(x * x + y * y + z * z);
        }

        Vector3 Normalize() const
        {
            number_t m = sqrtf(x * x + y * y + z * z);
            return Vector3(x / m, y / m, z / m);
        }

        Vector3 operator+(const Vector3 &other) const
        {
            Vector3 result;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            result.z = this->z + other.z;
            return result;
        }

        Vector3 operator-(const Vector3 &other) const
        {
            Vector3 result;
            result.x = this->x - other.x;
            result.y = this->y - other.y;
            result.z = this->z - other.z;
            return result;
        }

        Vector3 operator*(const number_t ratio) const
        {
            Vector3 result;
            result.x = this->x * ratio;
            result.y = this->y * ratio;
            result.z = this->z * ratio;
            return result;
        }

        Vector3 operator/(const number_t ratio) const
        {
            Vector3 result;
            result.x = this->x / ratio;
            result.y = this->y / ratio;
            result.z = this->z / ratio;
            return result;
        }

        Vector3 operator-() const
        {
            Vector3 result;
            result.x = -this->x;
            result.y = -this->y;
            result.z = -this->z;
            return result;
        }

        number_t operator*(const Vector3 &other) const
        {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }

        static Vector3 zero()
        {
            return Vector3(0, 0, 0);
        }

        static Vector3 right()
        {
            return Vector3(1, 0, 0);
        }

        static Vector3 up()
        {
            return Vector3(0, 1, 0);
        }

        static Vector3 forward()
        {
            return Vector3(0, 0, 1);
        }
    };

    inline Vector3 Cross(const Vector3 &v1, const Vector3 &v2)
    {
        return Vector3(v1.y * v2.z - v2.y * v1.z,
                       v1.z * v2.x - v2.z * v1.x,
                       v1.x * v2.y - v2.x * v1.y);
    }

    struct Vector4
    {
        number_t x, y, z, w;
        Vector4() : Vector4(0, 0, 0, 0) {}
        Vector4(number_t x, number_t y, number_t z, number_t w) : x(x), y(y), z(z), w(w) {}

        Vector4(const Vector3 &fromVec)
        {
            x = fromVec.x;
            y = fromVec.y;
            z = fromVec.z;
            w = 1;
        }

        number_t Magnitude() const
        {
            return sqrtf(x * x + y * y + z * z + w * w);
        }

        Vector4 Normalize() const
        {
            number_t m = sqrtf(x * x + y * y + z * z + w * w);
            return Vector4(x / m, y / m, z / m, w / m);
        }

        Vector4 operator+(const Vector4 &other) const
        {
            Vector4 result;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            result.z = this->z + other.z;
            result.w = this->w + other.w;
            return result;
        }

        Vector4 operator-(const Vector4 &other) const
        {
            Vector4 result;
            result.x = this->x - other.x;
            result.y = this->y - other.y;
            result.z = this->z - other.z;
            result.w = this->w - other.w;
            return result;
        }

        Vector4 operator*(const number_t ratio) const
        {
            Vector4 result;
            result.x = this->x * ratio;
            result.y = this->y * ratio;
            result.z = this->z * ratio;
            result.w = this->w * ratio;
            return result;
        }

        Vector4 operator/(const number_t ratio) const
        {
            Vector4 result;
            result.x = this->x / ratio;
            result.y = this->y / ratio;
            result.z = this->z / ratio;
            result.w = this->w / ratio;
            return result;
        }

        Vector4 operator-() const
        {
            Vector4 result;
            result.x = -this->x;
            result.y = -this->y;
            result.z = -this->z;
            result.w = -this->w;
            return result;
        }

        number_t operator*(const Vector4 &other) const
        {
            return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
        }
    };

    struct Quaternion
    {
        Vector3 v;
        number_t w;
        Quaternion() : Quaternion(0, 0, 0, 1) {}
        Quaternion(number_t x, number_t y, number_t z, number_t w) : v(x, y, x), w(w) {}
        Quaternion(const Vector3 &v, number_t w) : v(v), w(w) {}

        static Quaternion FromEuler(number_t x, number_t y, number_t z)
        {
            Quaternion result;
            Quaternion yRotate = Quaternion(Vector3(0, 1, 0) * sinf(y / 2), cosf(y / 2));
            Vector3 xVec = yRotate * Vector3(1, 0, 0);
            Quaternion xRotate = Quaternion(xVec * sinf(x / 2), cosf(x / 2));
            Vector3 zVec = xRotate * (yRotate * Vector3(0, 0, 1));
            Quaternion zRotate = Quaternion(zVec * sinf(z / 2), cosf(z / 2));

            return zRotate * xRotate * yRotate;
        }

        static Quaternion FromEuler(const Vector3 &v)
        {
            return FromEuler(v.x, v.y, v.z);
        }

        // Vector3 ToEuler()
        // {

        // }

        number_t Magnitude() const
        {
            number_t vm = v.Magnitude();
            return sqrtf(vm * vm + w * w);
        }

        Quaternion Conjugate() const
        {
            return Quaternion(-v, w);
        }

        Quaternion Inverse() const
        {
            return Conjugate() / Magnitude();
        }

        Quaternion operator*(const Quaternion &other) const
        {
            Quaternion result;
            result.w = w * other.w - v * other.v;
            result.v = other.v * w + v * other.w + Cross(other.v, v);
            return result;
        }

        Quaternion operator*(number_t ratio) const
        {
            Quaternion result;
            result.w = w * ratio;
            result.v = v * ratio;
            return result;
        }

        Quaternion operator/(number_t ratio) const
        {
            Quaternion result;
            result.w = w / ratio;
            result.v = v / ratio;
            return result;
        }

        Vector3 operator*(const Vector3 &vec) const
        {
            Quaternion p = Quaternion(vec, 0);
            p = (*this) * p * Inverse();
            return p.v;
        }
    };
    struct Matrix3x3
    {
        number_t data[9];

        Matrix3x3()
        {
            for (int i = 0; i < 9; ++i)
            {
                data[i] = 0;
            }
        }

        Matrix3x3(const Vector3 &col1, const Vector3 &col2, const Vector3 &col3)
        {
            data[0] = col1.x;
            data[1] = col2.x;
            data[2] = col3.x;
            data[3] = col1.y;
            data[4] = col2.y;
            data[5] = col3.y;
            data[6] = col1.z;
            data[7] = col2.z;
            data[8] = col3.z;
        }

        Matrix3x3 Transform() const
        {
            Matrix3x3 result;
            result.data[0] = data[0];
            result.data[1] = data[3];
            result.data[2] = data[6];
            result.data[3] = data[1];
            result.data[4] = data[4];
            result.data[5] = data[7];
            result.data[6] = data[2];
            result.data[7] = data[5];
            result.data[8] = data[8];

            return result;
        }

        void RowOpMul(Matrix3x3 &l, Matrix3x3 &r, uint32_t rowIndex, number_t mulValue) const
        {
            if (mulValue != 0)
            {
                l.data[rowIndex * 3] *= mulValue;
                l.data[rowIndex * 3 + 1] *= mulValue;
                l.data[rowIndex * 3 + 2] *= mulValue;

                r.data[rowIndex * 3] *= mulValue;
                r.data[rowIndex * 3 + 1] *= mulValue;
                r.data[rowIndex * 3 + 2] *= mulValue;
            }
        }

        void RowOpAdd(Matrix3x3 &l, Matrix3x3 &r, uint32_t dstRow, uint32_t srcRow, float ratio = 1) const
        {
            l.data[dstRow * 3] += l.data[srcRow * 3] * ratio;
            l.data[dstRow * 3 + 1] += l.data[srcRow * 3 + 1] * ratio;
            l.data[dstRow * 3 + 2] += l.data[srcRow * 3 + 2] * ratio;

            r.data[dstRow * 3] += r.data[srcRow * 3] * ratio;
            r.data[dstRow * 3 + 1] += r.data[srcRow * 3 + 1] * ratio;
            r.data[dstRow * 3 + 2] += r.data[srcRow * 3 + 2] * ratio;
        }

        void RowOpSub(Matrix3x3 &l, Matrix3x3 &r, uint32_t dstRow, uint32_t srcRow, float ratio = 1) const
        {
            l.data[dstRow * 3] -= l.data[srcRow * 3] * ratio;
            l.data[dstRow * 3 + 1] -= l.data[srcRow * 3 + 1] * ratio;
            l.data[dstRow * 3 + 2] -= l.data[srcRow * 3 + 2] * ratio;

            r.data[dstRow * 3] -= r.data[srcRow * 3] * ratio;
            r.data[dstRow * 3 + 1] -= r.data[srcRow * 3 + 1] * ratio;
            r.data[dstRow * 3 + 2] -= r.data[srcRow * 3 + 2] * ratio;
        }

        number_t Data(uint32_t row, uint32_t col) const
        {
            return data[row * 3 + col];
        }

        Matrix3x3 Inverse() const
        {
            Matrix3x3 l = *this;
            Matrix3x3 r = Matrix3x3::identity();

            for(int c=0;c<3;++c)
            {
                for(int i=c;i<3;++i)
                {
                    RowOpMul(l, r, i, l.Data(i, c));
                }

                for(int i=c+1;i<3;++i)
                {
                    RowOpAdd(l, r, c, i);
                }

                for(int i=0;i<3;++i)
                {
                    if(i!=c)
                    {
                        RowOpSub(l, r, i, c, l.Data(i, c) / l.Data(c, c));
                    }
                }

                RowOpMul(l, r, c, 1 / l.Data(c, c));
            }

            return r;
        }

        Matrix3x3 operator+(const Matrix3x3 &other) const
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] + other.data[i];
            }

            return result;
        }
        Matrix3x3 operator-(const Matrix3x3 &other) const
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] - other.data[i];
            }

            return result;
        }
        Matrix3x3 operator-() const
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = -this->data[i];
            }

            return result;
        }
        Matrix3x3 operator*(number_t ratio) const
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] * ratio;
            }

            return result;
        }
        Matrix3x3 operator/(number_t ratio) const
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] / ratio;
            }

            return result;
        }
        Matrix3x3 operator*(const Matrix3x3 &other) const
        {
            Matrix3x3 result;

            for (int i = 0; i < 9; ++i)
            {
                int row = i / 3 * 3, col = i % 3;
                result.data[i] = data[row] * other.data[col] +
                                 data[row + 1] * other.data[col + 3] +
                                 data[row + 2] * other.data[col + 6];
            }

            return result;
        }
        Vector3 operator*(const Vector3 &vec) const
        {
            Vector3 result;

            result.x = vec.x * data[0] + vec.y * data[1] + vec.z * data[2];
            result.y = vec.x * data[3] + vec.y * data[4] + vec.z * data[5];
            result.z = vec.x * data[6] + vec.y * data[7] + vec.z * data[8];

            return result;
        }

        static Matrix3x3 identity()
        {
            return Matrix3x3(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));
        }
    };

    struct Matrix4x4
    {
        number_t data[16];

        Matrix4x4()
        {
            for (int i = 0; i < 16; ++i)
            {
                data[i] = 0;
            }
        }

        Matrix4x4(const Vector4 &col1, const Vector4 &col2, const Vector4 &col3, const Vector4 &col4)
        {
            data[0] = col1.x;
            data[1] = col2.x;
            data[2] = col3.x;
            data[3] = col4.x;
            data[4] = col1.y;
            data[5] = col2.y;
            data[6] = col3.y;
            data[7] = col4.y;
            data[8] = col1.z;
            data[9] = col2.z;
            data[10] = col3.z;
            data[11] = col4.z;
            data[12] = col1.w;
            data[13] = col2.w;
            data[14] = col3.w;
            data[15] = col4.w;
        }

        Matrix4x4 Transform() const
        {
            Matrix4x4 result;
            for(int i=0;i<16;++i)
            {
                result.data[i] = data[i%4*4+i/4];
            }

            return result;
        }

        void RowOpMul(Matrix4x4 &l, Matrix4x4 &r, uint32_t rowIndex, number_t mulValue) const
        {
            if (mulValue != 0)
            {
                l.data[rowIndex * 4] *= mulValue;
                l.data[rowIndex * 4 + 1] *= mulValue;
                l.data[rowIndex * 4 + 2] *= mulValue;
                l.data[rowIndex * 4 + 3] *= mulValue;

                r.data[rowIndex * 4] *= mulValue;
                r.data[rowIndex * 4 + 1] *= mulValue;
                r.data[rowIndex * 4 + 2] *= mulValue;
                r.data[rowIndex * 4 + 3] *= mulValue;
            }
        }

        void RowOpAdd(Matrix4x4 &l, Matrix4x4 &r, uint32_t dstRow, uint32_t srcRow, float ratio = 1) const
        {
            l.data[dstRow * 4] += l.data[srcRow * 4] * ratio;
            l.data[dstRow * 4 + 1] += l.data[srcRow * 4 + 1] * ratio;
            l.data[dstRow * 4 + 2] += l.data[srcRow * 4 + 2] * ratio;
            l.data[dstRow * 4 + 3] += l.data[srcRow * 4 + 3] * ratio;

            r.data[dstRow * 4] += r.data[srcRow * 4] * ratio;
            r.data[dstRow * 4 + 1] += r.data[srcRow * 4 + 1] * ratio;
            r.data[dstRow * 4 + 2] += r.data[srcRow * 4 + 2] * ratio;
            r.data[dstRow * 4 + 3] += r.data[srcRow * 4 + 3] * ratio;
        }

        void RowOpSub(Matrix4x4 &l, Matrix4x4 &r, uint32_t dstRow, uint32_t srcRow, float ratio = 1) const
        {
            l.data[dstRow * 4] -= l.data[srcRow * 4] * ratio;
            l.data[dstRow * 4 + 1] -= l.data[srcRow * 4 + 1] * ratio;
            l.data[dstRow * 4 + 2] -= l.data[srcRow * 4 + 2] * ratio;
            l.data[dstRow * 4 + 3] -= l.data[srcRow * 4 + 3] * ratio;

            r.data[dstRow * 4] -= r.data[srcRow * 4] * ratio;
            r.data[dstRow * 4 + 1] -= r.data[srcRow * 4 + 1] * ratio;
            r.data[dstRow * 4 + 2] -= r.data[srcRow * 4 + 2] * ratio;
            r.data[dstRow * 4 + 3] -= r.data[srcRow * 4 + 3] * ratio;
        }

        number_t Data(uint32_t row, uint32_t col) const
        {
            return data[row * 4 + col];
        }

        Matrix4x4 Inverse() const
        {
            Matrix4x4 l = *this;
            Matrix4x4 r = Matrix4x4::identity();

            for(int c=0;c<4;++c)
            {
                for(int i=c;i<4;++i)
                {
                    RowOpMul(l, r, i, l.Data(i, c));
                }

                for(int i=c+1;i<4;++i)
                {
                    RowOpAdd(l, r, c, i);
                }

                for(int i=0;i<4;++i)
                {
                    if(i!=c)
                    {
                        RowOpSub(l, r, i, c, l.Data(i, c) / l.Data(c, c));
                    }
                }

                RowOpMul(l, r, c, 1 / l.Data(c, c));
            }

            return r;
        }

        Matrix4x4(const Vector3 &position, const Quaternion &rotation) 
        {
            Vector4 col1 = Vector4(rotation * Vector3(1, 0, 0));
            Vector4 col2 = Vector4(rotation * Vector3(0, 1, 0));
            Vector4 col3 = Vector4(rotation * Vector3(0, 0, 1));
            col1.w = col2.w = col3.w = 0;
            Vector4 col4 = Vector4(position);
            new (this) Matrix4x4(col1, col2, col3, col4);
        }

        Matrix4x4(const Vector3 &position, const Vector3 &euler)
        {
            Quaternion rotation = Quaternion::FromEuler(euler);
            Vector4 col1 = Vector4(rotation * Vector3(1, 0, 0));
            Vector4 col2 = Vector4(rotation * Vector3(0, 1, 0));
            Vector4 col3 = Vector4(rotation * Vector3(0, 0, 1));
            col1.w = col2.w = col3.w = 0;
            Vector4 col4 = Vector4(position);
            new (this) Matrix4x4(col1, col2, col3, col4);
        }

        Matrix4x4 operator+(const Matrix4x4 &other) const
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] + other.data[i];
            }

            return result;
        }
        Matrix4x4 operator-(const Matrix4x4 &other) const
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] - other.data[i];
            }

            return result;
        }
        Matrix4x4 operator-() const
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = -this->data[i];
            }

            return result;
        }
        Matrix4x4 operator*(number_t ratio) const
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] * ratio;
            }

            return result;
        }
        Matrix4x4 operator/(number_t ratio) const
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] / ratio;
            }

            return result;
        }
        Matrix4x4 operator*(const Matrix4x4 &other) const
        {
            Matrix4x4 result;

            for (int i = 0; i < 16; ++i)
            {
                int row = i / 4 * 4, col = i % 4;
                result.data[i] = data[row] * other.data[col] +
                                 data[row + 1] * other.data[col + 4] +
                                 data[row + 2] * other.data[col + 8] +
                                 data[row + 3] * other.data[col + 12];
            }

            return result;
        }
        Vector4 operator*(const Vector4 &vec) const
        {
            Vector4 result;

            result.x = vec.x * data[0] + vec.y * data[1] + vec.z * data[2] + vec.w * data[3];
            result.y = vec.x * data[4] + vec.y * data[5] + vec.z * data[6] + vec.w * data[7];
            result.z = vec.x * data[8] + vec.y * data[9] + vec.z * data[10] + vec.w * data[11];
            result.w = vec.x * data[12] + vec.y * data[13] + vec.z * data[14] + vec.w * data[15];

            return result;
        }

        Vector3 operator*(const Vector3 &vec) const
        {
            Vector4 v(vec);
            v = (*this) * v;
            return Vector3(v.x, v.y, v.z)/v.w;
        }

        static Matrix4x4 identity()
        {
            return Matrix4x4(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(0, 0, 0, 1));
        }
    };


    std::ostream &operator<<(std::ostream &os, const Vector2 &vec);
    std::ostream &operator<<(std::ostream &os, const Vector3 &vec);
    std::ostream &operator<<(std::ostream &os, const Vector4 &vec);
    std::ostream &operator<<(std::ostream &os, const Quaternion &r);

    std::ostream &operator<<(std::ostream &os, const Matrix3x3 &matrix);
    std::ostream &operator<<(std::ostream &os, const Matrix4x4 &matrix);

    void MathTestBench();
}
#endif
