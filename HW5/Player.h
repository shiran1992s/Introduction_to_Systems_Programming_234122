//
// Created by pc on 5/30/2018.
//

#ifndef HW4SOLUTION_PLAYER_H
#define HW4SOLUTION_PLAYER_H


#include "Weapon.h"
#include <iostream>
#include <ostream>


/** Type for defining the Player
*
* @name - string for the name of the player
* @level -int param for the level of the player
* @life- int param for the life of the player
* @strength -int param for the strength of the player
* @weapon1- Weapon param containing the weapon of the player
* @place -int param for the place of the player
*
*
* The following functions are available:
*   Player defult C'tor		- defult constructor for player
*
*   Player C'tor            - constructor for player with inputs
*
*   Player D'tor		    - Destroys a player ( releases the name pointer)
*
*   Player Copy constructor	- Copies a player and creates new one
*
*   nextLevel		        - increases the level by 1
*
*   isPlayer	            - returns weather or not the player's name is equal to the input name
*
*   makeStep		        - increases the place by 1
*
*   addLife  	            - increases the life by 1
*
*   addStrength		        - increases the strength by the input
*
*   isAlive	                - returns weather or not the player's life/level/strength equals to 0 ( one of them).
*
*   weaponIsWeak	       	- returns weather or not the player's weapon strength is lower then the input .
*
*	fight		            - make a fight between 2 players
*
*   operator<		        - comparing between 2 players according to their names (lexicographic)
*
*   operator>	            - comparing between 2 players according to their names (lexicographic)
*
*   operator=		        - making assignment to a player according to the details of the input player
*
*	operator<<		        - output operator , prints the neccesery details
*
*
**/
class Player {
    string name;
    int level;
    int life;
    int strength;
    Weapon weapon1;
    int place;
protected:
    int distance(const Player* player1,const Player* player2);
    bool canAttack(const Player* player1);
public:
/**
* Player(): creates a defult player type
*
* don't get an input
* @return
* 	a Player type with garbage values
**/

    explicit Player()= default;
/**
* Player(const char* name, const Weapon& weapon1): creates a player type according to the inputs
*
* @name - a string that contains the name of the player
* @weapon1 - a reference to a weapon type
* @return
* 	a Player type with the input values - name + weapon
**/

     explicit Player(const string name, const Weapon& weapon1);
/**
* Player(const Player& player): copy the input player values and creates a new player with the same values
*
* @player - a reference to an existing Player type
* @return
* 	a Player type with the input player values - name + weapon
**/

    Player(const Player& player);
///**
//*  ~Player(): delete the player (destructor)
//*
//* don't get input (except "this")
//* @return
//* 	don't return anything
//**/
//
//    ~Player();
/**
* nextLevel(): adds 1 to the level of the player
*
* don't get input (except "this")
* @return
* 	void function - don't return anything
**/

    void nextLevel();
/**
* isPlayer(const char* playername): checks if the player's name is equal to the input name
*
* @playername - a string of a name
* @return
* 	TRUE - if this is the player's name
* 	FALSE - if this isn't the player's name
**/

    bool isPlayer(const string playername)const;
/**
* makeStep(): adds 1 to the place of the player
*
* don't get input (except "this")
* @return
* 	void function - don't return anything
**/

    virtual void makeStep();
/**
* addLife(): adds 1 to the life of the player
*
* don't get input (except "this")
* @return
* 	void function - don't return anything
**/

    void addLife() ;
/**
* getPlace(): returns the place of the player
*
* don't get input (except "this")
* @return
* 	the place (int)
**/

    const int getPlace() const ;
/**
* addStrength(int strength_to_add): adds the input to the strength of the player
*
* @strength_to_add - the amount of strength
* @return
* 	void function - don't return anything
**/

    void addStrength(int strength_to_add);
/**
* isAlive(): checks if the player is alive ( if the player's life/level/strength equals to 0 ( one of them) )
*
* don't get input (except "this")
* @return
* 	TRUE - if this is the player is alive
* 	FALSE - if this isn't the player isn't alive
**/

    bool isAlive() const;
/**
* weaponIsWeak(int weapon_min_strength): checks if the player's weapon strength is lower then the input
*
* @weapon_min_strength - the minimum strength
*
* @return
* 	TRUE - if the players weapon strength is lower than the input
* 	FALSE - if the players weapon strength isn't lower than the input
**/

    bool weaponIsWeak(int weapon_min_strength)const;
/**
* fight(Player& player1): make a fight between 2 players , the players values are affected according to the
* weapon strength
*
* @player1 - a reference to an existing Player type
* @return
* 	TRUE - if the fight is completed
* 	FALSE - if the fight failed
**/

    bool fight(Player& player1);
/**
* operator=(const Player& player1): making assignment to a player according to the details of the input player
*
* @player1 - a reference to an existing Player type
*
* @return
* 	the player after the assignment
**/

    Player& operator=(const Player& player1) ;

/**
* operator<<(ostream& os,const Player& player1): output operator , prints the neccesery details
*
* @os - the wanted output source
* @player1 - a reference to an existing Player type
* @return
* 	prints to the wanted output
**/

    friend ostream& operator<<(ostream& os,const Player& player1);
/**
* operator<(const Player& player1,const Player& player2): comparing between 2 players
* according to their names (lexicographic)
*
* @player1 - a reference to an existing Player type
* @player2 - a reference to an existing Player type
* @return
* 	TRUE - if player1's name is before player2's name  (lexicographic)
* 	FALSE - if player1's name isn't before player2's name  (lexicographic)
**/

    friend bool operator<(const Player& player1,const Player& player2);
/**
* operator>(const Player& player1,const Player& player2): comparing between 2 players
* according to their names (lexicographic)
*
* @player1 - a reference to an existing Player type
* @player2 - a reference to an existing Player type
* @return
* 	TRUE - if player1's name is before player2's name  (lexicographic)
* 	FALSE - if player1's name isn't before player2's name  (lexicographic)
**/

    friend bool operator>(const Player& player1,const Player& player2);
};

bool operator<(const Player& player1,const Player& player2);
bool operator>(const Player& player1,const Player& player2);

#endif //HW4SOLUTION_PLAYER_H









