#ifndef OBSERVER_HPP
#define OBSERVER_HPP

extern "C" {
	#include "writeOutput.h"
}
#include <vector>
#include <queue>
#include <iostream>
#include <cstdio>
using namespace std;

#define NOT_IMPLEMENTED return;
#define acquire(lock) pthread_mutex_lock(&lock)
#define release(lock) pthread_mutex_unlock(&lock)
#define signal(cond) pthread_cond_signal(&cond)
#define wait(cond, lock) pthread_cond_wait(&cond, &lock)
enum ThreadWorkerType { MINER, SMELTER, FOUNDRY, TRANSPORTER };

typedef pthread_cond_t cond_t;
typedef pthread_mutex_t mutex_t;
typedef ThreadWorkerType worker_t;

#define logger(msg, no) \
    std::sprintf(wrto, "no: %d, msg: %s\n",no, msg); \
    std::cerr << wrto; \

template<class Worker, class Producer>
class Observer{

	std::queue<Worker*> std_workers;

	std::vector<Producer*> std_smelters[2];
	std::vector<Producer*> std_foundry;

	std::queue<Producer*> prm_waiters[3];

	sem_t start_sem;
	mutex_t start_lock;
	cond_t start_cond;

	int counts[3];
	size_t smelter_counts[2];

	size_t lastSmelterIdx[2];
	size_t lastFoundryIdx;
	size_t lastWorkerIdx;

	sem_t smelter_wait[2];
	sem_t foundry_wait;

	bool turn_foundry;

	sem_t asked[3];
	sem_t produced;
	mutex_t workerLock;
	mutex_t smelterLock[2];
	mutex_t foundryLock;
	mutex_t primaryLock;
	sem_t worker_holder;

	// hold active miner count
	// if it reaches zero, inform transporters

    public:
		Observer();
        void FillObserver(int minerCount, int ironSmelterCount, int copperSmelterCount, int FoundryCond);
		void waitStart();
		void start();
		// Producer interface
		void primaryNeed(Producer *, OreType requires);
		void Register(Producer* p);
		void Register(Producer* p, OreType oreType);
		void inform(OreType oreType, uint capacity);
        // Worker interface
		void warnProduce(OreType oreType, Worker *w);
		void informFinished(int ID);

		// transporter interface
		Worker *findMiner();
		Producer *checkFoundry(OreType oreType);
		Producer *checkSmelter(OreType oreType);
		Producer *findProducer(OreType oreType);
		bool workerWorks();
		inline void releaseWorker() {sem_post(&this->worker_holder);};
		void releaseProducer (worker_t wt, OreType oreType) {
			switch (wt) {
			case FOUNDRY:
				sem_post(&this->foundry_wait);
				break;
			case SMELTER:
				sem_post(&this->smelter_wait[oreType]);
				break;
			}
		};
};


// implementation


template<class Worker, class Producer>
Observer<Worker, Producer>::Observer ():
	lastWorkerIdx(-1), lastSmelterIdx{0, 0}, lastFoundryIdx(0) {
	this->workerLock = PTHREAD_MUTEX_INITIALIZER;
	this->smelterLock[IRON] = PTHREAD_MUTEX_INITIALIZER;
	this->smelterLock[COPPER] = PTHREAD_MUTEX_INITIALIZER;
	this->foundryLock = PTHREAD_MUTEX_INITIALIZER;
	this->primaryLock = PTHREAD_MUTEX_INITIALIZER;

	this->start_cond = PTHREAD_COND_INITIALIZER;
	this->start_lock = PTHREAD_MUTEX_INITIALIZER;
	sem_init(&this->produced, 0, 0);
	sem_init(&this->start_sem, 0, 0);

	//initialization is not finished yet
}

template<class Worker, class Producer>
void Observer<Worker, Producer>::FillObserver (int minerCount, int ironSmelterCount, int copperSmelterCount, int foundryCount) {

		this->counts[MINER] = minerCount;
		this->counts[SMELTER] = ironSmelterCount + copperSmelterCount;
		this->counts[FOUNDRY] = foundryCount;
		this->smelter_counts[IRON] = ironSmelterCount;
		this->smelter_counts[COPPER] = copperSmelterCount;

		sem_init(&this->worker_holder, 0, minerCount);
		sem_init(&this->smelter_wait[IRON], 0, ironSmelterCount);
		sem_init(&this->smelter_wait[COPPER], 0, copperSmelterCount);
		sem_init(&this->foundry_wait, 0, foundryCount);
		sem_init(&this->asked[IRON], 0, 0);
		sem_init(&this->asked[COPPER], 0, 0);
		sem_init(&this->asked[COAL], 0, 0);
};

template<class Worker, class Producer>
void Observer<Worker, Producer>::waitStart () {
	sem_post(&this->start_sem);
	acquire(this->start_lock);
	wait(this->start_cond, this->start_lock);
	release(this->start_lock);
}
template<class Worker, class Producer>
void Observer<Worker, Producer>::start () {
	for(int i = 0; i < this->counts[0] + this->counts[1] + this->counts[2]; ++i)
		sem_wait(&this->start_sem);
	pthread_cond_broadcast(&this->start_cond);
}

