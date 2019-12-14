#ifndef FOUNDRY_HPP
#define FOUNDRY_HPP

#include "producer.hpp"

class Foundry : public Producer {
	private:
		uint waitingIronCount;
		uint waitingCoalCount;
		FoundryInfo foundryInfo;

		sem_t waitIron;
		sem_t waitCoal;
		sem_t ironCount;
		sem_t coalCount;

		mutex_t ironLock;
		mutex_t coalLock;
		
		void ProduceFinished() override;
		bool waitCanProduce() override;
		void output(Action action, TransporterInfo *transporterInfo = NULL) override;
		bool isAvailable (OreType o) override;
        worker_t getType () override {return FOUNDRY;}
	
	public:
		Foundry() = delete;
		Foundry(int ID, uint capacity, uint interval);

		

		// Transporter interface
		bool loadFromTransporter(OreType oreType, TransporterInfo *transporterInfo) override;
};


#endif