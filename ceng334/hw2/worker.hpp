#ifndef WORKER_HPP
#define WORKER_HPP

extern "C"
{
    #include <unistd.h>
}

#include <cstdlib>

#include "observer.hpp"

class Producer;
class Worker;
typedef Observer<Worker, Producer> wpobserver;

class Worker{

    protected:
        int ID;
        uint capacity;
        uint interval;
        uint producedCount;
        bool active;

        static wpobserver *observer;
    public:
        static void setObserver(wpobserver*);

		mutex_t storageLock;


		virtual void output(Action action, TransporterInfo *transporterInfo = NULL) = 0;
		virtual bool waitCanProduce() = 0;
		virtual void workerStopped() = 0;
        virtual void workerProduced() = 0;


    public:
        Worker() = delete;
        Worker(int ID, uint capacity, uint interval);

        // Child class spesific routines
        virtual void work() = 0;

        // common utilities
        void sleep_awhile();
        void stopWork();

        void error_logger (char *msg, int no) {
            char wrto[512];
            std::sprintf(wrto, "ID: %d, msg: %s and no: %d\n", this->ID, msg, no);
            std::cerr << wrto;
        }
		// get functions
		inline int getID() { return this->ID; }
        
        inline bool isProduced () {
             acquire(this->storageLock);
             bool is = this->producedCount > 0;
             release(this->storageLock);
             return is;
             }
        virtual bool isActive () { return this->active;}

        virtual ~Worker() { };

};
#endif
