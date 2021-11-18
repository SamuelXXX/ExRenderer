#include "ExRendererTestbench.h"

using namespace ExRenderer::Testbench;

struct DemoJob:public ExRenderer::JobData
{
    int index;
    DemoJob(int i):index(i){}
    void Run() override
    {
        std::cout<<index<<std::endl;
        for(int i=1;i<1000000000;i++);
    }
};

int main(int argc, char *args[])
{
    Basic::Test();
    ExRenderer::JobScheduler jobScheduler;
    jobScheduler.StartThreads();
    for(int i=0;i<200;i++)
    {

        auto a= new DemoJob(i);
        a->Run();
        // jobScheduler.PushJob(new DemoJob(i));
    }
    
    std::cout<<"Start Jobs"<<std::endl;
    jobScheduler.Schedule();
    std::cout<<"Finish Jobs"<<std::endl;

    for(int i=0;i<2;i++)
    {
        jobScheduler.PushJob(new DemoJob(i));
    }
    
    std::cout<<"Start Jobs"<<std::endl;
    jobScheduler.Schedule();
    std::cout<<"Finish Jobs"<<std::endl;

    while (true)
    {
        /* code */
    }
    
    return 0;
}
