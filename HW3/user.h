
#ifndef _USER_H
#define _USER_H

#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "mtmflix.h"
#include "set.h"
#include "series.h"
#include "list.h"

#define EQUAL 0
#define NOT_EQUAL 1

typedef struct user_t* User;
typedef struct user_recommendations* UsersRecommendations;

typedef enum {
    USER_SUCCESS,
    USER_OUT_OF_MEMORY,
    USER_NULL_ARGUMENT,
    USER_ILLEGAL_NAME,
    USER_ILLEGAL_AGE,
    USER_ALREADY_EXISTS,
    USER_ALREADY_FRIEND,
    USER_NOT_FRIEND,
} UserResult;

/* if all arguments a valid and the users info is ok with the given age
 * limits in the system and the username dosent exist then a new struct is
 * created and the name age and empty sets of friends and likes are dedicated
 * to him*/
User UserCreate(const char* username, int age, UserResult* result);
/* destroys a user*/
void UserDestroy(User user);

/* gets the name of a user by the pointer for the struct. for external use*/
const char* UserGetName(User user, UserResult* result);
/* gets the age of a user by the pointer for the struct. for external use*/
const int UserGetAge(User user, UserResult* result);
/* gets the set of likes of a user by the pointer for the struct. for
 * external use*/
Set UserGetLikes(User user, UserResult* result);
/* gets the set of friends of a user by the pointer for the struct. for
 * external use*/
Set UserGetFriends(User user, UserResult* result);
/* gets the pointer to the struct that contain a name of a user by the name
 * of the user as a string. for external use*/
User NameGetUser(const char* name, Map UsersMap);
/* adds a given string of a series name to the set of likes of a user*/
UserResult UserLikeSeries(User user, const char* seriesName);
/* if a name equal to the series name given to the function is found in the
 * users likes set it removes that name from the set*/
UserResult UserUnlikeSeries(User user, const char* seriesName);
/* adds a given string of a users name to the set of friends of a user*/
UserResult UserAddFriend(User user, const char* friendsName);
/* if a name equal to the users name given to the function is found in the
 * users friends set it removes that name from the set*/
UserResult UserRemoveFriend(User user, const char* friendsName);
/* recives the maps of all users and series to pull information relevant for
 * the function such as the amount of series in the map and for called
 * functions such as the rank calculator "CurrSeriesGetRank" that needs all
 * the users to pull users from friends list.
 *
 * the function creates a set of ranks only relevant for the user given and
 * prints the all relevant series (up to length amount) the set is sorted by
 * CompareRank funtion, and the rank of shows that already exist in the users
 * likes set or series that arent age appropriate are given the rank 0.*/
void UserGetRecommendations(User user, char** seriesArray, Map allSeries,
                            Map allUsers, int length, FILE* outputStream);
/* uses a given print function to print all the information desired of a given
 * user found by his name*/
void UserMapPrintInfo(char** sortedNames, Map allUsers, FILE* outputStream);

/**for use of mtmFlix struct**/
/* copy function for the map of users */
MapDataElement UserCopyData(MapDataElement user);
/* free function for the map of users */
void UserFreeData(MapDataElement user);
/* copy function for the map of users */
MapKeyElement UserCopyKey(MapKeyElement username);
/* free function for the map of users */
void UserFreeKey(MapKeyElement username);
/* compare function for the map of users, compares by alphabetical order
 * (only 0 or not 0 are relevant since it is a non sorted map) */
int UsersCompare(MapKeyElement username1, MapKeyElement username2);

#endif
