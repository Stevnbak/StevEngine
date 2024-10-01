#include "Lockable.hpp"
#include <mutex>

namespace StevEngine {
	/*Lockable::Lockable() : lock(mutex) {
		lock.unlock();
	}
	Lockable::Lockable(const Lockable& o) : lock(mutex) {
		lock.unlock();
	}
	void Lockable::Lock() {
		lock.lock();
	}

	void Lockable::Unlock() {
		lock.unlock();
	}*/

	void Lockable::Lock() {
		//lock = new std::lock_guard(mutex);
	}

	void Lockable::Unlock() {
		//delete lock;
	}
}