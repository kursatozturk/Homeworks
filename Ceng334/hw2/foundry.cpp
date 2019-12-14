#include "foundry.hpp"

Foundry::Foundry(int ID, uint capacity, uint interval) : Producer(ID, capacity, interval, FOUNDRY_CREATED),
					waitingIronCount(0), waitingCoalCount(0) {

			sem_init(&this->ironCount, 0, 0);
			sem_init(&this->waitIron, 0, this->capacity);
			sem_init(&this->coalCount, 0, 0);
			sem_init(&this->waitCoal, 0, this->capacity);

			this->ironLock = PTHREAD_MUTEX_INITIALIZER;
			this->coalLock = PTHREAD_MUTEX_INITIALIZER;
			this->observer->Register(this);

			this->observer->inform(IRON, this->capacity);
			this->observer->inform(COAL, this->capacity);

			this->output(FOUNDRY_CREATED);
};

inline bool Foundry::waitCanProduce() {
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		return false;
	}
	ts.tv_sec += 5;
	if (sem_timedwait(&this->ironCount, &ts) == -1)
		return false;

	if (sem_timedwait(&this->coalCount, &ts) == -1)
		return false;
	return true;
}

inline void Foundry::ProduceFinished () {
	acquire(this->ironLock);
	this->waitingIronCount--;
	sem_post(&this->waitIron);

	acquire(this->coalLock);
	this->waitingCoalCount--;
	sem_post(&this->waitCoal);


	if (this->waitingIronCount == 1 && this->waitingCoalCount == 0) {
		this->observer->primaryNeed(this, COAL);

	}
	if (this->waitingIronCount == 0 && this->waitingCoalCount == 1) {
		this->observer->primaryNeed(this, IRON);
	}

	release(this->ironLock);
	release(this->coalLock);
	this->observer->inform(IRON, 1);
	this->observer->inform(COAL, 1);
}

bool Foundry::loadFromTransporter(OreType oreType, TransporterInfo *transporterInfo) {

	if (oreType == IRON) {
		sem_wait(&this->waitIron);
		acquire(this->transporterLock);

		this->output(TRANSPORTER_TRAVEL, transporterInfo);
		this->sleep_awhile(); // to travel

		acquire(this->ironLock);
		this->waitingIronCount++;
		release(this->ironLock);

		acquire(this->coalLock);
		if (this->waitingCoalCount == 0) 
			this->observer->primaryNeed(this, COAL);
		release(this->coalLock);
		sem_post(&this->coalCount);

	}
	else if (oreType == COAL) {
		sem_wait(&this->waitCoal);

		acquire(this->transporterLock);
		this->output(TRANSPORTER_TRAVEL, transporterInfo);
		
		this->sleep_awhile(); // to travel

		acquire(this->coalLock);
		this->waitingCoalCount++;
		release(this->coalLock);

		acquire(this->ironLock);

		
		if (this->waitingIronCount == 0)
			this->observer->primaryNeed(this, IRON);
		release(this->ironLock);

		sem_post(&this->ironCount);
	}

	this->sleep_awhile(); // unloading

	this->output(TRANSPORTER_DROP_ORE, transporterInfo);
	release(this->transporterLock);
	return true;
};
bool Foundry::isAvailable (OreType o) {
	bool is = this->isActive();
	if (o == IRON) {
		acquire(this->ironLock);
		is &= this->waitingIronCount < this->capacity;
		release(this->ironLock);
	}
	else if (o == COAL) {
		acquire(this->coalLock);
		is &= this->waitingCoalCount < this->capacity;
		release(this->coalLock);
	}
	else {
		is = false;
	}
	return is;
}
inline void Foundry::output(Action action, TransporterInfo *transporterInfo) {
	acquire(this->ironLock);
	acquire(this->coalLock);
	FillFoundryInfo(&this->foundryInfo, this->ID, this->capacity, this->waitingIronCount, this->waitingCoalCount,
		this->producedCount);
	release(this->ironLock);
	release(this->coalLock);
	WriteOutput(nullptr, transporterInfo, nullptr, &this->foundryInfo, action);
};