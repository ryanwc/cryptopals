#ifndef THREADING_POOL_H__
#define THREADING_POOL_H__

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


namespace CustomThreading
{
    /*
     * Pool for distributing work to worker threads, then retrieiving
     * results from those worker threads after all work is completed.
     * Abstracts away threading details (e.g. syncronization on work queue), 
     * client just needs to use public API.
     */
    template<typename ResultType, typename... ResultsFuncArgs>
	class ResultsPool {
		public:

            /* 
             * Enumerate possible states of a ResultPool
             */
            enum Status { 
                STOPPED = 0,  // none of the workers are active.
                STARTING,  // workers are coming online and beginning to process submitted work, if any.
                WAITING,  // all workers are online, but the work queue is empty. submitting work will cause transition to "WORKING".
                WORKING,  // all workers are online, and at least one is working.
                STOPPING  // workers are being deactivated.
            };

			typedef void ResultsFunc(ResultsFuncArgs...);

			ResultsPool(int numThreads);
			~ResultsPool();

            /*
             * Get the number of workers this pool supports. Depending on pool status 0 or more of 
             * those workers may be active or currently doing work.
             */
			int GetNumWorkers();

            /*
             * Get the pool's status
             */
            Status GetStatus();

            /*
             * Gets the number of 'jobs' in the work queue (i.e. have not yet be picked up by a worker). 
             */
            int GetSizeOfWorkQueue();

            /*
             * Submit some work to the pool. It will be picked up by any available workers.
             */
            void SubmitWork(ResultsFunc*);

            /*
             * Blocks the caller until all previously submitted work has been completed, 
             * then returns a list of the results.
             */
            std::vector<ResultType> WaitForAllResults();

            /*
             * Retrieves all results from already-completed work.
             * The pool will 'forget' the retrieved results, so the caller should save them if desired.
             */
            std::vector<ResultType> GetAvailableResults();

		private:

			static void _endWorker(void* data);
            void _startWorkers();
            void _workerLoop(int threadNum);

            Status _status;
			int _numThreads;
            // TODO: cache alignment helpful for any of these?
			std::unique_ptr<std::thread[]> _workers;
            std::unique_ptr<std::vector<ResultType>> _workerResults;
            std::unique_ptr<std::mutex> _workerResultsMutexes;
            std::queue<std::tuple<ResultsFunc*, void*>> _workQueue;
            std::vector<ResultType> _results;
			std::condition_variable _addedWorkCondition;
			std::mutex _workQueueMutex;
            std::mutex _resultsMutex;
	};
}

#endif // THREADING_POOL
