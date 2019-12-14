#ifndef SMELTER_HPP
#define SMELTER_HPP

#include "producer.hpp"

class Smelter: public Producer{
	private:
        int waitingOreCount;
        SmelterInfo smelterInfo;
		OreType oreType;
        // add semaphores, locks and other data below.

        sem_t waitingOre;
        sem_t oreCount;

        mutex_t oreLock;

		// utilities

        bool waitCanProduce() override;
        void output(Action action, TransporterInfo *transporterInfo = NULL) override;
        void ProduceFinished() override;
		bool isAvailable (OreType o) override;
        worker_t getType () override {return SMELTER;}

	public:
        Smelter() = delete;
        Smelter(int ID, OreType oreType, uint capacity, uint interval);
        
		// transporter interface
        bool loadFromTransporter (OreType oreType, TransporterInfo *transporterInfo) override;



        
};
#endif