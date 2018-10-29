#include "Blockchain.h"



Blockchain& Blockchain::operator=(Blockchain&& rhs) noexcept{
	if(this != &rhs) {
		if(this->head) {
			Koin *p(this->head);
			while(p){		
				Koin *tmp(p->getNext());
				delete p;
				p = tmp;
				}
				this->head = nullptr;
			}
		this->head = rhs.head;
		rhs.head = nullptr;
	}
	return *this;
}

Blockchain::~Blockchain(){
	if(this->softforked==true)return;
	Koin *q(this->head);
	while(q){
		Koin *tmp(q->getNext());
		delete q;
		q = tmp;
	}
	this->head = nullptr;
}

int Blockchain::getID() const{
	return this->id;
}

Koin* Blockchain::getHead() const{
	return this->head;
}

double Blockchain::getTotalValue() const{
	Koin *q(this->head);
	double res = 0;
	while(q){
		res += q->getValue();
		q = q->getNext();
	}
	return res;
}

long Blockchain::getChainLength() const{
	long length = 0;
	Koin *q(this->head);
	while(q){
		length++;
		q = q->getNext();
	}
	return length;
}

Blockchain& Blockchain::operator*=(int multiplier){
	if(this == nullptr) return *this;
	Koin *p(this->head);
	while(p){
		(*p) *= multiplier;
		p = p->getNext();
	}
	return *this;
}

Blockchain& Blockchain::operator/=(int divisor){
	Koin *p(this->head);
	while(p){
		(*p) /= divisor;
		p = p->getNext();
	}
	return *this;
}

void Blockchain::operator++(){
	double newKoinValue = Utilizer::fetchRandomValue();
	Koin *p(this->head);
	if(p == nullptr){
		this->head = new Koin(newKoinValue);
		return;
	}
	while(p->getNext())p = p->getNext();
	p->setNext(new Koin(newKoinValue));
}

void Blockchain::operator--(){
	Koin *p(this->head);
	if(p == nullptr)return;
	Blockchain* softblock = this->softforkchain();
	if(softblock){
			while(softblock->softforkchain())softblock = softblock->softforkchain();
			Koin *softed = softblock->head;
			if(softed->getNext()==nullptr)return;}
	
	if(p->getNext() == nullptr){
		delete p;
		this->head = nullptr;
		return;
	}
	while(p->getNext()->getNext())p = p->getNext();
	delete p->getNext();
	p->setNext(nullptr);
}

std::ostream& operator<<(std::ostream& os, const Blockchain& blockchain){
		os << std::setprecision(Utilizer::koinPrintPrecision()) << std::fixed;
		if(blockchain.head == nullptr)
			os << "Block " << blockchain.getID() << ": Empty.";
		else
    	os << "Block " << blockchain.getID() << ": " << *(blockchain.getHead()) << "(" << blockchain.getTotalValue() << ")";
    	return os;
    }

void Blockchain::softfork(){
	this->softforked = true;
}
Blockchain* Blockchain::softforkchain(){
	return softforkedchains;
}
void Blockchain::softforkchain(Blockchain* softed){
	this->softforkedchains = softed;
}








