//
// Created by pc on 5/30/2018.
//
#include "Player.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;



Player::Player(const char* name2, const Weapon& weapon2):weapon1(weapon2){
   name=new char [strlen(name2)+1];
   strcpy(name,name2);
   level=1;
   life=1;
   strength=1;
   place=0;
}
Player::Player(const Player& player):level(player.level),life(player.life),strength(player.strength)
        ,weapon1(player.weapon1),place(player.place) {
    if (&player) {
        name = new char[strlen(player.name) + 1];
        strcpy(name, player.name);
    }
}

ostream& operator<<(ostream& os,const Player& player1){
    return os <<"player name:" << player1.name << ',' << "weapon: {"<< player1.weapon1;
}
void Player::nextLevel(){
    level+=1;
}
bool Player::isPlayer(const char* playername)const{
    if(!this){
        return false;
    }
    if(strcmp(name,playername)==0){
        return true;
    }
    return false;
}
void Player::makeStep(){
    if(!this){
        return;
    }
    place+=1;
}
void Player::addLife(){
    if(!this){
        return;
    }
    life+=1;
}
void Player::addStrength(int strength_to_add){
    if(!this){
        return;
    }
    strength+=strength_to_add;
}
bool Player::isAlive() const{
    if(!this){
        return false;
    }
    return strength && life && level;
}
bool Player::weaponIsWeak(int weapon_min_strength)const{
    if(!this){
        return false;
    }
    if(weapon1.getValue() < weapon_min_strength){
        return true;
    }
    return false;
}
bool Player::fight(Player& player1){
    if(!this || !&player1){
        return false;
    }
    if(place==player1.place){
        if(operator>(weapon1,player1.weapon1)){
            if(weapon1.getTarget()==LEVEL){
                player1.level-=weapon1.getHitStrength();
                if(player1.level<0){
                    player1.level=0;
                }
                return true;
            }else if(weapon1.getTarget()==LIFE){
                player1.life-=weapon1.getHitStrength();
                if(player1.life<0){
                    player1.life=0;
                }
                return true;
            }else{
                player1.strength-=weapon1.getHitStrength();
                if(player1.strength<0) {
                    player1.strength = 0;
                }
                return true;
            }
        }else if(operator<(weapon1,player1.weapon1)){
            if(player1.weapon1.getTarget()==LEVEL){
                level-=player1.weapon1.getHitStrength();
                if(level<0){
                    level=0;
                }
                return true;
            }else if(player1.weapon1.getTarget()==LIFE){
                life-=player1.weapon1.getHitStrength();
                if(life<0){
                    life=0;
                }
                return true;
            }else {
                strength -= player1.weapon1.getHitStrength();
                if (strength < 0) {
                    strength = 0;
                }
                return true;
            }
        }
        return false;
    }
    return false;
}

bool operator<(const Player& player1,const Player& player2) {
    if(strcmp(player1.name,player2.name)<0){
        return true;
    }
    return false;
}
bool operator>(const Player& player1,const Player& player2) {
    if(strcmp(player1.name,player2.name)>0){
        return true;
    }
    return false;
}
Player& Player::operator=(const Player& player1){
    if(this==&player1){
        return *this;
    }
    delete[] name;
    name=new char[strlen(player1.name)+1];
    strcpy(name,player1.name);
    level=player1.level;
    life=player1.life;
    strength=player1.strength;
    weapon1=player1.weapon1;
    place=player1.place;
    return *this;
}
Player::~Player(){
    delete[] name;
}