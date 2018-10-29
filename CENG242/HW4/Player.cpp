#include "Player.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

uint Player::getID() const{
	return this->id;
}

const Coordinate& Player::getCoord() const{
	return this->coordinate;
}
int Player::getHP() const{
	return this->HP;
}
std::string Player::getBoardID() const{
	std::stringstream s;
	std::string BoardID("");
	if(this->id < 10){
		s << "0";}
	s << this->getID();
	s >> BoardID;
	s.str(std::string());
	return BoardID;
}

bool Player::isDead() const{
	return (this->getHP() <= 0);
}
 
void Player::executeMove(Move move){
	this->coordinate = this->coordinate + move;
	switch(move){
	        case NOOP:
            case ATTACK:
                break;
            case UP:
				std::cout << this->getFullName() << "(" << this->getHP() << ") moved ";
                std::cout << "UP." << std::endl;
                break;
            case DOWN:
				std::cout << this->getFullName() << "(" << this->getHP() << ") moved ";
                std::cout << "DOWN." << std::endl;
                break;
            case LEFT:
				std::cout << this->getFullName() << "(" << this->getHP() << ") moved ";
                std::cout << "LEFT." << std::endl;
                break;
            case RIGHT:
				std::cout << this->getFullName() << "(" << this->getHP() << ") moved ";
                std::cout << "RIGHT." << std::endl;
                break;
	}

}

bool Player::attackTo(Player *player){
	if(player == this)return false;
	uint damageDone = std::max(Entity::damageForWeapon(weapon) - Entity::damageReductionForArmor(player->armor), 0);
	std::cout << this->getFullName() << "(" << this->getHP() << ") ATTACKED " << player->getFullName() << "(" << player->getHP() << ")! (-" << damageDone << ")" << std::endl;
	player->HP -= damageDone;
 	return player->isDead();
}

void Player::setName(const std::string &name){
	PlayerName = name;
}
const std::string Player::getName() const{
	return this->PlayerName;
}

Player::~Player(){}

void Player::getStormDamage(uint stormDamage){
	HP-=stormDamage;
}