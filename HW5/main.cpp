#include <iostream>
#include "mtm_array.h"

#include "Game.h"
using std::cout;
using std::cin;
using std::cerr;
using std::endl;



void simpleTest() {
    Game game = Game(5);
    assert(game.addPlayer("Linoy", "lazer gun", LEVEL, 5) == SUCCESS);
    assert(game.addPlayer("Denis", "light saver", LIFE, 7) == SUCCESS);
    assert(game.addPlayer("Gal", "pirate sword", STRENGTH, 6) == SUCCESS);
    cout << game << endl;
    assert(game.fight("Linoy", "Denis") == SUCCESS);
    cout << game << endl;
    assert(game.addPlayer("Alon", "lazer gun", STRENGTH, 5) == SUCCESS);
    assert(game.addStrength("Alon", 10) == SUCCESS);
    assert(game.fight("Alon", "Gal") == SUCCESS);
    cout << game << endl;
    assert(game.removeAllPlayersWithWeakWeapon(13) == true);
    cout << game << endl;


}


int main() {
    simpleTest();
}