#ifndef TRANSPORTER_HPP
#define TRANSPORTER_HPP

#include "producer.hpp"
#include "miner.hpp"


class Transporter{
    int ID;
    uint interval;
    OreType carriedOre;
    TransporterInfo transporterInfo;
protected:
    static wpobserver *observer;
public:
    static wpobserver* FillObserver(int minerCount, int ironsmelterCount, int coppersmelterCount, int foundryCount);
    static wpobserver *getObserver() {return observer;}
public:

    Transporter() = delete;
    Transporter(int ID, uint interval);

    // main thread routine
    void work();

    void error_logger(char* msg);

    void output(Action action);
    void fill(int OreType);

};

#endif