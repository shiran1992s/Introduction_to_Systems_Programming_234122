//
// Created by Asaf Raudnitz on 16/06/2018.
//

#ifndef HW5_WARRIOR_H
#define HW5_WARRIOR_H

#include "player.h"

class Warrior : public player {
    bool mounted;
public:
    explicit Warrior()= default;
    explicit Warrior(string const& name, const Weapon& weapon1, const bool mounted);
    Warrior(const Warrior& warrior) override ;
    ~Warrior();
    void makeStep() override ;
};

#endif //HW5_WARRIOR_H
