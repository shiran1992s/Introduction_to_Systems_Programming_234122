//
// Created by pc on 5/30/2018.
//

#include "Game.h"
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;


template<class T, int SIZE>
void SortPlayers(vector<Player*> players ,int players_n){
    bool swapped = true;
    int j = 0;
    Player* tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < players_n - j; i++) {
            if (*players[i] > *players[i + 1]) {
                tmp = players[i];
                players[i] = players[i + 1];
                players[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

Game::Game(int max_players2):max_players(max_players2),players(max_players2,nullptr),players_num(0){
}
GameStatus Game::addPlayer(const string player_name,const string weapon_name,Target target1,int hit_strength) {
    int i = 0;
    if(players_num >= max_players){
        return GAME_FULL;
    }
    for (; i < players_num; i++) {
        if (players[i]->isPlayer(player_name)) {
            return NAME_ALREADY_EXISTS;
        }
    }
    Weapon tmp_weapon(weapon_name,target1,hit_strength);
    Player tmp_pl_pointer(player_name,tmp_weapon);
    players[players_num] = new Player();
    (*players[players_num])=tmp_pl_pointer;
    players_num++;
    return SUCCESS;
}
GameStatus Game::nextLevel(const string player_name){
    for(int i=0;i<players_num;i++){
        if(players[i]->isPlayer(player_name)){
            players[i]->nextLevel();
            return SUCCESS;
        }
    }
    return NAME_DOES_NOT_EXIST;
}
GameStatus Game::makeStep(const string player_name){
    for(int i=0;i<players_num;i++){
        if(players[i]->isPlayer(player_name)){
            players[i]->makeStep();
            return SUCCESS;
        }
    }
    return NAME_DOES_NOT_EXIST;
}
GameStatus Game::addLife(const string player_name){
    for(int i=0;i<players_num;i++){
        if(players[i]->isPlayer(player_name)){
            players[i]->addLife();
            return SUCCESS;
        }
    }
    return NAME_DOES_NOT_EXIST;
}
GameStatus Game::addStrength(const string player_name, int strength_to_add){
    if(strength_to_add<0){
        return INVALID_PARAM;
    }
    for(int i=0;i<players_num;i++){
        if(players[i]->isPlayer(player_name)){
            players[i]->addStrength(strength_to_add);
            return SUCCESS;
        }
    }
    return NAME_DOES_NOT_EXIST;
}
bool Game::removeAllPlayersWithWeakWeapon(int weapon_strength){
    int counter(0);
    for(int i=0;i<players_num;i++){
        if(players[i]->weaponIsWeak(weapon_strength)){
            if(i!=players_num-1){
                *players[i]=*players[players_num-1];
                i=-1;
                players_num--;
                counter++;
            }else {
                delete players[i];
                players[i]= new Player();
                players_num--;
                counter++;
            }
        }
    }
    return (counter>0);
}
GameStatus Game::fight(const string player_name1, const string player_name2){
    int player1_flag(-1),player2_flag(-1);
    for(int i=0;i<players_num;i++) {
        if (players[i]->isPlayer(player_name1)) {
            player1_flag = i;
        }
        if (players[i]->isPlayer(player_name2)) {
            player2_flag = i;
        }
    }
    if(player1_flag!=-1 && player2_flag!=-1) {
        Player &player2_ref = *(players[player2_flag]);
        if (players[player1_flag]->fight(player2_ref)) {
            if (!(players[player1_flag]->isAlive())) {
                if(player1_flag!=players_num-1){
                    *players[player1_flag]=*players[players_num-1];
                    players_num--;
                }else{
                    delete players[player1_flag];
                    players[player1_flag]= new Player();
                    players_num--;
                }
            }
            if (!(players[player2_flag]->isAlive())) {
                if(player1_flag!=players_num-1) {
                    *players[player2_flag] = *players[players_num - 1];
                    players_num--;
                }else{
                    delete players[player1_flag];
                    players[player1_flag]= new Player();
                    players_num--;
                }
            }
            return SUCCESS;
        } else {
            return FIGHT_FAILED;
        }
    }else{
        return NAME_DOES_NOT_EXIST;
    }
}

Game::Game(const Game& game):max_players(game.max_players),players(game.players)
        ,players_num(game.players_num){
    for (int i = 0; i < max_players; ++i) {
        this->players[i]= new Player();
    }
    for (int i = 0; i < players_num; ++i) {
       *this->players[i]=*(game.players[i]);
    }
}
Game& Game::operator=(const Game& game1){
    if(this==&game1){
        return *this;
    }
    for (int i = 0; i < max_players; ++i) {
       delete players[i];
    }
    delete[] players;
    max_players=game1.max_players;
    players_num=game1.players_num;
    players = new Player*[max_players];
    for(int i=0;i<max_players;i++){
        players[i]= new Player();
    }
    for (int i = 0; i < players_num; ++i) {
        *players[i]=*game1.players[i];
    }
    return *this;
}

ostream& operator<<(ostream& os,const Game& game1){
    Game* sorted_game = new Game(game1);
    SortPlayers(sorted_game->players,sorted_game->players_num);
    for (int i = 0; i <sorted_game->players_num ; i++) {
         os <<"player " <<i<<": {"<<*sorted_game->players[i]<<endl;
    }
    delete sorted_game;
    return os;
}

Game::~Game(){
    for (int i = 0; i < max_players; ++i) {
        delete players[i];
    }
    delete[] players;
}