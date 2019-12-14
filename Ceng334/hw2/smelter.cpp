#include "smelter.hpp"
#include "observer.hpp"

Smelter::Smelter (int ID, OreType oreType, uint capacity, uint interval): 
            Producer(ID, capacity, interval, SMELTER_CREATED), 
            oreType(oreType), waitingOreCount(0)
    {
    //init semaphores
    this->oreLock = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&this->waitingOre, 0, this->capacity); 
    sem_init(&this->oreCount, 0, 0); // initially no ore exists to produce ingot
    
    this->observer->Register(this, this->oreType);
    this->observer->inform(this->oreType, this->capacity);
    this->output(SMELTER_CREATED);

};
inline bool Smelter::waitCanProduce () {
    struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		std::cerr << "ERROR" << std::endl;
		return false;
	}
	ts.tv_sec += 5;
    if (sem_timedwait(&this->oreCount, &ts) == -1){
        return false;
    }
    if (sem_timedwait(&this->oreCount, &ts) == -1){
        return false;
    }
    return true;
}

inline void Smelter::ProduceFinished () {
    
    acquire(this->oreLock);
    this->waitingOreCount -= 2;
    if (this->waitingOreCount == 1)
        this->observer->primaryNeed(this, this->oreType);

    release(this->oreLock);
	sem_post(&this->waitingOre);
	sem_post(&this->waitingOre);
    this->observer->inform(this->oreType, 2);
}

bool Smelter::loadFromTransporter (OreType oreType, TransporterInfo *transporterInfo) {
    sem_wait(&this->waitingOre);
    acquire(this->transporterLock);

    this->output(TRANSPORTER_TRAVEL, transporterInfo);
	this->sleep_awhile(); // to travel

    acquire(this->oreLock);
    this->waitingOreCount++;
    if ( this->waitingOreCount == 1)
        this->observer->primaryNeed(this, this->oreType);

    release(this->oreLock);


    this->sleep_awhile(); // unloading
    this->output(TRANSPORTER_DROP_ORE, transporterInfo);
    sem_post(&this->oreCount);
    release(this->transporterLock);
    return true;
};

bool Smelter::isAvailable (OreType o){
    acquire(this->oreLock);
    bool is = this->isActive() & this->waitingOreCount < this->capacity;
    release(this->oreLock);
    return is;
}

inline void Smelter::output(Action action, TransporterInfo *transporterInfo){
    acquire(this->oreLock);    
    FillSmelterInfo(&this->smelterInfo, this->ID, this->oreType, this->capacity, this->waitingOreCount, this->producedCount);
    release(this->oreLock);
    WriteOutput(nullptr, transporterInfo, &this->smelterInfo, nullptr, action);
}