//
// Created by Asaf Raudnitz on 16/06/2018.
//

#ifndef HW5_TROLL_H
#define HW5_TROLL_H

#include "player.h"

class Troll : public player {
    int max_life;
public:
    explicit Troll()= default;
    explicit Troll(string const& name, const Weapon& weapon, const int max_life);
    Troll(const Troll& troll) override ;
    ~Troll();
    void makeStep() override ;
    void addLife() override ;
};

#endif //HW5_TROLL_H
