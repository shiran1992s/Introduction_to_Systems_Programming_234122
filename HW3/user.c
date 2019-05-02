
#include "user.h"

/* this struct contains all the given info of a user
 * @ name - allocated string of the users name
 * @ age - a positive integer age
 * @ friend - a sorted set of names of other users that this user chose to
 * add to his friends list
 * series - a sorted set of names of series names that this user chose to
 * add to his likes list*/
struct user_t{
    char* name;
    unsigned int age;
    Set friends;
    Set likes;
};
/* an internal struct user for the UserGetRecommendation function.
 * pairs a series name with a non-negative integer that states the rank of a
 * given show (only in comparison to a spesific user!) */
struct user_recommendations{
    char* name;
    int rank;
};
/* used both for sets and lists that hold string based valuse*/
void* CopyString(void* String){
    if(!String){
        return NULL;
    }
    char* stringCopy = malloc(sizeof(char*)*strlen((char*) String));
    if(!stringCopy || !strcpy(stringCopy, (char*) String)){
        return NULL;
    }
    return stringCopy;
}
/* used both for sets and lists that hold string based valuse*/
void FreeString(void* string){
    if(string){
        free(string);
    }
}
/* used both for sets and lists that hold string based valuse*/
int CompareStrings(void* existing, void* new){
    if(!existing || !new){
        return EQUAL;
    }
    return strcmp((char*) existing, (char*) new);
}
/* adds a rank struct into a temporary set, the copy function is taking all
 * the structs argument because the set itself is sorted and by a correct use
 * of the compare function this set sorts by two different parameters*/
void* CopyRank(void* rank){
    if(!rank){
        return NULL;
    }
    UsersRecommendations rankCopy = malloc(sizeof(*rankCopy));
    if(!rankCopy){
        return NULL;
    }
    rankCopy->name = malloc(sizeof(char)*(strlen((
            (UsersRecommendations)rank)->name)+1));
    if(!rankCopy->name ||
       !strcpy(rankCopy->name,((UsersRecommendations)rank)->name)){
        return NULL;
    }
    rankCopy->rank = ((UsersRecommendations) rank)->rank;
    return rankCopy;
}
/* for the use of a set of ranks*/
void FreeRank(void* rank){
    if(!rank || !((UsersRecommendations)rank)->name){
        return;
    }
    free(((UsersRecommendations)rank)->name);
    free((UsersRecommendations)rank);
}
/* this compare makes sure of no duplications in a given set as well as
 * sorting the set by the following:
 * the higher rank is first
 * if two ranks are equal the smaller name of the rank is first*/
int CompareRank(void* rank1, void* rank2){
    UsersRecommendations r1 = (UsersRecommendations) rank1;
    UsersRecommendations r2 = (UsersRecommendations) rank2;
    if(r1->rank > r2->rank){
        return -1;
    }
    else if(r1->rank < r2->rank){
        return 1;
    }
    else{
        return (strcmp(r1->name, r2->name));
    }
}
/* internal function. makes sure the user pointer is pointing to a real
 * argument*/
static bool IsUserValid(User user, UserResult* result){
    if(!user){
        *result = USER_NULL_ARGUMENT;
        return false;
    }
    return true;
}
/* internal function. makes sure the user name has only approved characters:
 * only numbers 0-9, small letters a-z, capital letters A-Z are approved*/
static bool IsUsernameValid(const char* name){
    int i=0;
    do{
        if(name[i] >= '0' && name[i] <= '9'){
            i++;
            continue;
        }else if(name[i] >= 'a' && name[i] <= 'z'){
            i++;
            continue;
        } else if(name[i] >= 'A' && name[i] <= 'Z'){
            i++;
            continue;
        }else{
            return false;
        }
    } while(name[i]);
    return true;
}
/* if all arguments a valid and the users info is ok with the given age
 * limits in the system and the username dosent exist then a new struct is
 * created and the name age and empty sets of friends and likes are dedicated
 * to him*/
