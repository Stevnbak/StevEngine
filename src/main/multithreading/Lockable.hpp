#pragma once
#include <mutex>

namespace StevEngine {
	class Lockable {
		public:
			void Lock();
			void Unlock();
			Lockable() {};
			Lockable(const Lockable& o) {};
		private:
			std::mutex mutex;
			//std::unique_lock<std::mutex> lock;
			std::lock_guard<std::mutex>* lock;
	};
}