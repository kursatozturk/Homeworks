#include "miner.hpp"
Miner::Miner(int ID, OreType oreType, uint capacity, uint interval, uint totalOre): 
        Worker(ID, capacity, interval), totalOre(totalOre), oreType(oreType) {

    this->finish = PTHREAD_COND_INITIALIZER;
    sem_init(&this->canProduce, 0, this->capacity);
    this->output(MINER_CREATED);
};

void Miner::work(){

	this->observer->waitStart();

    for(int i = 0; i < this->totalOre; ++i){
        this->waitCanProduce();
        this->output(MINER_STARTED);
        this->workerProduced();
        this->output(MINER_FINISHED);
        this->sleep_awhile();
    }
    this->workerStopped();
};

bool Miner::Load2Transporter(TransporterInfo *transporterInfo){

    *transporterInfo->carry = this->oreType;
    this->output(TRANSPORTER_TRAVEL);

    this->sleep_awhile(); // to travel

    acquire(this->storageLock);
    this->producedCount--;
    release(this->storageLock);

    this->output(TRANSPORTER_TAKE_ORE, transporterInfo);
    
    this->sleep_awhile(); // loading

    signal(this->finish); // thinkin its not safe
    sem_post(&this->canProduce);

    return true;
};

inline bool Miner::waitCanProduce () {
    sem_wait(&this->canProduce);
    return true;
};

inline void Miner::workerProduced () {

    this->sleep_awhile();

	acquire(this->storageLock);
    this->producedCount++;
	release(this->storageLock);

    Worker::observer->warnProduce(this->oreType, this);
};

inline void Miner::workerStopped () {
    acquire(this->storageLock);
    while (this->producedCount > 0) {
        wait(this->finish, this->storageLock);
    }
    this->active = false;
    release(this->storageLock);
    Worker::observer->informFinished(this->ID);
    this->output(MINER_STOPPED);
};

inline void Miner::output(Action action, TransporterInfo *transporterInfo){
	acquire(this->storageLock);
    FillMinerInfo(&this->minerInfo, this->ID, this->oreType, this->capacity, this->producedCount);
	release(this->storageLock);
    WriteOutput(&this->minerInfo, transporterInfo, nullptr, nullptr, action);
}