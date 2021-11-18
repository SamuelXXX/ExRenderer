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
    }

    void JobThread::DoJob()
    {
        while (running)
        {
            if(jobData!=nullptr)
            {
                jobData->Run();
                delete jobData;
                jobData=nullptr;
            }
        }
        
    }

    void JobScheduler::PushJob(JobData *data)
    {
        m_jobs.push_back(data);
    }

    void JobScheduler::Schedule()
    {
        for(int i=0;i<MAX_THREADS;++i)
        {
            jobThreads[i].jobData=nullptr;
            jobThreads[i].running=false;
            jobThreads[i].Start();
        }

        while (m_jobs.size()!=0)
        {
            for(int i=0;i<MAX_THREADS;++i)
            {
                if(jobThreads[i].jobData==nullptr&&m_jobs.size()!=0)
                {
                    jobThreads[i].jobData=m_jobs[m_jobs.size()-1];
                    m_jobs.pop_back();
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

        for(int i=0;i<MAX_THREADS;++i)
        {
            jobThreads[i].jobData=nullptr;
            jobThreads[i].running=false;
            jobThreads[i].Stop();
        }
        
        
    }
}