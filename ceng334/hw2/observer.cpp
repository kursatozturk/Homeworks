#include "observer.hpp"
using namespace std;

template<class Worker, class Producer>
Observer<Worker, Producer>::Observer ():
	lastWorkerIdx(0) {
		

	this->workerLock = PTHREAD_MUTEX_INITIALIZER;
	this->producerLock = PTHREAD_MUTEX_INITIALIZER;
	this->produced = PTHREAD_COND_INITIALIZER;

	sem_init(&this->IronAsked, 0, 0);
	sem_init(&this->CopperAsked, 0, 0);
	sem_init(&this->CoalAsked, 0, 0);

	//initialization is not finished yet

}

template<class Worker, class Producer>
void Observer<Worker, Producer>::FillObserver (int minerCount, int smelterCount, int foundryCount) {
		this->minerCount = minerCount;
		this->smelterCount = smelterCount;
		this->foundryCount = foundryCount;
		this->workers = vector<Worker*>(minerCount, nullptr);
		this->producers_primary = vector<PrimaryNeed<Producer> *>(smelterCount + foundryCount, nullptr);
};

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::hire (Worker* worker) {
	int ID = worker->getID();
	this->workers[ID] = worker;
};

template<class Worker, class Producer>
inline int Observer<Worker, Producer>::hire (Producer *producer) {
	this->producers_secondary.push_back(producer);
	return this->producers_secondary.size() - 1;
}

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::primaryNeed (int idx, OreType requires) {
	acquire(this->producerLock);
	Producer *p = this->producers_secondary[idx];
	this->producers_secondary[idx] = nullptr;
	this->producers_primary[idx] = new PrimaryNeed<Producer>(p, requires);
	release(this->producerLock);
}

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::informCapacityOpens (OreType oreType, uint count) {
	switch (oreType) {
		case IRON:
			for(uint i = 0; i < count; ++i)
				sem_post(&this->IronAsked);
			break;
		case COAL:
			for(uint i = 0; i < count; ++i)
				sem_post(&this->CoalAsked);
			break;
		case COPPER:
			for(uint i = 0; i < count; ++i)
				sem_post(&this->CopperAsked);
			break;
	}
};

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::informCapacity (OreType oreType, uint capacity) {
	switch (oreType) {
		case IRON:
			for(uint i = 0; i < capacity; ++i)
				sem_post(&this->IronAsked);
			break;
		case COAL:
			for(uint i = 0; i < capacity; ++i)
				sem_post(&this->CoalAsked);
			break;
		case COPPER:
			for(uint i = 0; i < capacity; ++i)
				sem_post(&this->CopperAsked);
			break;
	}
};
template<class Worker, class Producer>
inline void Observer<Worker, Producer>::informFinished(int ID){
	acquire(this->workerLock);
	this->workers[ID] = nullptr;
	this->minerCount--;
	release(this->workerLock);
}
template<class Worker, class Producer>
inline bool Observer<Worker, Producer>::workerWorks () {
	bool works = true;
	acquire(this->workerLock);
	works = this->minerCount != 0;
	release(this->workerLock);
	return works;
}
template<class Worker, class Producer>
inline void Observer<Worker, Producer>::warnProduce (OreType oreType) {
	signal(this->produced);
}
template<class Worker, class Producer>
inline Worker *Observer<Worker, Producer>::findMiner () {
	acquire(this->workerLock);
	wait(this->produced, this->workerLock);
	size_t size = (this->workers).size();
	size_t idx = this->lastWorkerIdx + 1;
	if(idx == size){
		idx = 0;
	}
	Worker *w;
	for(size_t i = idx; i < size; ++i){
		if(i == this->lastWorkerIdx)
			break;
		w = this->workers[i];
		if(w->isActive() && w->isProduced()){
			this->lastWorkerIdx = i;
			release(this->workerLock);
			return w;
		}
		if(i == size - 1)
			i = 0;
	}
	release(this->workerLock);
	// function must not reach this point by the design
	// In case of it, put error stater
	cerr << "MINER CANNOT BE FOUND" << endl;
	return nullptr;
}
template<class Worker, class Producer>
Producer *Observer<Worker, Producer>::findProducer (OreType oreType) {
	switch (oreType)
	{
	case IRON:
		sem_wait(&this->IronAsked);
		break;
	case COPPER:
		sem_wait(&this->CopperAsked);
		break;
	case COAL:
		sem_wait(&this->CoalAsked);
		break;
	default:
		cerr << " Why do we have to do this? " << endl;
		break;
	}

	acquire(this->producerLock);
	size_t size = this->producers_primary.size();
	Producer *producer;
	size_t idx = 0;
	for (auto &p: this->producers_primary) {
		idx++;
		if( p == nullptr)continue;
		if (p->isActive() && p->waits(oreType)) {
			producer = p->p;
			this->producers_primary[idx] = nullptr;
			this->producers_secondary[producer->getIdx()] = producer;
			release(this->producerLock);
			return producer;
		}
	}
	// if not returned, continue to secondary producers

	idx = 0;
	for(auto &p: this->producers_secondary){
		idx++;
		if(p == nullptr)continue;
		if(p->isActive() && p->waits(oreType)) {
			release(this->producerLock);
			return p;
		}
	}
		
}
