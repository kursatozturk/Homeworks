#include "Ambusher.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Armor Ambusher::getArmor() const{
	return this->armor;
}
Weapon Ambusher::getWeapon() const{
	return this->weapon;
}

std::vector<Move> Ambusher::getPriorityList() const{
	return this->PriorityList;
}

const std::string Ambusher::getFullName() const{
	return this->getName()+this->getBoardID();
}

Color::Code Ambusher::getColorID() const {
	return Color::FG_BLUE;
}