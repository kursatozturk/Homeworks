#ifndef HW4_PACIFIST_H
#define HW4_PACIFIST_H


#include "Player.h"

class Pacifist : public Player {
public:
    Pacifist(uint id, int x, int y):Player(id,x,y){
    	HP = 100;
    	armor = METAL;
    	weapon = NOWEAPON;
    	PriorityList = { UP, LEFT, DOWN, RIGHT };
    	setName("Pacifist");
    }
    Armor getArmor() const;
    Weapon getWeapon() const;
    std::vector<Move> getPriorityList() const;
    const std::string getFullName() const;
	Color::Code getColorID() const;
	~Pacifist() = default;
    // Name     : Pacifist
    // Priority : { UP, LEFT, DOWN, RIGHT }

    // Armor    : METAL
    // Weapon   : NOWEAPON
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW
    
};


#endif //HW4_PACIFIST_H
