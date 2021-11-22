#include"ExRendererTestbench.h"

namespace ExRenderer::Testbench::JobSystem
{
    struct DemoJob:public ExRenderer::JobSystem::JobData
    {
        uint32_t index;

        DemoJob(uint32_t index):index(index){}

        void Run() override
        {
            std::cout<<index<<std::endl;
            for(int i=0;i<1000000000;i++);
        }
    };

    void Test()
    {
        ExRenderer::JobSystem::JobScheduler jobScheduler;

        jobScheduler.PrepareScheduler<DemoJob>(200);
        for(int i=0;i<100;++i)
        {
            jobScheduler.PushJob<DemoJob>(i);
        }
        jobScheduler.Schedule();
    }
}