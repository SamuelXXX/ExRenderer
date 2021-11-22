#include"ExJobScheduler.h"

namespace ExRenderer
{
    std::mutex mtx[MAX_THREADS];

    void JobThread::Wait()
    {
        mtx[index].lock();
    }

    void JobThread::Notify()
    {
        if(!mtx[index].try_lock())
        {
            mtx[index].unlock();
        }
        
    }

    void JobThread::Start()
    {
        running=true;
        mThread=std::thread(&JobThread::DoJob,this);
    }

    void JobThread::Stop()
    {
        running=false;
        Notify();
        mThread.join();
    }

    #pragma GCC optimize(2)
    void JobThread::DoJob()
    {
        while (running)
        {
            Wait();
            if(jobData!=nullptr)
            {
                jobData->Run();
                jobData=nullptr;
            }
        }
        
    }

    #pragma GCC optimize(2)
    void JobScheduler::Schedule()
    {
        while (jobSize!=0)
        {
            for(int i=0;i<MAX_THREADS;++i)
            {
                if(jobThreads[i].jobData==nullptr&&jobSize!=0)
                {
                    jobThreads[i].jobData=PopJob();
                    jobThreads[i].Notify();
                } 
            }
        }

        int active=MAX_THREADS;
        while (active)
        {
            active=0;
            for(int i=0;i<MAX_THREADS;++i)
            {
                if(jobThreads[i].jobData!=nullptr)
                {
                    ++active;
                }
            }
        }        
        
    }
}