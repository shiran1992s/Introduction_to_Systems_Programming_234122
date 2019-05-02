//
// Created by Asaf Raudnitz on 16/06/2018.
//

#include "Wizard.h"



explicit Wizard::Wizard(string const& name
        , const Weapon& weapon1, const int range):name(name)
        ,weapon(),range(range){
    if(weapon.getTarget() == LIFE){
        throw "imma shelo";
    }
    weapon=weapon1;
}
Wizard::Wizard(const Wizard& wizard):name(wizard.name)
        ,weapon(wizard.weapon),range(wizard.range){
}
bool Wizard::canAttack(const Player* player1){
    if(range>=Player::distance(this,player1)){
        if(!Player::distance(this,player1)){
            return false;
        }
        return true;
    }
    return false;
}