#include "Pacifist.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Armor Pacifist::getArmor() const{
	return this->armor;
}
Weapon Pacifist::getWeapon() const{
	return this->weapon;
}

std::vector<Move> Pacifist::getPriorityList() const{
	return this->PriorityList;
}

const std::string Pacifist::getFullName() const{
	return this->getName()+this->getBoardID();
}

Color::Code Pacifist::getColorID() const {
	return Color::FG_GREEN;
}