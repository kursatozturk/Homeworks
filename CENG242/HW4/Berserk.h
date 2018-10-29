#ifndef HW4_BERSERK_H
#define HW4_BERSERK_H


#include "Player.h"

class Berserk : public Player {
public:
    Berserk(uint id, int x, int y):Player(id,x,y){
    	HP = 100;
    	armor = WOODEN;
    	weapon = PISTOL;
    	PriorityList = { ATTACK, UP, LEFT, DOWN, RIGHT };
    	setName("Berserk");
    }
    Armor getArmor() const;
    Weapon getWeapon() const;
    std::vector<Move> getPriorityList() const;
    const std::string getFullName() const;
    Color::Code getColorID() const;
    ~Berserk() = default;    

    // Name     : Berserk
    // Priority : { ATTACK, UP, LEFT, DOWN, RIGHT }

    // Armor    : WOODEN
    // Weapon   : PISTOL
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW
    
};


#endif //HW4_BERSERK_H
