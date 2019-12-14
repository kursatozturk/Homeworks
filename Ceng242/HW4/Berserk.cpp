#include "Berserk.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Armor Berserk::getArmor() const{
	return this->armor;
}
Weapon Berserk::getWeapon() const{
	return this->weapon;
}

std::vector<Move> Berserk::getPriorityList() const{
	return this->PriorityList;
}

const std::string Berserk::getFullName() const{
	return this->getName()+this->getBoardID();
}

Color::Code Berserk::getColorID() const {
	return Color::FG_RED;
}