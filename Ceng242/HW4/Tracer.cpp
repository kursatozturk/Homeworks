#include "Tracer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Armor Tracer::getArmor() const{
	return this->armor;
}
Weapon Tracer::getWeapon() const{
	return this->weapon;
}

std::vector<Move> Tracer::getPriorityList() const{
	return this->PriorityList;
}

const std::string Tracer::getFullName() const{
	return this->getName()+this->getBoardID();
}

Color::Code Tracer::getColorID() const {
	return Color::FG_YELLOW;
}