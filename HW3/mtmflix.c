#include "mtmflix.h"
#include "user.h"
#include "series.h"

struct mtmFlix_t{
        Map users;
        Map series;
};


/*creates a new mtmflix system which includes a map of users and a map of
 * series, each map contains the name as key and the structs as data */
MtmFlix mtmFlixCreate(){
    MtmFlix system = malloc(sizeof(*system));
    if(!system){
        return NULL;
    }
    system->users = mapCreate(UserCopyData, UserCopyKey, UserFreeData,
                              UserFreeKey, UsersCompare);
    if(!system->users){
        free(system);
        return NULL;
    }
    system->series = mapCreate(SeriesCopyData, SeriesCopyKey, SeriesFreeData,
            SeriesFreeKey, SeriesCompare);
    if(!system->series){
        mapDestroy(system->users);
        free(system);
        return NULL;
    }
    return system;
}

/* destroys an existing system */
void mtmFlixDestroy(MtmFlix mtmflix){
    if(!mtmflix){
        return;
    }
    if(mtmflix->users){
        mapDestroy(mtmflix->users);
    }
    if(mtmflix->series){
        mapDestroy(mtmflix->series);
    }
    free(mtmflix);
}
/* creates a new series struct that includes all the given info of the series
 * and puts it in the data of a map element with the key set to be the name
 * of the series (the name exists twice, 1 in the key and 1 in the struct of
 * the series under name
 * the function makes sure all the information given is legal up to the
 * checks which are relevant for the series itself to check, the basically
 * returns the same notification*/
MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name, int
episodesNum, Genre genre, int* ages, int episodesDuration){
    if(!mtmflix || !name){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(mapContains(mtmflix->series, (MapKeyElement) name)){
        return MTMFLIX_SERIES_ALREADY_EXISTS;
    }
    SeriesResult result;
    Series series = SeriesCreate(name, episodesNum, genre, ages,
            episodesDuration, &result);
    switch (result){
        case SERIES_NULL_ARGUMENT:
            return MTMFLIX_OUT_OF_MEMORY;
        case SERIES_ILLEGAL_NAME :
            return MTMFLIX_ILLEGAL_SERIES_NAME;
        case SERIES_ILLEGAL_DURATION :
            return MTMFLIX_ILLEGAL_EPISODES_DURATION;
        case SERIES_ILLEGAL_EPISODES_NUM :
            return MTMFLIX_ILLEGAL_EPISODES_NUM;
        default : ;
    }
     if(mapPut(mtmflix->series, (MapKeyElement) name, (MapDataElement) &series)
        != MAP_SUCCESS){
        SeriesDestroy(series);
         return MTMFLIX_OUT_OF_MEMORY;
    }
    SeriesDestroy(series);
    return MTMFLIX_SUCCESS;
}
/* if the series is valid this function makes sure to remove the series from
 * any users list of likes because it is not allowed for a user to like a
 * series that is not in the system. when it finishes that the function
 * removes the chosen series from the series map connected to the system*/
MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name){
    if(!mtmflix || !name){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->series, (MapKeyElement) name)){
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    MAP_FOREACH(char*, in, mtmflix->users){
        UserResult res;
        User user = mapGet(mtmflix->users, (MapKeyElement) in);
        if(setIsIn(UserGetLikes(user,&res),(SetElement) name)){
            UserUnlikeSeries(user,name);
        }
    }
    mapRemove(mtmflix->series, (MapKeyElement) name);
    return MTMFLIX_SUCCESS;
}
/* creates a new user based on the info given to the function, makes sure
 * that the pointers are valid and that the user dosent exist in the system,
 * if it passes the user creation is made elsewhere and the function just
 * repeats the results in case of a failior, if all passes the user created
 * is copied the the user map in the system and the original is destroyed*/
MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix, const char* username, int age){
    if(!mtmflix || !username){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(mapContains(mtmflix->users, (MapKeyElement) username)){
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    UserResult result;
    User user = UserCreate(username, age,&result);
    switch (result){
        case USER_NULL_ARGUMENT:
            UserDestroy(user);
            return MTMFLIX_OUT_OF_MEMORY;
        case USER_ILLEGAL_NAME:
            UserDestroy(user);
            return MTMFLIX_ILLEGAL_USERNAME;
        case USER_OUT_OF_MEMORY:
            UserDestroy(user);
            return MTMFLIX_OUT_OF_MEMORY;
        case USER_ILLEGAL_AGE:
            UserDestroy(user);
            return MTMFLIX_ILLEGAL_AGE;
        default:;
    }
    if(mapPut(mtmflix->users, (MapKeyElement) username, (MapDataElement) &user)
            != MAP_SUCCESS){
        UserDestroy(user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    UserDestroy(user);
    return MTMFLIX_SUCCESS;
}
/* removes an existing user for the user map of the system*/
MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username){
    if(!mtmflix || !username){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->users, (MapKeyElement) username) ||
       !strcmp(username, "")){
        return MTMFLIX_USER_DOES_NOT_EXIST;
    } else {
        mapRemove(mtmflix->users, (MapKeyElement) username);
        return MTMFLIX_SUCCESS;
    }
}
/* makes sure all arguments are valid, and that the show that the user wants
 * to add to his likes list is age appropriate. if it is the series name is
 * added to a set (for the assurance there is no duplications) of likes in
 * the struct of the user*/
MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username, const
char* seriesName){
    if(!mtmflix || !username || !seriesName){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->users,(MapKeyElement) username)){
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    if(!mapContains(mtmflix->series,(MapKeyElement) seriesName)){
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    User user = mapGet(mtmflix->users,(MapKeyElement) username);
    Series series = mapGet(mtmflix->series, (MapKeyElement) seriesName);
    UserResult userResult;
    int Ages[2];
    SeriesGetAgeLimits(series, Ages);
    if(UserGetAge(user, &userResult) < Ages[MIN_AGE_CELL] ||
            UserGetAge(user, &userResult) > Ages[MAX_AGE_CELL]){
        return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
    }
    UserLikeSeries(user, seriesName);
    return MTMFLIX_SUCCESS;
}
/* if all arguments are valid and the user previusly liked this show the it
 * removes the series name from the set of likes*/
MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix, const char* username, const
char* seriesName){
    if(!mtmflix || !username || !seriesName){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->users,(MapKeyElement) username)){
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    if(!mapContains(mtmflix->series,(MapKeyElement) seriesName)){
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    User user = mapGet(mtmflix->users,(MapKeyElement) username);
    UserUnlikeSeries(user, seriesName);
    return MTMFLIX_SUCCESS;
}
/* if all arguments are valid the the name username2 is added to the set
 * friends of the user with the name username1*/
MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix, const char* username1, const
char* username2){
    if(!mtmflix || !username1 || !username2){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->users, (MapKeyElement) username1) ||
            !mapContains(mtmflix->users, (MapKeyElement) username2)){
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    User user = NameGetUser(username1, mtmflix->users);
    if(UserAddFriend(user, username2) != USER_SUCCESS){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_SUCCESS;
}
/* if the user with the name username1 has a friend with the name username2
 * in his set of friends then this function removes that name from the set*/
MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix, const char* username1,
const char* username2){
    if(!mtmflix || !username1 || !username2){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->users, (MapKeyElement) username1) ||
       !mapContains(mtmflix->users, (MapKeyElement) username2)){
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    User user = NameGetUser(username1, mtmflix->users);
    UserRemoveFriend(user, username2);
    return  MTMFLIX_SUCCESS;
}
/* internal function used to get the minimum between two numbers. used for
 * the calculations of the user recommendation ranking system*/
static int GetMinInt(int a, int b){
    if(a<b){
        return a;
    }
    return b;
}
/* internal function used to destroy an array of strings. used for the array
 * of strings that exists in the mtmFlixGetRecommendations function for the
 * struct that calculates ranks of shows based on a given user */
static void DestroyArray(void** array, int size){
    for (int i = 0; i < size; ++i) {
        free(array[i]);
    }
    if(size){
        free(array);
    }
}
/* makes sure the arguments are valid.
 * @length - finds the maximum amount of series possible to recommend for a
 * given user
 * @seriesArray - hold all the series names existing in the map to add to the
 * set of structs called UserRecommendation which holds the name of a series
 * and pairs it to the rank this given series gets for a given user.
 * this array is created at the point this function is called for the case
 * series are deleted or added between recommendation checks.
 * */
MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix, const char* username,
                                        int count, FILE* outputStream){
    if(!mtmflix || !username || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapContains(mtmflix->users, (MapKeyElement) username)){
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    if(count < 0){
        return MTMFLIX_ILLEGAL_NUMBER;
    }
    int length;
    if(!count){
        length = mapGetSize(mtmflix->series);
    }else{
        length = GetMinInt(mapGetSize(mtmflix->series), count);
    }
    char** seriesArray = malloc(sizeof(char*)*mapGetSize(mtmflix->series));
    if(!seriesArray){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    int i=0;
    MAP_FOREACH(char*, seriesName, mtmflix->series) {
        seriesArray[i] = malloc(sizeof(char)*(strlen(seriesName)+1));
        if(!seriesArray[i]){
            return MTMFLIX_OUT_OF_MEMORY;
        }
        strcpy(seriesArray[i], seriesName);
        i++;
    }
    UserGetRecommendations(NameGetUser(username, mtmflix->users),seriesArray,
                           mtmflix->series,mtmflix->users,length, outputStream);
    DestroyArray((void**) seriesArray,mapGetSize(mtmflix->series));
    return MTMFLIX_SUCCESS;
}
/* sorts all shows by order of genre and prints by genre and alphabetical
 * order all the existing series in the system */
MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix, int seriesNum, FILE*
outputStream){
    if(!mtmflix || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapGetSize(mtmflix->series)){
        return  MTMFLIX_NO_SERIES;
    }
    SeriesSortByGenreAndName(mtmflix->series, seriesNum, outputStream);
    return MTMFLIX_SUCCESS;
}
/* used to swap arrays, mainly to swap names of shows while sorting in the
 * function below*/
void SwapArr(char** a, char** b){
    char* temp = *a;
    *a = *b;
    *b = temp;
}
/* sorts and prints all user by alphabetical order*/
MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream){
    if(!mtmflix || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!mapGetSize(mtmflix->users)){
        return  MTMFLIX_NO_USERS;
    }
    if(!mapGetSize(mtmflix->users)){
        return  MTMFLIX_NO_USERS;
    }
    int size = mapGetSize(mtmflix->users);
    int i=0;
    char** usersNames = malloc(sizeof(*usersNames)*size);
    MAP_FOREACH(char*, username, mtmflix->users) {
        usersNames[i] = malloc(sizeof(char)*(strlen(username)+1));
        strcpy(usersNames[i], username);
        i++;
    }
    for (int j = 0; j < size-1; ++j) {
        for (int k = j; k < size; ++k) {
            if(strcmp(usersNames[j], usersNames[k]) > 0){
                SwapArr(&(usersNames[j]),&(usersNames[k]));
            }
        }
    }
    UserMapPrintInfo(usersNames,mtmflix->users, outputStream);
    DestroyArray((void**) usersNames, size);
    return MTMFLIX_SUCCESS;
}