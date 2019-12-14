#include "GameEngine.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
void GameEngine::sortPlayers(std::vector<Player *> *players){
	bool flag = true;
	if(players == nullptr) return;
	while(flag){
		flag = false;
		std::vector<Player *>::iterator it = players->begin();
		if(it == players->end()) return;
		for(std::vector<Player *>::iterator nextIt = it + 1; nextIt != players->end(); ++nextIt, ++it){
			if((*nextIt)->getID() < (*it)->getID()){
				Player *tmp = *nextIt;
				*nextIt = *it;
				*it = tmp;
				flag = true;
			}
		}
	}
}

GameEngine::GameEngine(uint boardSize, std::vector<Player *> *players):board(boardSize, players){
	currentRound = 0;
	myPlayers = players;
}
GameEngine::~GameEngine(){
	//delete the vector.
	for(auto p : *myPlayers) delete p;
	myPlayers->clear();
	delete myPlayers;
}

const Board& GameEngine::getBoard() const{
	return this->board;
}

Player* GameEngine::operator[](uint id) const{
	std::vector<Player *>::iterator it = this->myPlayers->begin();
	for(; it != this->myPlayers->end(); ++it){
		if ((*it)->getID() == id) return *it;
	}
	return nullptr;
}

bool GameEngine::isFinished() const{
	if(myPlayers->size() == 1){
		if(board.isCoordHill((*myPlayers)[0]->getCoord()))return true;
		else return false;
	}
	else if(myPlayers->size() == 0) return true;
	else return false;
}

void GameEngine::takeTurn(){
	currentRound++;
	std::cout << "-- START ROUND " << currentRound << " --" << std::endl;
	board.updateStorm(currentRound);
	sortPlayers(myPlayers);
	size_t size = 0; 
	while(size < myPlayers->size()){
		Player *players = (*myPlayers)[size];
		takeTurnForPlayer(players->getID());
		if(players == (*myPlayers)[size])size++;
	}

	std::cout << "-- END ROUND " << currentRound << " --" << std::endl;
}

Move GameEngine::takeTurnForPlayer(uint playerID){
	Player *player = (*this)[playerID];
	if(player == nullptr) return NOOP;
	std::vector<Move> PriorityList = player->getPriorityList();
	std::vector<Coordinate> visibleCoords = board.visibleCoordsFromCoord(player->getCoord());
	if(board.isStormInCoord(player->getCoord())){
		std::cout << player->getFullName() << "(" << player->getHP() << ") is STORMED! (-" << board.stormDamage() << ")" << std::endl;  
		player->getStormDamage(board.stormDamage());
	}
	if(player->isDead()){
		std::cout << player->getFullName() << "(" << player->getHP() << ") DIED." << std::endl;
		std::vector<Player *>::iterator DeathFinder = myPlayers->begin();
		for(; DeathFinder != myPlayers->end(); ++ DeathFinder)
			if((*DeathFinder) == player)break;
		delete player;
		myPlayers->erase(DeathFinder);
		return NOOP;
	}
	for(std::vector<Move>::iterator it = PriorityList.begin(); it != PriorityList.end(); ++it){
		switch(*it){
			case NOOP:
				return NOOP;
				break;
			case ATTACK:{
				std::vector<Player *> *canBeAttacked = new std::vector<Player *>();
				for(std::vector<Coordinate>::iterator itVis = visibleCoords.begin(); itVis != visibleCoords.end(); ++itVis){
					if(board[*itVis])canBeAttacked->push_back(board[*itVis]);
				}
				if(canBeAttacked->size() == 0){
					delete canBeAttacked;
					continue;
				}
				sortPlayers(canBeAttacked);
				Player *prior = (*canBeAttacked)[0];
				if(player->attackTo(prior)){
					std::cout << prior->getFullName() << "(" << prior->getHP() << ") DIED." << std::endl;	
					std::vector<Player *>::iterator DeathFinder = myPlayers->begin();
					for(; DeathFinder != myPlayers->end(); ++DeathFinder)
						if((*DeathFinder) == prior)
							break;
						
					delete prior;
					myPlayers->erase(DeathFinder);
				}
				delete canBeAttacked;
				return ATTACK;
				break;
			}
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:{
				if(board.calculateCoordWithMove(*it,player->getCoord()) != player->getCoord()){
					int newDistance = board.getHillCoord() - board.calculateCoordWithMove(*it,player->getCoord());
					int oldDistance = board.getHillCoord() - player->getCoord();
					if(newDistance < oldDistance){
						player->executeMove(*it);
						return *it;
					}
					else continue;
				}
				else continue;
				break;
			}
		}
	}
}

Player* GameEngine::getWinnerPlayer() const{
	if(isFinished()){
		if(myPlayers->size() == 1){
			return (*myPlayers)[0];
		}
	}
	std::cout << "hela vela velvela" << std::endl;
	return nullptr;
}