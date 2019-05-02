//
// Created by pc on 5/30/2018.
//

#include "Weapon.h"
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

Weapon::Weapon(const string name1, Target target2,int hit_strength2):name(name1)
        ,target1(target2),hit_strength(hit_strength2){
}
ostream& operator<<(ostream& os,const Weapon& weapon1){
    return os <<"weapon name: " << weapon1.name << ", " << "weapon value:"<< weapon1.getValue()<<"}}"<<",";
}
Target Weapon::getTarget()const{

    return target1;
}
int Weapon::getHitStrength()const{
    return hit_strength;
}
int Weapon::getValue()const {
    if(target1==LEVEL){
        return getHitStrength();
    }else if(target1==STRENGTH){
        return (2*getHitStrength());
    }
    return (3*getHitStrength());
}
bool operator==(const Weapon& weapon1,const Weapon& weapon2)  {
    if(weapon1.getValue()== weapon2.getValue()){
        return true;
    }
    return false;
}
bool operator!=(const Weapon& weapon1,const Weapon& weapon2){
    if (!(weapon1 == weapon2)){
        return true;
    }
    return false;
}
bool operator<(const Weapon& weapon1,const Weapon& weapon2){
    if(weapon1.getValue() < weapon2.getValue()){
        return true;
    }
    return false;
}
bool operator>(const Weapon& weapon1,const Weapon& weapon2){
    if(weapon1.getValue() > weapon2.getValue()){
        return true;
    }
    return false;
}
Weapon::Weapon(const Weapon& weapon):name(name)
        ,target1(weapon.target1),hit_strength(weapon.hit_strength){
}
Weapon& Weapon::operator=(const Weapon& weapon1){
    if(this==&weapon1){
        return *this;
    }
    name = weapon1.name;
    target1=weapon1.target1;
    hit_strength=weapon1.hit_strength;
    return *this;
}
