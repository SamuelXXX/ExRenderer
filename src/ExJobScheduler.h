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

        void *jobDataBuffer;
        size_t jobDataBufferSize;
        size_t jobDataBufferCursor;

    public:
        template <class JobType>
        void PrepareScheduler(size_t maxJobCount)
        {
            size_t size=maxJobCount*sizeof(JobType);
            if(size>jobDataBufferSize)
            {
                if(jobDataBuffer!=nullptr)
                {
                    std::free(jobDataBuffer);
                }
                jobDataBuffer=std::malloc(size);
                jobDataBufferSize=size;
            }

            jobDataBufferCursor=0;
        }

        template <class T,typename ...Args>
        T *MakeJob(Args... args)
        {
            void *retPtr=(uint8_t *)jobDataBuffer+jobDataBufferCursor;
            jobDataBufferCursor+=sizeof(T);
            T *data=new (retPtr)T(args...);
            return data;
        }

    public:
        JobScheduler()
        {
            std::cout << "Init JobScheduler" << std::endl;
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                jobThreads[i].index = i;
                jobThreads[i].jobData = nullptr;
                jobThreads[i].running = false;
                jobThreads[i].Start();
            }

            jobDataBuffer=nullptr;
            jobDataBufferSize=0;
        }

        JobScheduler(const JobScheduler&)=delete;
        JobScheduler(JobScheduler&&)=delete;
        JobScheduler& operator=(const JobScheduler&)=delete;
        JobScheduler& operator=(JobScheduler&&)=delete;

        ~JobScheduler()
        {
            std::cout << "Finish JobScheduler" << std::endl;
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                jobThreads[i].jobData = nullptr;
                jobThreads[i].running = false;
                jobThreads[i].Stop();
            }
            if(jobDataBuffer!=nullptr)
            {
                std::free(jobDataBuffer);
            }
        }

    public:
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

        template <class T,typename ...Args>
        void PushJob(Args... args)
        {
            T* data=MakeJob<T>(args...);
            PushJob(data);
        }

        JobData *PopJob()
        {
            return m_jobs[--jobSize];
        }

        void Schedule();
    };
}

#endif