#ifndef __EXLIGHT_H__
#define __EXLIGHT_H__

#include <vector>
#include "ExMath.h"

namespace ExRenderer
{
    class Light
    {
    public:
        virtual Vector3 GetDirection(const Vector3 &) const = 0;
        virtual Vector4 GetColor(const Vector3 &) const = 0;
    };

    class DirectionalLight : public Light
    {
        Vector3 direction;
        Vector4 color;

    public:
        DirectionalLight(const Vector3 &direction, const Vector4 &color) : direction(direction), color(color) {}

    public:
        Vector3 GetDirection(const Vector3 &worldPos) const override
        {
            return direction;
        }

        Vector4 GetColor(const Vector3 &worldPos) const override
        {
            return color;
        }
    };

    class PointLight : public Light
    {
        Vector3 position;
        Vector4 color;

    public:
        PointLight(const Vector3 &position, const Vector4 &color) : position(position), color(color) {}

    public:
        Vector3 GetDirection(const Vector3 &worldPos) const override
        {
            return worldPos - position;
        }

        Vector4 GetColor(const Vector3 &worldPos) const override
        {
            Vector3 dir = worldPos - position;
            float intensity = 1 / (dir * dir);
            return color * intensity;
        }
    };

    class LightPackManager
    {
        std::vector<Light *> m_lights;

    public:
        LightPackManager() = default;
        ~LightPackManager()
        {
            this->ClearLights();
        }

        LightPackManager(const LightPackManager &) = delete;
        LightPackManager(LightPackManager &&) = delete;
        LightPackManager &operator=(const LightPackManager &) = delete;
        LightPackManager &operator=(LightPackManager &&) = delete;

    public:
        template <class LT, typename... Args>
        void AddLight(Args... args)
        {
            LT *lightPtr = new LT(args...);
            m_lights.push_back(lightPtr);
        }
        void ClearLights()
        {
            for (auto lightPtr : m_lights)
            {
                delete lightPtr;
            }
            m_lights.clear();
        }
        size_t Size()
        {
            return m_lights.size();
        }

    public:
        std::vector<Light *>::iterator begin()
        {
            return m_lights.begin();
        }

        std::vector<Light *>::iterator end()
        {
            return m_lights.end();
        }
    };
}

#endif