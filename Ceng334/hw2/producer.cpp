#include "producer.hpp"
Producer::Producer (int ID, uint capacity, uint interval, Action createdMessage) : 
			Worker(ID, capacity, interval), CreatedMessage(createdMessage),transporterFree(true)
	{
		this->transporterLock = PTHREAD_MUTEX_INITIALIZER;
	}

void Producer::work () {
	this->observer->waitStart();
	while (this->waitCanProduce()) {
		this->output( (Action) ((int)(this->CreatedMessage) + 1));
		this->workerProduced();
		this->output( (Action) ((int)(this->CreatedMessage) + 2));
	}
    this->workerStopped();
    this->output((Action) ((int)(this->CreatedMessage) + 3));
}


inline void Producer::workerProduced () {
    this->sleep_awhile();

	acquire(this->storageLock);
    this->producedCount++;
	release(this->storageLock);

	this->ProduceFinished();

};
bool Producer::markProducer () {
	acquire(this->transporterLock);
	if (this->transporterFree) {
		this->transporterFree = false;
		return true;
	}
	else return false;
	release(this->transporterLock);
}
void Producer::unmarkProducer () {
	acquire(this->transporterLock);
	this->transporterFree = true;
	release(this->transporterLock);

}
inline void Producer::workerStopped () {
	this->active = false;
};
