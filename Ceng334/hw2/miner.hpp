#ifndef MINER_HPP
#define MINER_HPP

#include "worker.hpp"

class Miner: public Worker{
	private:
		uint totalOre;
		OreType oreType;
		MinerInfo minerInfo;

		
		// add semaphores, locks and other data below.
		sem_t canProduce;
        cond_t finish;
    public:
        // initializers
        Miner() = delete;
        Miner(int ID, OreType oreType, uint capacity, uint interval, uint totalOre);

        // transporter interface
        bool Load2Transporter(TransporterInfo *transporterInfo);

	private:
		// utilities
        void output(Action action, TransporterInfo *transporterInfo = NULL) override;
		bool waitCanProduce() override;
		void workerStopped() override;
		void workerProduced() override;
	public:
        // thread routine
        void work() override;

		//getFunctions
		inline OreType getOreType () { return this->oreType; }
		virtual ~Miner() {std::cerr << "A Miner is dying\n";}
		 
};

#endif