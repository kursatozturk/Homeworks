#include "Koin.h"

using namespace std;	
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

bool aredoubleseq(const double lhs,const double rhs){
	double diff(lhs - rhs);
	if(diff < 0) diff *= -1;
	return (diff < Utilizer::doubleSensitivity());
}
	

Koin::Koin(double value){
	this->value = value;
	this->next = nullptr;

}
Koin::Koin(const Koin& rhs){
	this->value = rhs.value;
	this->next = rhs.next;
}


Koin& Koin::operator=(const Koin& rhs){

	this->value = rhs.value;
	Koin *p(rhs.next);
	Koin *q(this);
	while(p){
		if(q->next!=nullptr)q->next->value = p->value;
		else q->next = new Koin(p->value);
		q = q->next;
		p = p->next;
	}
	return *this;
}

double Koin::getValue() const{
	return this->value;
}


Koin* Koin::getNext() const{
	return this->next;
}


void Koin::setNext(Koin *next){
	this->next  = next;
}

bool Koin::operator==(const Koin& rhs)const{
	if(!this)return false;
	if(rhs.next && this->next) return (aredoubleseq(rhs.value, this->value) && (*this->next == *rhs.next));
	else if(rhs.next || this -> next) return false;
	else return aredoubleseq(rhs.value, this->value);
}

bool Koin::operator!=(const Koin& rhs)const{
	return !(*this == rhs);
}

Koin& Koin::operator*=(int multiplier){
	if(this)this->value *= multiplier;
	return *this;

}

Koin& Koin::operator/=(int divisor){
	if(this)this->value /= divisor;
	return *this;
}

ostream& operator<<(ostream& os, const Koin& koin){
	const Koin *p(&koin);
	while(p){
		os << setprecision(Utilizer::koinPrintPrecision()) << fixed << p->value << "--";
		p = p->next;
	}
	os << "|";
	return os;
}






