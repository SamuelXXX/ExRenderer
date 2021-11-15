#ifndef __EXMATH_H__
#define __EXMATH_H__

#include <cmath>

namespace ExRenderer
{
    struct Vector2
    {
        float x, y;

        Vector2() : Vector2(0, 0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        float Magnitude()
        {
            return sqrtf(x * x + y * y);
        }

        Vector2 Normalize()
        {
            float m = sqrtf(x * x + y * y);
            return Vector2(x / m, y / m);
        }

        Vector2 operator+(const Vector2 &other)
        {
            Vector2 result;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            return result;
        }

        Vector2 operator-(const Vector2 &other)
        {
            Vector2 result;
            result.x = this->x - other.x;
            result.y = this->y - other.y;
            return result;
        }

        Vector2 operator*(const float ratio)
        {
            Vector2 result;
            result.x = this->x * ratio;
            result.y = this->y * ratio;
            return result;
        }

        Vector2 operator/(const float ratio)
        {
            Vector2 result;
            result.x = this->x / ratio;
            result.y = this->y / ratio;
            return result;
        }

        Vector2 operator-()
        {
            Vector2 result;
            result.x = -this->x;
            result.y = -this->y;
            return result;
        }

        float operator*(const Vector2 &other)
        {
            return this->x * other.x + this->y * other.y;
        }
    };
    struct Vector3
    {
        float x, y, z;

        Vector3() : Vector3(0, 0, 0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        float Magnitude()
        {
            return sqrtf(x * x + y * y + z * z);
        }

        Vector3 Normalize()
        {
            float m = sqrtf(x * x + y * y + z * z);
            return Vector3(x / m, y / m, z / m);
        }

        Vector3 operator+(const Vector3 &other)
        {
            Vector3 result;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            result.z = this->z + other.z;
            return result;
        }

        Vector3 operator-(const Vector3 &other)
        {
            Vector3 result;
            result.x = this->x - other.x;
            result.y = this->y - other.y;
            result.z = this->z - other.z;
            return result;
        }

        Vector3 operator*(const float ratio)
        {
            Vector3 result;
            result.x = this->x * ratio;
            result.y = this->y * ratio;
            result.z = this->z * ratio;
            return result;
        }

        Vector3 operator/(const float ratio)
        {
            Vector3 result;
            result.x = this->x / ratio;
            result.y = this->y / ratio;
            result.z = this->z / ratio;
            return result;
        }

        Vector3 operator-()
        {
            Vector3 result;
            result.x = -this->x;
            result.y = -this->y;
            result.z = -this->z;
            return result;
        }

        float operator*(const Vector3 &other)
        {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }
    };
    struct Vector4
    {
        float x, y, z, w;
        Vector4() : Vector4(0, 0, 0, 0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        float Magnitude()
        {
            return sqrtf(x * x + y * y + z * z + w * w);
        }

        Vector4 Normalize()
        {
            float m = sqrtf(x * x + y * y + z * z + w * w);
            return Vector4(x / m, y / m, z / m, w / m);
        }

        Vector4 operator+(const Vector4 &other)
        {
            Vector4 result;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            result.z = this->z + other.z;
            result.w = this->w + other.w;
            return result;
        }

        Vector4 operator-(const Vector4 &other)
        {
            Vector4 result;
            result.x = this->x - other.x;
            result.y = this->y - other.y;
            result.z = this->z - other.z;
            result.w = this->w - other.w;
            return result;
        }

        Vector4 operator*(const float ratio)
        {
            Vector4 result;
            result.x = this->x * ratio;
            result.y = this->y * ratio;
            result.z = this->z * ratio;
            result.w = this->w * ratio;
            return result;
        }

        Vector4 operator/(const float ratio)
        {
            Vector4 result;
            result.x = this->x / ratio;
            result.y = this->y / ratio;
            result.z = this->z / ratio;
            result.w = this->w / ratio;
            return result;
        }

        Vector4 operator-()
        {
            Vector4 result;
            result.x = -this->x;
            result.y = -this->y;
            result.z = -this->z;
            result.w = -this->w;
            return result;
        }

        float operator*(const Vector4 &other)
        {
            return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
        }
    };

    struct Matrix3x3
    {
        float data[9];

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

        Matrix3x3 Transform()
        {
            Matrix3x3 result;
            result.data[0]=data[0];
            result.data[1]=data[3];
            result.data[2]=data[6];
            result.data[3]=data[1];
            result.data[4]=data[4];
            result.data[5]=data[7];
            result.data[6]=data[2];
            result.data[7]=data[5];
            result.data[8]=data[8];

            return result;
        }

        Matrix3x3 operator+(const Matrix3x3 &other)
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] + other.data[i];
            }

            return result;
        }
        Matrix3x3 operator-(const Matrix3x3 &other)
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] - other.data[i];
            }

            return result;
        }
        Matrix3x3 operator-()
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = -this->data[i];
            }

            return result;
        }
        Matrix3x3 operator*(float ratio)
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] * ratio;
            }

            return result;
        }
        Matrix3x3 operator/(float ratio)
        {
            Matrix3x3 result;
            for (int i = 0; i < 9; ++i)
            {
                result.data[i] = this->data[i] / ratio;
            }

            return result;
        }
        Matrix3x3 operator*(const Matrix3x3 &other)
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
        Vector3 operator*(const Vector3 &vec)
        {
            Vector3 result;

            result.x = vec.x * data[0] + vec.y * data[1] + vec.z * data[2];
            result.y = vec.x * data[3] + vec.y * data[4] + vec.z * data[5];
            result.z = vec.x * data[6] + vec.y * data[7] + vec.z * data[8];

            return result;
        }
    };

    struct Matrix4x4
    {
        float data[16];

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

        Matrix4x4 operator+(const Matrix4x4 &other)
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] + other.data[i];
            }

            return result;
        }
        Matrix4x4 operator-(const Matrix4x4 &other)
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] - other.data[i];
            }

            return result;
        }
        Matrix4x4 operator-()
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = -this->data[i];
            }

            return result;
        }
        Matrix4x4 operator*(float ratio)
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] * ratio;
            }

            return result;
        }
        Matrix4x4 operator/(float ratio)
        {
            Matrix4x4 result;
            for (int i = 0; i < 16; ++i)
            {
                result.data[i] = this->data[i] / ratio;
            }

            return result;
        }
        Matrix4x4 operator*(const Matrix4x4 &other)
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
        Vector4 operator*(const Vector4 &vec)
        {
            Vector4 result;

            result.x = vec.x * data[0] + vec.y * data[1] + vec.z * data[2] + vec.w * data[3];
            result.y = vec.x * data[4] + vec.y * data[5] + vec.z * data[6] + vec.w * data[7];
            result.z = vec.x * data[8] + vec.y * data[9] + vec.z * data[10] + vec.w * data[11];
            result.w = vec.x * data[12] + vec.y * data[13] + vec.z * data[14] + vec.w * data[15];

            return result;
        }
    };
}
#endif
