#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include "worker.hpp"

class Producer: public Worker{
    // Abstract class for Smelter & Foundry
    protected:
        Action CreatedMessage;
        
        mutex_t transporterLock;
        bool transporterFree;

    public:
        Producer() = delete;
        Producer(int ID, uint capacity, uint interval, Action createdMessage);

        bool markProducer();
        void unmarkProducer();

        // thread routine
        void work() override;
        void workerProduced() override;
        void workerStopped() override;

        virtual worker_t getType() = 0;
        virtual void ProduceFinished() = 0;
        virtual bool loadFromTransporter(OreType oreType, TransporterInfo *transporterInfo) = 0;
        virtual bool isAvailable(OreType) = 0;


        virtual ~Producer() {
            std::cerr << "A producer is dying\n";
        };
};

#endif // PRODUCER_HPP