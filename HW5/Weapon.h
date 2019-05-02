//
// Created by pc on 5/30/2018.
//

#ifndef HW4SOLUTION_WEAPON_H
#define HW4SOLUTION_WEAPON_H

#include "Weapon.h"
#include <iostream>
#include <ostream>
#include <cstring>
#include <string>
using std::ostream;
using std::string;


/** Type used for returning Target codes from Weapon functions */
typedef enum target{
    LEVEL,
    STRENGTH,
    LIFE
}Target;


/** Type for defining the Weapon
*
* @name - string for the name of the weapon
* @target1 -Enum param for the target of the weapon
* @hit_strength -int param for the hit strength of the weapon
*
*
* The following functions are available:
*   Weapon defult C'tor		- defult constructor for weapon
*
*   Weapon C'tor            - constructor for weapon with inputs
*
*   Weapon D'tor		    - Destroys a weapon ( releases the name pointer)
*
*   Weapon Copy constructor	- Copies a weapon and creates new one
*
*   getTarget		        - returns on which target the weapon is affective
*
*   getHitStrength	        - returns the weapon hit strength
*
*   getValue		        - returns the weapon damage points
*
*   operator<		        - comparing between 2 weapon according to their values
*
*   operator>	            - comparing between 2 weapon according to their values
*
*   operator=		        - making assignment to a weapon according to the details of the input weapon
*
*   operator!=		        - comparing between 2 weapon according to their values
*
*   operator==		        - comparing between 2 weapon according to their values
*
*	operator<<		        - output operator , prints the neccesery details
*
**/

class Weapon {
    string name;
    Target target1;
    int hit_strength;


public:
/**
* Weapon(): creates a defult weapon type
*
* don't get an input
* @return
* 	a weapon type with garbage values
**/
    explicit Weapon()= default;
/**
* Weapon(const string name, Target target1,int hit_strength): creates a weapon type according to the inputs
*
* @name - a string that contains the name of the player
* @target1 - the kind of target the weapon is affective against
* @hit_strength - the amount of hit strength
* @return
* 	a weapon type with the input values
**/

    explicit Weapon(const string name1, Target target1,int hit_strength);
/**
* Weapon(const Weapon& weapon): copy the input weapon values and creates a new weapon with the same values
*
* @weapon - a reference to an existing weapon type

* @return
* 	a weapon type with the input weapon values
**/

    Weapon(const Weapon& weapon);
///**
//   *   ~Weapon(): delete the weapon (destructor)
//   *
//   * don't get input (except "this")
//   * @return
//   * 	don't return anything
//**/
//
//    ~Weapon();
/**
* getTarget(): gets the weapon's target
*
* don't get input (except "this")
* @return
* 	returns the weapon's target
**/

    Target getTarget()const;
/**
* getHitStrength(): gets the weapon's hit strength
*
* don't get input (except "this")
* @return
* 	returns the weapon's hit strength
**/

    int getHitStrength()const;
/**
* getValue(): gets the weapon's hit strength by calling to getHitStrength
*
* don't get input (except "this")
* @return
* 	returns the weapon's value (hit strength)
**/

    int getValue()const ;
/**
* operator<<(ostream& os,const Weapon& weapon1): output operator , prints the neccesery details
*
* @os - the wanted output source
* @player1 - a reference to an existing weapon type
* @return
* 	prints to the wanted output
**/

    friend ostream& operator<<(ostream& os,const Weapon& weapon1);
/**
* operator=(const Weapon& weapon1): making assignment to a weapon according to the details of the input weapon
*
* @weapon1 - a reference to an existing weapon type
*
* @return
* 	the weapon after the assignment
**/

    Weapon& operator=(const Weapon& weapon1);
};
/**
* operator==(const Weapon& weapon1,const Weapon& weapon2): comparing between 2 weapons
* according to their values
*
* @weapon1 - a reference to an existing weapon type
* @weapon2 - a reference to an existing weapon type
* @return
* 	TRUE - if weapon1's value is equal to the weapon2's value
* 	FALSE - if weapon1's value isn't equal to the weapon2's value
**/

bool operator==(const Weapon& weapon1,const Weapon& weapon2);
/**
* operator<(const Weapon& weapon1,const Weapon& weapon2): comparing between 2 weapons
* according to their values
*
* @weapon1 - a reference to an existing weapon type
* @weapon2 - a reference to an existing weapon type
* @return
* 	TRUE - if weapon1's value isn't equal to the weapon2's value
* 	FALSE - if weapon1's value is equal to the weapon2's value
**/

bool operator!=(const Weapon& weapon1,const Weapon& weapon2);
/**
* operator<(const Weapon& weapon1,const Weapon& weapon2): comparing between 2 weapons
* according to their values
*
* @weapon1 - a reference to an existing weapon type
* @weapon2 - a reference to an existing weapon type
* @return
* 	TRUE - if weapon1's value is smaller than weapon2's value
* 	FALSE - if weapon1's value isn't smaller than weapon2's value
**/
bool operator<(const Weapon& weapon1,const Weapon& weapon2);
/**
* operator>(const Weapon& weapon1,const Weapon& weapon2): comparing between 2 weapons
* according to their values
*
* @weapon1 - a reference to an existing weapon type
* @weapon2 - a reference to an existing weapon type
* @return
* 	TRUE - if weapon1's value is bigger than weapon2's value
* 	FALSE - if weapon1's value isn't bigger than weapon2's value
**/

bool operator>(const Weapon& weapon1,const Weapon& weapon2);

#endif //HW4SOLUTION_WEAPON_H
