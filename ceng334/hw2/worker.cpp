#include "worker.hpp"

using namespace std;

wpobserver *Worker::observer = nullptr;

Worker::Worker(int ID, uint capacity, uint interval): ID(ID), capacity(capacity), 
            interval(interval), producedCount(0), active(true)
				{
	this->storageLock = PTHREAD_MUTEX_INITIALIZER;
};

void Worker::setObserver (wpobserver* o) {
    if (Worker::observer == nullptr) {
        Worker::observer = o;

    }
}

void Worker::sleep_awhile(){
    usleep(this->interval - (this->interval*0.01) + 
                        (std::rand()%(int)(this->interval*0.02)));
};