User UserCreate(const char* username, int age, UserResult* result){
    if(!username){
        *result = USER_NULL_ARGUMENT;
        return NULL;
    }
    if(age <MTM_MIN_AGE || age >MTM_MAX_AGE){
        *result = USER_ILLEGAL_AGE;
        return NULL;
    }
    if(!IsUsernameValid(username)){
        *result = USER_ILLEGAL_NAME;
        return NULL;
    }
    User user = malloc(sizeof(*user));
    if(!user){
        *result = USER_OUT_OF_MEMORY;
        return NULL;
    }
    user->name = malloc(sizeof(username)*(strlen(username)+1));
    if(!user->name || !strcpy(user->name,username)){
        free(user);
        *result = USER_OUT_OF_MEMORY;
        return NULL;
    }
    user->age = (unsigned) age;
    user->friends = setCreate(CopyString, FreeString, CompareStrings);
    user->likes = setCreate(CopyString, FreeString, CompareStrings);
    *result = USER_SUCCESS;
    return user;
}
/* gets the name of a user by the pointer for the struct. for external use*/
const char* UserGetName(User user, UserResult* result){
    if(!IsUserValid(user,result)){
        return NULL;
    }
    *result = USER_SUCCESS;
    return user->name;
}
/* gets the age of a user by the pointer for the struct. for external use*/
const int UserGetAge(User user, UserResult* result){
    if(!IsUserValid(user,result)){
        return 0;
    }
    *result = USER_SUCCESS;
    return user->age;
}
/* gets the set of likes of a user by the pointer for the struct. for
 * external use*/
Set UserGetLikes(User user, UserResult* result){
    if(!IsUserValid(user,result)){
        return NULL;
    }
    return user->likes;
}
/* gets the set of friends of a user by the pointer for the struct. for
 * external
 * use*/
Set UserGetFriends(User user, UserResult* result){
    if(!IsUserValid(user,result)){
        return NULL;
    }
    return user->friends;
}
/* gets the pointer to the struct that contain a name of a user by the name
 * of the user as a string. for external use*/
User NameGetUser(const char* name, Map UsersMap){
    if(name && mapContains(UsersMap, (MapKeyElement) name)){
        return (User) mapGet(UsersMap,(MapKeyElement) name);
    }
    return NULL;
}
/* addes a given string of a series name to the set of likes of a user*/
UserResult UserLikeSeries(User user, const char* seriesName){
    if(!user || !seriesName){
        return USER_NULL_ARGUMENT;
    }
    if(setIsIn(user->likes,(SetElement) seriesName)){
        return USER_SUCCESS;
    }
    setAdd(user->likes,(SetElement) seriesName);
    return USER_SUCCESS;
}
/* if a name equal to the series name given to the function is found in the
 * users likes set it removes that name from the set*/
UserResult UserUnlikeSeries(User user, const char* seriesName){
    if(!user || !seriesName){
        return USER_NULL_ARGUMENT;
    }
    if(setIsIn(user->likes,(SetElement) seriesName)){
        setRemove(user->likes, (SetElement) seriesName);
    }
    return USER_SUCCESS;
}
/* addes a given string of a users name to the set of friends of a user*/
UserResult UserAddFriend(User user, const char* friendsName){
    if(!user || !friendsName){
        return USER_NULL_ARGUMENT;
    }
    UserResult res;
    if(!strcmp(UserGetName(user,&res),friendsName)){
        return USER_SUCCESS;
    }
    if(setIsIn(user->friends,(SetElement) friendsName)){
        return USER_SUCCESS;
    }
    setAdd(user->friends, (SetElement) friendsName);
    return USER_SUCCESS;
}
/* if a name equal to the users name given to the function is found in the
 * users friends set it removes that name from the set*/
UserResult UserRemoveFriend(User user, const char* friendsName){
    if(!user || !friendsName){
        return USER_NULL_ARGUMENT;
    }
    if(setIsIn(user->friends, (SetElement) friendsName)){
        setRemove(user->friends, (SetElement) friendsName);
    }
    return USER_SUCCESS;
}
/* copy function for the map of users */
MapKeyElement UserCopyKey(MapKeyElement username){
    if(!username){
        return NULL;
    }
    MapKeyElement key = malloc(sizeof((char*) username)*
            strlen((char*)username));
    if(!key || !strcpy((char*) key, (char*) username)){
        return NULL;
    }
    return key;
}
/* free function for the map of users */
void UserFreeKey(MapKeyElement username){
    if(!username){
        return;
    }
    free(username);
}
/* copy function for the map of users */
MapDataElement UserCopyData(MapDataElement user){
    if(!user){
        return NULL;
    }User user_p = *(User*) user;
    UserResult result;
    User data = UserCreate(user_p->name, user_p->age, &result);
    if(!data){
        return NULL;
    }

    if(result != USER_SUCCESS){
        UserDestroy(data);
        return NULL;
    }
    return data;
}
/* free function for the map of users */
void UserFreeData(MapDataElement user){
    if(!user){
        return;
    }
    UserDestroy((User) user);
}
/* compare function for the map of users, compares by alphabetical order
 * (only 0 or not 0 are relevant since it is a non sorted map) */
int UsersCompare(void* username1, void* username2){
    if(!username1 || !username2){
        return 0;
    }
    return strcmp((char*) username1,(char*) username2);
}
/* an internal function that calculates the avarage duration of series in a
 * given users like list, although duration are integer returns a float
 * number because of a devision made (just a convension of the desired
 * calculation for the excersise)*/
