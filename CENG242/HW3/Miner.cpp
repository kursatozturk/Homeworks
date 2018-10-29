#include "Miner.h"


int Miner::getNextAvailableBlockchainID() const{
	return BlockchainCount;
}

Miner::~Miner(){
	for(int i = 0; i < BlockchainCount; ++i){
		if(blocks[i] != nullptr)delete blocks[i];
		blocks[i] == nullptr;	
	}
    blocks.clear();
	this->name = "";
	this->BlockchainCount = 0;
}

void Miner::createNewBlockchain(){
	blocks.push_back(new Blockchain(getNextAvailableBlockchainID()));
	(this->BlockchainCount)++;
}

void Miner::mineUntil(int cycleCount){
	for(int i = 0; i < cycleCount; ++i)	
		for(int j = 0; j < BlockchainCount; ++j) ++(*blocks[j]);
}

void Miner::demineUntil(int cycleCount){
	for(int i = 0; i < cycleCount; ++i)	
		for(int j = 0; j < BlockchainCount; ++j)--(*blocks[j]);

}

double Miner::getTotalValue() const{
	double value = 0;
	for(int i = 0; i < BlockchainCount; ++i){
		value += blocks[i]->getTotalValue();
	}
	return value;
}

long Miner::getBlockchainCount() const{
	return BlockchainCount;
}

Blockchain* Miner::operator[](int id) const{
	for(int i = 0; i < BlockchainCount; ++i){
		if(blocks[i]->getID() == id)return blocks[i];
	}
	return nullptr;
}

bool Miner::softFork(int blockchainID){
	Blockchain *softedchain = (*this)[blockchainID];
	if(softedchain == nullptr)return false;
	Koin *last = softedchain->getHead();
	if(last == nullptr){
		blocks.push_back(new Blockchain(this->getNextAvailableBlockchainID(),nullptr));
		(this->BlockchainCount)++;
		return true;
	}
	while(last->getNext()) last = last->getNext();

	blocks.push_back(new Blockchain(this->getNextAvailableBlockchainID(),last));
	blocks[BlockchainCount]->softfork();
	blocks[blockchainID]->softforkchain(blocks[BlockchainCount]);
	(this->BlockchainCount)++;
	return true;
}

bool Miner::hardFork(int blockchainID){
	Blockchain *softedchain = (*this)[blockchainID];
	if(softedchain == nullptr)return false;
	Koin *last = softedchain->getHead();
	if(last == nullptr)return false;
	while(last->getNext()) last = last->getNext();
	Blockchain *newchain = new Blockchain(this->getNextAvailableBlockchainID(),new Koin(last->getValue()));
	blocks.push_back(newchain);
	(this->BlockchainCount)++;
	return true;
}

    std::ostream& operator<<(std::ostream& os, const Miner& miner){
        os << std::setprecision(Utilizer::koinPrintPrecision()) << std::fixed;
    	os << "-- BEGIN MINER --" << std::endl;
    	os << "Miner name: " << miner.name << std::endl;
    	os << "Blockchain count: " << miner.getBlockchainCount() << std::endl;
        os << "Total value: " << miner.getTotalValue() << std::endl << std::endl;
    	for(int i = 0; i < miner.getBlockchainCount(); ++i)os << *(miner.blocks[i]) << std::endl;

    	os << std::endl << "-- END MINER --"<< std::endl;
    	return os;
    }