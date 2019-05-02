//
// Created by pc on 5/30/2018.
//

#ifndef HW4SOLUTION_GAME_H
#define HW4SOLUTION_GAME_H

#include <iostream>
#include "Player.h"
#include <cassert>

/** Type used for returning result codes from Game functions */


typedef enum GameResult_t {
    SUCCESS,
    GAME_FULL,
    NAME_ALREADY_EXISTS,
    NAME_DOES_NOT_EXIST,
    INVALID_PARAM,
    FIGHT_FAILED
} GameStatus;


/** Type for defining the Game
*
* @max_players - int param for the max number of players in the Game
* @players -an array of pointers to Player types
* @players_num- int param for the current number of players in the Game
*
*
* The following functions are available:
*   Player defult C'tor		- defult constructor for Game
*
*   Player C'tor            - constructor for Game with inputs
*
*   Player D'tor		    - Destroys a Game ( releases the pointers of players and the array)
*
*   Player Copy constructor	- Copies a Game and creates new one
*
*   addPlayer		        - add a player to the game if possible
*
*   nextLevel	            - increases the level of a specific player in the game by 1
*
*   makeStep		        - increases the place of a specific player in the game by 1
*
*   addLife  	            - increases the life of a specific player in the game by 1
*
*   addStrength		        - increases the strength of a specific player in the game by the input
*
*   removeAllPlayersWithWeakWeapon - removes all the players in the game with a weaker weapon than the input
*
*   fight	       	        - make a fight between 2 players in the game
*
*	operator[]		        - [] operator for the Players array in the Game type
*
*   operator[]		        - [] operator for the Players array in the Game type ( const version)
*
*   operator=	            - making assignment to a game according to the details of the input game
*
*   SortPlayers		        - sorting the array of players according to their names (lexicographic)
*
*   deletePlayer		    - removes a specific player in the game
*
*	operator<<		        - output operator , prints the neccesery details
*
*
**/

class Game {
    int max_players;
    Player** players;
    int players_num;

public:
/**
* Game(): creates a defult game type
*
* don't get an input
* @return
* 	a game type with garbage values
**/

    explicit Game()= default;
/**
* Game(int max_players): creates a game type according to the inputs
*
* @max_players - the number of max players in the game
* @return
* 	a game type with the input values
**/
   explicit Game(int max_players);
/**
*  ~Game(): delete the game (destructor)
*
* don't get input (except "this")
* @return
* 	don't return anything
**/

    ~Game();
/**
* Game(const Game& game): copy the input game values and creates a new game with the same values
*
* @game - a reference to an existing game type
* @return
* 	a game type with the input game values
**/
    Game(const Game& game);
/**
* addPlayer(const char* player_name,const char* weapon_name,Target target1,int hit_strength): adds a player with the
* inputs
*
* @player_name - the name of the wanted player
* @weapon_name - the name of the wanted weapon
* @target1 - the kind of target the weapon is affective against
* @hit_strength - the amount of hit strength
* @return
* 	NAME_ALREADY_EXISTS - the player is already in the game
 * 	GAME_FULL - if the game already has the maximum players
 * 	SUCCESS - if the adding action is a success
 * 	NAME_DOES_NOT_EXIST - if the adding action failed
**/
    GameStatus addPlayer(const char* player_name,const char* weapon_name,Target target1,int hit_strength);
/**
* nextLevel(const char* player_name):  promotes the player with the input name to the next level
* @player_name - the name of the wanted player
* @return
 * 	SUCCESS - if the promoting action is a success
 * 	NAME_DOES_NOT_EXIST - there is no player with that name
**/


    GameStatus nextLevel(const char* player_name);
/**
* makeStep(const char* player_name): promotes the player with the input name to the next place
*
* @player_name - the name of the wanted player
* @return
 * 	SUCCESS - if the promoting action is a success
 * 	NAME_DOES_NOT_EXIST - there is no player with that name
**/
    GameStatus makeStep(const char* player_name);
/**
* addLife(const char* player_name): adds 1 to the life of the player with the input name
*
* @player_name - the name of the wanted player
* @return
 * 	SUCCESS - if the adding action is a success
 * 	NAME_DOES_NOT_EXIST - there is no player with that name
**/

    GameStatus addLife(const char* player_name);
/**
* addStrength(const char* player_name, int strength_to_add): adds the input to the strength of the player
* with the wanted name
*
* @player_name - the name of the wanted player
* @strength_to_add - the amount of strength
* @return
 * 	INVALID_PARAM - a negetive amount of strength
 * 	SUCCESS - if the adding action is a success
 * 	NAME_DOES_NOT_EXIST - there is no player with that name
**/

    GameStatus addStrength(const char* player_name, int strength_to_add);
/**
* removeAllPlayersWithWeakWeapon(int weapon_strength): removes all the players in the game with a
* weapon's strength that is lower then the input
*
* @weapon_min_strength - the minimum strength
*
* @return
* 	TRUE - if there is players that were removed
* 	FALSE - if there isn't players that were removed
**/

    bool removeAllPlayersWithWeakWeapon(int weapon_strength);
/**
* fight(const char* player_name1, const char* player_name2): make a fight between the 2 input players in the game
*
* @player_name1 - the name of the first wanted player
* @player_name2 - the name of the second wanted player
* @return
 * 	FIGHT_FAILED - if the fight failed for some reason
 * 	SUCCESS - if the fight action is a success
 * 	NAME_DOES_NOT_EXIST - there is no player with a input name
**/

    GameStatus fight(const char* player_name1, const char* player_name2);

/**
* operator<<(ostream& os,const Game& game1): output operator , prints the neccesery details
*
* @os - the wanted output source
* @game1 - a reference to an existing game type
* @return
* 	prints to the wanted output
**/

    friend ostream& operator<<(ostream& os,const Game& game1);
/**
* operator=(const Game& game1): making assignment to a game according to the details of the input game
*
* @game1 - a reference to an existing game type
*
* @return
* 	the game after the assignment
**/
    Game& operator=(const Game& game1) ;
};

/**
* SortPlayers(Player** players ,int players_n): sorting an array of pointers to players according
* to their names (lexicographic)
*
* @players - an array of pointers to players
* @players_n - number of players in the array
* @return
* 	void function - don't return anything
**/
void SortPlayers(Player** players ,int players_n);

#endif //HW4SOLUTION_GAME_H




