#pragma once
#include "main/multithreading/Lockable.hpp"
#include <functional>
#include <thread>
#include <vector>

namespace StevEngine {
	using JobFunction = std::function<void()>;
	template<typename T> using JobArgFunction = std::function<void(T arg)>;

	class JobQueue : Lockable {
		public:
			JobQueue(bool multithreading);
			void QueueJob(JobFunction function);
			void RunJob(JobFunction function);
			template<typename T>
			void RunJob(JobArgFunction<T> function, T arg) {
				Lock();
				if(multithreading) threads.emplace_back(function, arg);
				else function(arg);
				Unlock();
			}
			void RunAllJobs();
			void WaitForAllJobs();
		private:
			std::vector<JobFunction> queue;
			std::vector<std::thread> threads;
			bool multithreading;
	};
}