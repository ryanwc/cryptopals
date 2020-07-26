#ifndef THREADING_POOL_H__
#define THREADING_POOL_H__

#include <tuple>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>



namespace CustomThreading
{

    // use partial template specialization to "unwrap" a function signature (e.g. bool(int, int))
    // https://stackoverflow.com/questions/27604128/c-stdfunction-like-template-syntax
    template<typename ResultsFuncSig>
    class ResultsPool {};  // TODO: raise not-implemented error in constructor?

    /*
     * Pool for distributing work to worker threads, then retrieiving results from those worker threads.
     * Abstracts away threading details (e.g. sync on work queue), client just needs to use public API.
     * Templated by the return type and arg types of the results func passed as a poniter to the
     * constructor. Results func is the func that will be executed by pool workers on work submission.
     * Must use std::function-style templating, e.g. to use a results function that takes an int, a bool, 
     * and returns a float, would template like ResultsPool<float(int, bool)>. 
     * So, can use with decltype() since that is what is returned for functions.
     * Caveat 1: using decltype(myFunc) will not compile if myFunc is overloaded.
     *   but you can still provide the types in code without using decltype.
     * Caveat 2: does not work (or at least is not tested yet) with member functions.
     * Example:
     *   float foo(int one, bool two) { return 2.0; };
     *   ResultsPool<decltype(foo)> fooResultsPool(foo);
     *   // alternately: ResultsPool<float(int,bool)> fooResultsPool(foo);
     *   fooResultsPool.SubmitWork(1, true);
     *   fooResultsPool.SubmitWOrk(2, false);
     *   auto fooResults = fooResultsPool.WaitForAllResults();  // len 2 vector of float, each entry == 2.0
     */
    template<typename RetT, typename ...ArgTs>
	class ResultsPool<RetT(ArgTs...)> {
		public:

            // just a convenient name for func pointer
            typedef RetT(*PoolResultsFuncPtr)(ArgTs...);

            // the func used to produce results by workers in this pool
            PoolResultsFuncPtr ResultsFuncPtr;

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

            /*
             * Construct a new ResultsPool with numThreads workers executing func pointed to by resultsFuncPtr
             * when work is subitted.
             */
			ResultsPool(int numThreads, PoolResultsFuncPtr resultsFuncPtr);
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
            void SubmitWork(ArgTs...);  // TODO: bind the args to the func provided in constructor

            /*
             * Blocks the caller until all previously submitted work has been completed, 
             * then returns a list of the results.
             */
            std::vector<RetT> WaitForAllResults();

            /*
             * Retrieves all results from already-completed work.
             * The pool will 'forget' the retrieved results, so the caller should save them if desired.
             */
            std::vector<RetT> GetAvailableResults();

		private:

            // kill a worker currently alive in the pool
			static void _endWorker();
            // start all workers in the pool
            void _startWorkers();
            // the entry point for pool worker threadss
            void _workerLoop(int threadNum);

            // status of this pool
            Status _status;
            // number of max worker threads in this pool
			int _numThreads;
            // TODO: cache alignment helpful for any of these? probably _workerResults?
            // keep track of the workers in the pool
			std::unique_ptr<std::thread[]> _workers;
            // each worker has its own results list so do not have to syncronize so often
            std::unique_ptr< std::vector<RetT> > _workerResults;
            // need a mutex so master thread can safely access each worker result list
            std::unique_ptr<std::mutex> _workerResultsMutexes;
            // the queue of work workers pull work from
            std::queue< std::tuple<ArgTs...> > _workQueue;
            // the signal from master to workers that work has been added to the queue
			std::condition_variable _addedWorkCondition;
            // need to sync on the work queue
			std::mutex _workQueueMutex;
	};
}

#endif // THREADING_POOL
