//
// Created by Asaf Raudnitz on 16/06/2018.
//

#ifndef HW5_WIZARD_H
#define HW5_WIZARD_H

#include "player.h"

class Wizard : public player {
    int range;
public:
    explicit Wizard()= default;
    explicit Wizard(string const& name, const Weapon& weapon1, const int range);
    Wizard(const Wizard& wizard) override ;
    ~Wizard();
    bool canAttack(const Player* player1) override ;
};


#endif //HW5_WIZARD_H