static float UserGetAvarageDurationLikes(User user, Map allSeries){
    float sum = 0;
    int count = 0;
    SET_FOREACH(char*,seriesName,user->likes){
        sum += SeriesGetDuration(NameGetSeries(seriesName,allSeries));
        count++;
    }
    if(count) {
        sum = sum/count;
        return sum;
    } else{
        return 0;
    }
}
/*an internal function to save a lib*/
static float Abs(float a){
    if(a<0){
        return -a;
    }
    return a;
}
/* an internal function makes sure a given users age is in the boundries of a
 * given series age limits*/
static bool IsSeriesAgeAppropriate(int age, Series series){
    int seriesAgeLimit[2];
    SeriesGetAgeLimits(series, seriesAgeLimit);
    if(age < seriesAgeLimit[MIN_AGE_CELL] ||
            age > seriesAgeLimit[MAX_AGE_CELL]){
        return false;
    }
    return true;
}
/* an internal function calculates the rank of a given series for a given
 * user by rules decided by higher ranked people
 * the calculation: rank = ((G*F)/(1+|CUR-L|))*/
static int CurrSeriesGetRank(User user,char* currSeries ,Map allSeries, Map
allUsers){
    float G,F,CUR,L,total;
    G = GenreGetSeriesAmount(SeriesGetGenre(
            NameGetSeries(currSeries, allSeries)),user->likes,allSeries);
    F = SeriesGetAmountOfFriendLikes(user->friends,allUsers,currSeries);
    L = UserGetAvarageDurationLikes(user, allSeries);
    CUR = SeriesGetDuration(NameGetSeries(currSeries, allSeries));
    total = (G*F)/(1+Abs(CUR-L));
    return (int) total;
}
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
                              Map allUsers, int length, FILE* outputStream){
      if(!mapGetSize(allSeries) || !user || !seriesArray){
          return;
      }
      Set UserRec =setCreate(CopyRank, FreeRank, CompareRank);
      for(int i=0; i < mapGetSize(allSeries); i++){
          UsersRecommendations rank = malloc(sizeof(*rank));
          if(!rank){
              return;
          }
          rank->name = malloc(sizeof(char)*(strlen(seriesArray[i])+1));
          if(!rank->name){
              free(rank);
              return;
          }
          strcpy(rank->name, seriesArray[i]);
          rank->rank = CurrSeriesGetRank(user,rank->name,allSeries,allUsers);
          setAdd(UserRec, (SetElement) rank);
          free(rank->name);
          free(rank);
      }
      int relevant =0;
      SET_FOREACH(UsersRecommendations ,rank,UserRec){
          if(!IsSeriesAgeAppropriate
                  (user->age, NameGetSeries(rank->name, allSeries)) ||
             setIsIn(user->likes, (SetElement) rank->name)){
              rank->rank = 0;
          }else{
              relevant++;
          }
      }
      int count =0;
      setGetFirst(UserRec);
      SET_FOREACH(UsersRecommendations ,rank,UserRec){
          if(count < relevant && rank->rank && count < length) {
              const char* d = mtmPrintSeries(rank->name,
                             SeriesNameGetGenreArray(rank->name, allSeries));
              fprintf(outputStream,"%s" ,d);
              count++;
          }
      }
      setDestroy(UserRec);
  }
/* used for copying a set to a list. mainly for the usage of the print
 * function given to us*/
List SetToList(Set set){
    List list = listCreate(CopyString, FreeString);
    SET_FOREACH(char*,friendName,set){
        listInsertLast(list, (ListElement) friendName);
    }
    return list;
}
/* uses a given print function to print all the information desired of a given
 * user found by his name*/
void UserMapPrintInfo(char** sortedNames, Map allUsers, FILE* outputStream){
    UserResult result;
    for (int i = 0; i < mapGetSize(allUsers); ++i) {
        List friends =
                SetToList((NameGetUser(sortedNames[i], allUsers))->friends);
        List likes =
                SetToList((NameGetUser(sortedNames[i], allUsers))->likes);
        const char* d = mtmPrintUser(sortedNames[i], UserGetAge(NameGetUser
                (sortedNames[i],allUsers),&result), friends, likes);
        fprintf(outputStream,"%s",d);
        listDestroy(friends);
        listDestroy(likes);
    }
}
/* destroys a user*/
void UserDestroy(User user){
    if(!user){
        return;
    }
    if(user->likes){
        setDestroy(user->likes);
    }
    if(user->friends){
        setDestroy(user->friends);
    }
    if(user->name){
        free(user->name);
    }
    free(user);
}

