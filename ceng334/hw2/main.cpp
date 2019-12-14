#include "miner.hpp"
#include "smelter.hpp"
#include "foundry.hpp"
#include "transporter.hpp"
extern "C" {
    #include <sys/stat.h> 
    #include <fcntl.h>
}

using namespace std;

//wpobserver *Transporter::observer = new wpobserver();
//wpobserver *Worker::observer = Transporter::getObserver();

OreType getoretype(int val) {
    switch(val) {
        case 0: 
            return IRON;
            break;
        case 1:
            return COPPER;
            break;
        case 2:
            return COAL;
            break;
    }
    return IRON;
}
struct mine {
    int ID;
    uint interval;
    uint capacity;
    OreType oreType;
    uint totalOre;
    mine(int ID, uint i, uint c, OreType ot, uint tO): ID(ID), interval(i), capacity(c), oreType(ot), totalOre(tO) { };
};
struct transporter {
    int ID;
    uint interval;
    transporter(int ID, uint i): ID(ID), interval(i) { };
};
struct smelter {
    int ID;
    uint interval;
    uint capacity;
    OreType oreType;
    smelter(int ID, uint i, uint c, OreType ot): ID(ID), interval(i), capacity(c), oreType(ot) { };
};
struct foundry {
    int ID;
    uint interval;
    uint capacity;
    foundry(int ID, uint i, uint c): ID(ID), interval(i), capacity(c) { };
};
class InputGetter {

public:
    int number_of_miner;
    int number_of_smelter;
    int number_of_foundry;
    int number_of_transporter;
    int number_of_iron_smelter;
    int number_of_copper_smelter;
    vector<mine> miner_threads;
    vector<transporter> transporter_threads;
    vector<smelter> smelter_threads;
    vector<foundry> foundry_threads;
    
    InputGetter() : number_of_iron_smelter(0), number_of_copper_smelter(0) {}

    friend istream & operator >> (istream &in,  InputGetter &ig);

};

istream &operator >> (istream &in, InputGetter &ig) {
    in >> ig.number_of_miner;
    for (int i = 0; i < ig.number_of_miner; ++i) {
        uint interval;
        uint capacity;
        int oreType;
        uint totalOre;
        in >> interval >> capacity >> oreType >> totalOre;
        ig.miner_threads.push_back(mine(i + 1, interval, capacity, getoretype(oreType), totalOre));
        // create 
    }
    in >> ig.number_of_transporter;
    for (int i = 0; i < ig.number_of_transporter; ++i) {
        uint interval;
        in >> interval;
        ig.transporter_threads.push_back(transporter(i + 1, interval));
    }
    in >> ig.number_of_smelter;
    for (int i = 0; i < ig.number_of_smelter; ++i) {
        uint interval;
        uint capacity;
        int oreType;
        in >> interval >> capacity >> oreType;
        ig.smelter_threads.push_back(smelter(i + 1, interval, capacity, getoretype(oreType)));
        if (getoretype(oreType) == IRON) {
            ig.number_of_iron_smelter++;
        }
        else if (getoretype(oreType) == COPPER) {
            ig.number_of_copper_smelter++;
        }
    }
    in >> ig.number_of_foundry;
    for (int i = 0; i < ig.number_of_foundry; ++i) {
        uint interval;
        uint capacity;
        in >> interval >> capacity;
        ig.foundry_threads.push_back(foundry(i + 1, interval, capacity));
    }
    return in;
};


typedef struct thread_routine_param_t {
    void *obj;
    worker_t type;
    thread_routine_param_t(void* o, worker_t wt) : obj(o), type(wt) { }
} trp_t;

#define c_thread( t_id, OBJECT, TYPE) pthread_create(&t_id, nullptr, thread_routine, (void *)new trp_t((void*)OBJECT, TYPE))
#define wait_for_each(id_list) for(auto &id: id_list) pthread_join(id, nullptr)

void *thread_routine(void* param) {
    
    switch (((trp_t*)param)->type) {
        case MINER:
        case SMELTER:
        case FOUNDRY:
            ((Worker*) ((trp_t*)param)->obj)->work();
            break;
        case TRANSPORTER:
            ((Transporter*) ((trp_t*)param)->obj)->work();
            break;
    }
    return nullptr;
}
int main () {
    /*
    auto fd = open("error.log", O_WRONLY | O_APPEND);
    if(dup2(fd, STDERR_FILENO) < 0){
        ;   
    }
    */
    InputGetter it;
    cin >> it;
    Worker::setObserver(Transporter::FillObserver(it.number_of_miner, it.number_of_iron_smelter, it.number_of_copper_smelter, it.number_of_foundry));

    vector<Miner*> minerList;
    vector<Smelter*> smelterList;
    vector<Foundry*> foundryList;
    vector<Transporter*> transporterList;
    InitWriteOutput();

    for (auto m: it.miner_threads) {
        minerList.push_back(new Miner(m.ID, m.oreType, m.capacity, m.interval, m.totalOre));
    }
    for (auto s: it.smelter_threads) {
        smelterList.push_back(new Smelter(s.ID, s.oreType, s.capacity, s.interval));
    }
    for (auto f: it.foundry_threads) {
        foundryList.push_back(new Foundry(f.ID, f.capacity, f.interval));
    }
    for (auto t: it.transporter_threads) {
        transporterList.push_back(new Transporter(t.ID, t.interval));
    }
    // Let Debuggin Start
    vector<pthread_t> thread_ids_miners(it.number_of_miner, 0);
    vector<pthread_t> thread_ids_smelter(it.number_of_smelter, 0);
    vector<pthread_t> thread_ids_foundry(it.number_of_foundry, 0);
    vector<pthread_t> thread_ids_transporter(it.number_of_transporter, 0);

    for (size_t i = 0; i < minerList.size(); i++) {
        Miner *m = minerList[i];
        c_thread(thread_ids_miners[i], m, MINER);
    }
    
    for (size_t i = 0; i < transporterList.size(); i++) {
        Transporter *t = transporterList[i];
        c_thread(thread_ids_transporter[i], t, TRANSPORTER);
    }

    for (size_t i = 0; i < smelterList.size(); i++) {
        Smelter *s = smelterList[i];
        c_thread(thread_ids_smelter[i], s, SMELTER);
    }
    for (size_t i = 0; i < foundryList.size(); i++) {
        Foundry *f = foundryList[i];
        c_thread(thread_ids_foundry[i], f, FOUNDRY);
    }
    Transporter::getObserver()->start();

    wait_for_each(thread_ids_miners);
    wait_for_each(thread_ids_smelter);
    wait_for_each(thread_ids_foundry);
    wait_for_each(thread_ids_transporter);
    
    return 0;
}