template<class Worker, class Producer>
void Observer<Worker, Producer>::Register (Producer* p) {
	this->std_foundry.push_back(p);
}

template<class Worker, class Producer>
void Observer<Worker, Producer>::Register (Producer* p, OreType oreType) {
	this->std_smelters[oreType].push_back(p);
}

template<class Worker, class Producer>
void Observer<Worker, Producer>::inform (OreType oreType, uint capacity) {
	for (int i = 0; i < capacity; ++i)
		sem_post(&this->asked[oreType]);
}

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::primaryNeed (Producer *p, OreType requires) {
	acquire(this->primaryLock);
	this->prm_waiters[requires].push(p);
	release(this->primaryLock);
}

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::informFinished (int ID) {
	acquire(this->workerLock);
	char wrto[512];
	this->counts[MINER]--;
	std::cerr << wrto;
	release(this->workerLock);
}

template<class Worker, class Producer>
inline bool Observer<Worker, Producer>::workerWorks () {
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		return false;
	}
	ts.tv_sec += 5;
	if (sem_timedwait(&this->produced, &ts) == -1) {
		return false;
	}
	bool works = true;
	acquire(this->workerLock);
	works = this->counts[MINER] != 0;
	release(this->workerLock);
	return works;
}

template<class Worker, class Producer>
inline void Observer<Worker, Producer>::warnProduce (OreType oreType, Worker *w) {
	acquire(this->workerLock);
	this->std_workers.push(w);
	release(this->workerLock);
	sem_post(&this->produced);
}

template<class Worker, class Producer>
inline Worker *Observer<Worker, Producer>::findMiner () {
	sem_wait(&this->worker_holder);
	acquire(this->workerLock);

	Worker *w = nullptr;
	if (!this->std_workers.empty()) {
		w = this->std_workers.front();
		this->std_workers.pop();
	}
	release(this->workerLock);
	return w;
}

template<class Worker, class Producer>
Producer *Observer<Worker, Producer>::checkSmelter (OreType oreType) {
	Producer *smelter = nullptr;
	if (this->std_smelters[oreType].size() == 0)return nullptr;
	sem_wait(&this->smelter_wait[oreType]);
	acquire(this->smelterLock[oreType]);
	size_t idx = this->lastSmelterIdx[oreType];
	
	while (true) {
		if (idx == this->smelter_counts[oreType])
			idx = 0;
		Producer *p = this->std_smelters[oreType][idx];
		if (p->isAvailable(oreType)) {
			smelter = p;
			this->lastSmelterIdx[oreType] = idx + 1;
			break;
		}
		idx++;
		if (idx == this->lastSmelterIdx[oreType]) {
			sem_post(&this->smelter_wait[oreType]);
			break;
		}
	}
	release(this->smelterLock[oreType]);
	return smelter;
}

template<class Worker, class Producer>
Producer *Observer<Worker, Producer>::checkFoundry (OreType oreType) {
	Producer *foundry = nullptr;
	if (this->std_foundry.size() == 0)return nullptr;
	sem_wait(&this->foundry_wait);
	acquire(this->foundryLock);
	size_t idx = this->lastFoundryIdx;
	while (true) {
		if (idx == this->counts[FOUNDRY])
			idx = 0;
		Producer *fo = this->std_foundry[idx];
		if (fo->isAvailable(oreType)) {
			foundry = fo;
			this->lastFoundryIdx = idx + 1;
			break;
		}
		idx++;
		if (idx == this->lastFoundryIdx) {
			sem_post(&this->foundry_wait);
			break;
		}
	}
	release(this->foundryLock);
	return foundry;
	}
template<class Worker, class Producer>
Producer *Observer<Worker, Producer>::findProducer (OreType oreType) {
	Producer *p = nullptr;
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		return nullptr;
	}
	ts.tv_sec += 5;
	if (sem_timedwait(&this->asked[oreType], &ts) == ETIMEDOUT) 
		return nullptr; // wait till capacity opens
	// If any prior producer is waiting, find it
	acquire(this->primaryLock);
	if (!this->prm_waiters[oreType].empty()) {
		p = this->prm_waiters[oreType].front();
		this->prm_waiters[oreType].pop();
		release(this->primaryLock);
	}
	// if not, continue with having storage open
	else {
		release(this->primaryLock);
		switch (oreType)
		{
		case COPPER:
			// Only smelters uses copper. So if there exists any availability, it is smelter.
			p = this->checkSmelter(oreType);
			break;
		case COAL:
			// Only foundries uses coal. So if there exists any availability, it is foundry.
			p = this->checkFoundry(oreType);
			break;
		case IRON:
			// Both smelter and foundry uses iron. One of them must have opening. 
			// Add a turn priority to make it distinguished equally.
			if (this->turn_foundry) {
				// turn is in foundry
				p = this->checkFoundry(oreType);
				if (!p) // if no foundry is available, check smelters.
					p = this->checkSmelter(oreType);
				else
					this->turn_foundry = !this->turn_foundry;
			}
			else {
				//turn is in smelter
				p = this->checkSmelter(oreType);
				if (!p)  // if no smelter is available, check foundries.
					p = this->checkFoundry(oreType);
				else
					this->turn_foundry = !this->turn_foundry;
			}
			break;
		default:
			break;
		}
		
	}
	return p;
}


#endif