#include "Dummy.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Armor Dummy::getArmor() const{
	return this->armor;
}
Weapon Dummy::getWeapon() const{
	return this->weapon;
}

std::vector<Move> Dummy::getPriorityList() const{
	return this->PriorityList;
}

const std::string Dummy::getFullName() const{
	return this->getName()+this->getBoardID();
}
