#ifndef RESULTS_POOL_H__
#define RESULTS_POOL_H__

#include <tuple>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>


namespace CustomThreading
{

    // use partial template specialization to "unwrap" a function signature (e.g. bool(int, int))
    // https://stackoverflow.com/questions/27604128/c-stdfunction-like-template-syntax
    template<typename ResultsFuncSig>
    class ResultsPool {};  // TODO: raise not-implemented error somewhere inside?

    // appears you cant do partial template class specialization in .cpp separate from .h
    // another option for this implementation is not to use partial specialization
    // and just do template<typename RetT, typename ...ArgsT> class ResultsPool.
    // downside is that prohibits clients from using decltype like ResultsPool<decltype(clientFunc)>
    // 
    // also note, an 'easier' design might be to force the results func to take a single arg -- void pointer -- 
    // and return a single arg -- void pointer. but that is more C-like, and forces client to work with void pointers.
    /*
     * Pool for distributing work to worker threads, then retrieiving results from those worker threads.
     * Abstracts away threading details (e.g. sync on work queue), client just needs to use public API.
     * Templated by the return type and arg types of the results func pointer passed to the constructor. 
     * Results func is the func that will be executed by pool workers on work submission.
     * Must use std::function-style with templating, e.g. to use a results function myFunc that takes an int and a bool 
     * and returns a float, would template like ResultsPool<float(int, bool)> or ResultsPool<decltype(myFunc)>. 
     * Caveat 1: using decltype(myFunc) will not compile if myFunc is overloaded.
     * Caveat 2: does not work (or at least is not tested yet) with member functions.
     * 
     * Trivial Example:
     * 
     *   float MaybeAddOne(int myInt, bool addOne) { return addOne ? myInt + 1.0 : float(myInt); }
     *   ResultsPool<decltype(MaybeAddOne)> myResultsPool(&MaybeAddOne);
     *   // alternately: ResultsPool<float(int,bool)> myResultsPool(&MaybeAddOne);
     *   for (int i = 0; i < 10000; i++) {
     *       fooResultsPool.SubmitWork(i, i % 2 == 0 ? true : false);
     *   }
     *   auto results = myResultsPool.WaitForAllResults();  // blocks to get a len 10000 std:vector<float>
     * 
     * Psuedocode for micro batch processing:
     * 
     *   ProcessedEvent ProcessEvent(RawEvent rawEvent) { // some code to process the event }
     * 
     *   int main() {
     *       EventSource eventSource(myConfig);
     *       ProcessedEventStore processedEventStore(myConfig);
     *       ResultsPool<decltype(ProcessEvent)> eventResultsPool(&ProcessEvent);
     *       std::vector<RawEvent> nextBatch;
     *       while (true) {
     *           sleep(myConfig.batchSecs);
     *           processedEventStore.Persist(eventResultsPool.GetAvailableResults());
     *           nextBatch = eventSource.GetNextBatch();
     *           for (int i = 0; i < nextBatch.length(); i++) {
     *               eventResultsPool.SubmitWork(nextBatch[i]);
     *           }
     *       }
     *   }
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
			ResultsPool(int numThreads, PoolResultsFuncPtr resultsFuncPtr) {
                _numThreads = numThreads;
                ResultsFuncPtr = resultsFuncPtr;
                _workers = std::make_unique<std::thread[]>(numThreads);
                _workerResultsMutexes = std::make_unique<std::mutex[]>(numThreads);

                for (int i = 0; i < _numThreads; i++) {
                    _workers[i] = std::thread(&ResultsPool::_WorkerLoop, this, i);
                }
            }

			~ResultsPool() {

            }

            /*
             * Get the number of workers this pool supports. Depending on pool status 0 or more of 
             * those workers may be active or currently doing work.
             */
			int GetNumWorkers() {
                return _numThreads;
            }

            /*
             * Get the pool's status
             */
            Status GetStatus() {
                return _status;
            }

            /*
             * Gets the number of 'jobs' in the work queue (i.e. have not yet be picked up by a worker). 
             */
            int GetSizeOfWorkQueue() {

            }

            /*
             * Submit some work to the pool. It will be picked up by any available workers.
             */
            void SubmitWork(ArgTs... args) {
                _workQueueMutex.lock();
                _workQueue.push(std::bind(ResultsFuncPtr, args...));
                _workQueueMutex.unlock();
                _addedWorkCondition.notify_one();
            }

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
			static void _EndWorker();
            // start all workers in the pool
            void _StartWorkers();

            // the entry point for pool worker threadss
            void _WorkerLoop(int workerNum) {

                std::unique_lock<std::mutex> myQueueLock(_workQueueMutex);
                std::unique_lock<std::mutex> myResultsLock(_workerResultsMutexes[workerNum]);
                while (true) {

                    myQueueLock.lock();
                    _addedWorkCondition.wait(myQueueLock, [this]{ return _workQueue.size() > 0; });
                    
                    std::function<RetT(void)> nextBoundFunc = _workQueue.front();
                    _workQueue.pop();
                    myQueueLock.unlock();
                    _addedWorkCondition.notify_one();

                    // TODO: stop worker

                    RetT result = nextBoundFunc();
                    myResultsLock.lock();
                    _workerResults[workerNum].push(result);
                    myResultsLock.unlock();
                }
            }

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
            std::unique_ptr<std::mutex[]> _workerResultsMutexes;

            // the queue of work workers pull work from. all call args already bound.
            std::queue< std::function<RetT(void)> > _workQueue;

            // the signal from master to workers that work has been added to the queue
			std::condition_variable _addedWorkCondition;

            // need to sync on the work queue
			std::mutex _workQueueMutex;
	};
}

#endif // RESULTS_POOL
