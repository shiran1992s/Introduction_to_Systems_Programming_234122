//
// Created by pc on 5/30/2018.
//

#include "Weapon.h"
using std::cout;
using std::cin;
using std::cerr;
using std::endl;


Weapon::Weapon(const char* name, Target target2,int hit_strength2):target1(target2),hit_strength(hit_strength2){
    this->name=new char [strlen(name)+1];
    strcpy(this->name,name);
}
ostream& operator<<(ostream& os,const Weapon& weapon1){
    return os <<"weapon name:" << weapon1.name << "," << "weapon value:"<< weapon1.getValue()<<"}}";
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
    if(weapon1.getValue()!= weapon2.getValue()){
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
Weapon::Weapon(const Weapon& weapon):target1(weapon.target1),hit_strength(weapon.hit_strength){
    if(&weapon){
        name=new char [strlen(weapon.name)+1];
        strcpy(name,weapon.name);
    }
}
Weapon& Weapon::operator=(const Weapon& weapon1){
    if(this==&weapon1){
        return *this;
    }
    delete[] name;
    name=new char[strlen(weapon1.name)+1];
    strcpy(name,weapon1.name);
    target1=weapon1.target1;
    hit_strength=weapon1.hit_strength;
    return *this;
}
Weapon::~Weapon(){
   delete[] name;
}