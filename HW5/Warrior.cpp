//
// Created by Asaf Raudnitz on 16/06/2018.
//

#include "Warrior.h"


explicit Warrior::Warrior(string const& name
        , const Weapon& weapon1, const bool mounted):name(name)
        ,weapon(),mounted(mounted){
    if(weapon.getTarget()== LEVEL){
        throw "imma shelo";
    }
    weapon=weapon1;
}
Warrior::Warrior(const Warrior& warrior):name(warrior.name)
        ,weapon(warrior.weapon),mounted(warrior.mounted){
}
void Warrior::makeStep(){
    if(mounted){
        for(int i=0; i<5; i++){
            Player::makeStep();
        }
    }
    Player::makeStep();
}
