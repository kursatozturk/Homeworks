#include "transporter.hpp"

wpobserver *Transporter::observer = nullptr;

Transporter::Transporter (int ID, uint interval): ID(ID), interval(interval) {
    this->output(TRANSPORTER_CREATED);
    this->transporterInfo.carry = new OreType;
}
wpobserver *Transporter::FillObserver (int minerCount, int ironsmelterCount, int coppersmelterCount, int foundryCount) {
    Transporter::observer = new wpobserver();
    Transporter::observer->FillObserver(minerCount, ironsmelterCount, coppersmelterCount, foundryCount);
    return Transporter::observer;
}
void Transporter::error_logger (char* msg) {
    char wrto[512];
    std::sprintf(wrto, "ID: %d, msg: %s\n", this->ID, msg);
    std::cerr << wrto;
}
void Transporter::fill (int oreType) {
    OreType *o = this->transporterInfo.carry;
    if (oreType != -1)
        *o = (OreType) oreType;
    FillTransporterInfo(&this->transporterInfo, this->ID, o);
}
void Transporter::work () {
    this->observer->waitStart();
    while (this->observer->workerWorks()) {
        Miner* w = (Miner* ) this->observer->findMiner();
        if (w == nullptr) continue;
        this->fill(-1);
        if (w->Load2Transporter(&this->transporterInfo)) {
            this->carriedOre = w->getOreType();
            this->observer->releaseWorker();
        }
        else {
            this->observer->releaseWorker();
            continue;
        }
        
        Producer *p = this->observer->findProducer(this->carriedOre);
        this->fill(this->carriedOre);
        if (p != nullptr && p->loadFromTransporter(this->carriedOre, &this->transporterInfo)) {
            // one travel finished.
            // continue to other one.
            this->observer->releaseProducer(p->getType(), this->carriedOre);
            continue;
        }
        else {
            continue;
        }
    }
    FillTransporterInfo(&this->transporterInfo, this->ID, &this->carriedOre);
    WriteOutput(NULL, &this->transporterInfo, NULL, NULL, TRANSPORTER_STOPPED);

}


inline void Transporter::output (Action action) {

    FillTransporterInfo(&this->transporterInfo, this->ID, &this->carriedOre);
    WriteOutput(NULL, &this->transporterInfo, NULL, NULL, action);

}


