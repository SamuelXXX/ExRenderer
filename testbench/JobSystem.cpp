#include"ExRendererTestbench.h"

namespace ExRenderer::Testbench::JobSystem
{
    struct DemoJob:public JobData
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
        JobScheduler jobScheduler;

        jobScheduler.PrepareScheduler(200*sizeof(DemoJob));
        for(int i=0;i<100;++i)
        {
            jobScheduler.PushJob<DemoJob>(i);
        }
        jobScheduler.Schedule();
    }
}