#ifndef __EXJOB_THREAD_H__
#define __EXJOB_THREAD_H__

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

#define MAX_THREADS 16
#define MAX_JOBS 16

namespace ExRenderer
{
    struct JobData
    {
        virtual void Run() = 0;
    };

    struct JobThread
    {
        int index;
        std::thread mThread;
        JobData *jobData;
        bool running;

        void Start();
        void Stop();
        void DoJob();

        void Wait();
        void Notify();
    };

    class JobScheduler
    {
    public:
        JobData *m_jobs[MAX_JOBS];
        size_t jobSize = 0;

        JobThread jobThreads[MAX_THREADS];

    public:
        JobScheduler()
        {
        }

    public:
        void StartThreads()
        {
            std::cout << "Init JobScheduler" << std::endl;
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                jobThreads[i].index = i;
                jobThreads[i].jobData = nullptr;
                jobThreads[i].running = false;
                jobThreads[i].Start();
            }
        }

        void StopThreads()
        {
            std::cout << "Finish JobScheduler" << std::endl;
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                jobThreads[i].jobData = nullptr;
                jobThreads[i].running = false;
                jobThreads[i].Stop();
            }
        }
        void PushJob(JobData *data)
        {
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                if (jobThreads[i].jobData == nullptr)
                {
                    jobThreads[i].jobData = data;
                    jobThreads[i].Notify();
                    return;
                }
            }
            m_jobs[jobSize++] = data;
            if (jobSize == MAX_JOBS)
            {
                Schedule();
            }
        }
        JobData *PopJob()
        {
            return m_jobs[--jobSize];
        }
        void Schedule();
        void NotifyAll();
    };
}

#endif