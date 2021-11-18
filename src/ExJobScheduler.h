#ifndef __EXJOB_THREAD_H__
#define __EXJOB_THREAD_H__

#include<vector>
#include<thread>
#include<iostream>

#define MAX_THREADS 8

namespace ExRenderer
{
    struct JobData
    {
        virtual void Run()=0;
    };

    struct JobThread
    {
        std::thread mThread;
        JobData *jobData;
        bool running;

        void Start();
        void Stop();
        void DoJob();
    };

    class JobScheduler
    {
        std::vector<JobData*> m_jobs;
        JobThread jobThreads[MAX_THREADS];
    public:
        JobScheduler()
        {
            for(int i=0;i<MAX_THREADS;++i)
            {
                jobThreads[i].jobData=nullptr;
                jobThreads[i].running=false;
            }
        }

        
        public:
        void PushJob(JobData*);
        void Schedule();
    };
}

#endif