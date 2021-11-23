#ifndef __EXRENDERER_TESTBENCH_H__
#define __EXRENDERER_TESTBENCH_H__

#include"../src/ExRenderer.h"

namespace ExRenderer::Testbench
{
    namespace Math
    {
        void Test();
    }

    namespace JobSystem
    {
        void Test();
    }

    namespace Basic
    {
        void Test();
    }

    namespace Diffuse
    {
        void Test();
    }

    namespace MultiLight
    {
        void Test();
    }
}
#endif