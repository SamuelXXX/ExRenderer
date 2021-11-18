#include"ExJobScheduler.h"

namespace ExRenderer
{
    void JobThread::Start()
    {
        running=true;
        mThread=std::thread(&JobThread::DoJob,this);
    }

    void JobThread::Stop()
    {
        running=false;
        mThread.detach();
    }

    #pragma GCC optimize(2)
    void JobThread::DoJob()
    {
        while (running)
        {
            if(jobData!=nullptr)
            {
                //lock.lock();
                jobData->Run();
                jobData=nullptr;
                //lock.unlock();
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
                //jobThreads[i].lock.lock();
                if(jobThreads[i].jobData==nullptr&&jobSize!=0)
                {
                    jobThreads[i].jobData=PopJob();
                }
                //jobThreads[i].lock.unlock();
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