#ifndef HW4_AMBUSHER_H
#define HW4_AMBUSHER_H


#include "Player.h"

class Ambusher : public Player {
public:
    Ambusher(uint id, int x, int y):Player(id,x,y) {
    	HP = 100; 
    	armor = NOARMOR; 
    	weapon = SEMIAUTO;
    	PriorityList= {ATTACK};
    	setName("Ambusher"); 
    }
    Armor getArmor() const;
    Weapon getWeapon() const;
    std::vector<Move> getPriorityList() const;
    const std::string getFullName() const;
    Color::Code getColorID() const;
    ~Ambusher() = default;

    // Name     : Ambusher
    // Priority : { ATTACK }

    // Armor    : NOARMOR
    // Weapon   : SEMIAUTO
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW
};


#endif //HW4_AMBUSHER_H
