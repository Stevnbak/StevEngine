#include "JobQueue.hpp"
#include <atomic>
#include <thread>

namespace StevEngine {
	/*std::atomic<size_t> numThreads{ 0 };
	void Run(JobFunction function) {
		numThreads++;
		function();
		numThreads--;
	}*/


	JobQueue::JobQueue(bool multi) : multithreading(multi) {}
	//Add a job to the queue
	void JobQueue::QueueJob(JobFunction function) {
		queue.push_back(function);
	}
	//Directly run a job
	void JobQueue::RunJob(JobFunction function) {
		Lock();
		if(multithreading) threads.emplace_back(function);
		else function();
		Unlock();
	}
	//Create threads for all queued jobs
	void JobQueue::RunAllJobs() {
		if(multithreading)  {
			for(auto& func : queue) threads.emplace_back(func);
		} else {
			for(auto& func : queue) func();
		}
	}
	//Make sure all jobs are finished
	void JobQueue::WaitForAllJobs() {
		//while(numThreads > 0) {}
		auto iterator = threads.begin();
		while(iterator != threads.end()) {
			if(iterator->joinable()) iterator->join();
			iterator = threads.erase(iterator);
		}
	}
}