//
// Created by Asaf Raudnitz on 16/06/2018.
//

#include "Troll.h"



explicit Troll::Troll(string const& name
        , const Weapon& weapon, const int max_life):name(name)
        ,weapon(weapon),max_life(max_life){
}
Troll::Troll(const Troll& troll):name(troll.name),weapon(troll.weapon)
            ,max_life(troll.max_life){
}
void Troll::addLife(){
    if(life<max_life){
        Player::addLife();
    }
}
void Troll::makeStep(){
    for(int i=0; i<2; i++){
        Player::makeStep();
    }
    addLife();
}