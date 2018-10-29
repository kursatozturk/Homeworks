#include "Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/


Board::Board(uint boardSize, std::vector<Player *> *players):boardSize(boardSize){
     this->players = players;
     stormDamageForRound = 0;
     stormWidthForRound = 0;
}

Board::~Board(){
}


uint Board::getSize() const{
	return boardSize;
}


bool Board::isCoordInBoard(const Coordinate& coord) const{
	return (coord.x < boardSize && coord.x >= 0 && coord.y < boardSize && coord.y >= 0); 
}


bool Board::isStormInCoord(const Coordinate &coord) const{
     return (coord.x >= 0 && coord.x < stormWidthForRound) || (coord.x >= boardSize - stormWidthForRound && coord.x < boardSize) || (coord.y >= 0 && coord.y < stormWidthForRound) || (coord.y >= boardSize - stormWidthForRound && coord.y < boardSize);
}

void Board::updateStorm(uint currentRound){
     if(boardSize >= 2 * Entity::stormWidthForRound(currentRound))
          stormWidthForRound = Entity::stormWidthForRound(currentRound);
     stormDamageForRound = Entity::stormDamageForRound(currentRound);
}

bool Board::isCoordHill(const Coordinate& coord) const{
	return (coord.x == (boardSize - 1)/2 && coord.y == (boardSize - 1)/2);
}


Player* Board::operator[](const Coordinate& coord) const{
	std::vector<Player *>::iterator it(players->begin());
	for(; it != players -> end(); ++it){
		if ((*it)->getCoord() == coord)return *it;
	}
	return nullptr;
}

Coordinate Board::calculateCoordWithMove(Move move, const Coordinate &coord) const{
	Coordinate newCoord = coord + move;
	if(!isCoordInBoard(newCoord))return coord;
	if((*this)[newCoord])return coord;
	Player *currPlayer = (*this)[coord];
	if(currPlayer == nullptr)return coord;
	return newCoord;
}


    /**
     * Find the visible coordinates from given coordinate.
     *
     * Explanation: The immediate UP/DOWN/LEFT/RIGHT tiles must be calculated.
     *
     * There could be max of 4 tiles, and min of 2 tiles (on corners).
     * Every found coordinate must be in the board limits.
     *
     * If the given coordinate is not in board, return a vector with size = 0. Order does NOT matter.
     *
     * Example:
     *
     * 01----
     * 02HH--
     * ------
     *
     * visibleCoordsFromCoord(Coordinate(0, 0)) == { (1, 0), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(1, 1)) == { (1, 0), (2, 1), (1, 2), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(-1, 0)) == { }
     *
     * @param coord The coordinate to search.
     * @return All coordinates visible.
     */
std::vector<Coordinate> Board::visibleCoordsFromCoord(const Coordinate &coord) const{
std::vector<Move> moves = {UP, DOWN, LEFT, RIGHT};
std::vector<Coordinate> visibleCoordinates;
if(!isCoordInBoard(coord))return visibleCoordinates;
for(std::vector<Move>::iterator it = moves.begin(); it != moves.end(); ++it){
	Coordinate isVisible(coord + *it);
	if(!isCoordInBoard(isVisible)) continue;
	if(coord != isVisible)visibleCoordinates.push_back(isVisible);
}
return visibleCoordinates;
}



const uint Board::stormDamage() const{
     return stormDamageForRound;
} 
const Coordinate Board::getHillCoord() const{
     return Coordinate((boardSize-1)/2,(boardSize-1)/2);
}
