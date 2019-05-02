
#include "series.h"
 /* a struct of a series:
  * @episodeNum - a positive amount of episodes in the show
  * @episodeDuration - a positive number of the length of the show
  * @ages - hold in the first pointer the minimun age required to watch the
  * show and in the second pointer the minimun age required to watch the show
  * @genre - hold the enum dedicated to the genre of the show
  * @name - an allocated array of the name of the show*/
struct series_t{
    unsigned int episodeNum;
    unsigned int episodeDuration;
    int ages[2];
    Genre genre;
    char* name;
};
/*an internal function that makes sure a series name is made only by the
 * approved characters: 0-9, a-z, A-Z */
static bool IsSeriesNameValid(const char*name){
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
/*an internal function that makes sure the enum is by the given rules*/
static bool IsGenreValid(Genre genre){
    switch (genre){
        case SCIENCE_FICTION :
            return true;
        case DRAMA :
            return true;
        case COMEDY :
            return true;
        case CRIME :
            return true;
        case MYSTERY :
            return true;
        case DOCUMENTARY :
            return true;
        case ROMANCE :
            return true;
        case HORROR :
            return true;
        default:
            return false;
    }
}
/*an internal function that makes sure all the parameters are valid according
 * to given rules: the name isnt empty, the amount of episodes and the
 * duration of them is a positive number ,the genre is valid*/
static bool SeriesArgumentValid(const char* name, int episodesNum, Genre genre,
int episodeDuration, const int* ages, SeriesResult* result){
    if(!name) {
        *result = SERIES_NULL_ARGUMENT;
        return false;
    }
    if(!IsSeriesNameValid(name)){
        *result = SERIES_ILLEGAL_NAME;
        return false;
    }
    if(episodesNum <= 0){
        *result = SERIES_ILLEGAL_EPISODES_NUM;
        return false;
    }
    if(!strcmp(name,"")){
        *result = SERIES_ILLEGAL_NAME;
        return false;
    }
    if(!ages){
        *result = SERIES_NULL_ARGUMENT;
    }
    if(episodeDuration <= 0){
        *result = SERIES_ILLEGAL_DURATION;
        return false;
    }
    if(!IsGenreValid(genre)){
        *result = SERIES_ILLEGAL_GENRE;
        return false;
    }
    *result = SERIES_SUCCESS;
    return true;
}
/*an internal function that makes sure the age limits of a show arent out of
 * the bounds of the system it is in*/
static void SeriesSetAgeLimits(Series series, const int* ages){
    if(ages[MIN_AGE_CELL] < MTM_MIN_AGE){
        series->ages[MIN_AGE_CELL] = MTM_MIN_AGE;
    }else{
        series->ages[MIN_AGE_CELL] = (unsigned) ages[MIN_AGE_CELL];
    }
    if(ages[MAX_AGE_CELL] < MTM_MAX_AGE){
        series->ages[MAX_AGE_CELL] = (unsigned) ages[MAX_AGE_CELL];
    }else{
        series->ages[MAX_AGE_CELL] = MTM_MAX_AGE;
    }
}
/*creates a struct of a series by given parameters*/
Series SeriesCreate(const char* name, int episodesNum, Genre genre, int* ages,
                    int episodeDuration, SeriesResult* result){
    if(!SeriesArgumentValid(name,episodesNum,genre,
                            episodeDuration,ages,result)){
        return NULL;
    }
    Series series = malloc(sizeof(*series));
    if(!series){
        *result = SERIES_OUT_OF_MEMORY;
        return NULL;
    }
    series->name = malloc(sizeof(name)*(strlen(name)+1));
    if(!series->name || !strcpy(series->name,name)){
        free(series);
        *result = SERIES_OUT_OF_MEMORY;
        return NULL;
    }
    series->episodeNum = (unsigned) episodesNum;
    series->episodeDuration = (unsigned) episodeDuration;

    if(!ages){
        series->ages[MIN_AGE_CELL] = MTM_MIN_AGE;
        series->ages[MAX_AGE_CELL] = MTM_MAX_AGE;
    }else{
       SeriesSetAgeLimits(series, ages);
    }
    series->genre = genre;
    *result = SERIES_SUCCESS;
    return series;
}
/*an internal function that makes sure all the parameters given to the
 * SeriesCreate function are valid.*/
static bool SeriesValid(Series series, SeriesResult* result){
    return  SeriesArgumentValid(series->name,series->episodeNum,series->genre,
                                series->episodeDuration,series->ages,result);
}
/*an external function used to pull a string of the name of a series from the
 * pointer to the struct*/
char* SeriesGetName(Series series){
    SeriesResult result;
    if(SeriesValid(series,&result) && result == SERIES_SUCCESS){
        return series->name;
    }
    return NULL;
}
/*an external function used to pull the enum genre of a show from the pointer
 * to the struct of the series*/
Genre SeriesGetGenre(Series series){
    if(!series){
        return HORROR;
    }
    return series->genre;
}
/*an external function used to get the episode duration of a valid series*/
int SeriesGetDuration(Series series){
    SeriesResult result;
    if(!series){
        return 0;
    }
    if(SeriesValid(series,&result) && result == SERIES_SUCCESS){
        return series->episodeDuration;
    }
    return 0;
}
/*an external function used to get the episodes number of a valid series*/
int SeriesGetEpisodeNum(Series series){
    SeriesResult result;
    if(!series){
        return 0;
    }
    if(SeriesValid(series,&result) && result == SERIES_SUCCESS){
        return series->episodeNum;
    }
    return 0;
}
/*an external function used to get the ages of a valid series into a given
 * int array of the size of 2, it will not fill more slots as that is all the
 * info held by it.*/
void SeriesGetAgeLimits(Series series, int* Ages){
    SeriesResult result;
    if(!series || !Ages){
        return;
    }
    if(SeriesValid(series,&result) && result == SERIES_SUCCESS){
        Ages[MIN_AGE_CELL] = series->ages[MIN_AGE_CELL];
        Ages[MAX_AGE_CELL] = series->ages[MAX_AGE_CELL];
    }
}
/*an external function used to get pointer to the struct of a series if the
 * string given matches an existing series in a given map*/
Series NameGetSeries(char* seriesName, Map SeriesMap){
    if(mapContains(SeriesMap, (MapKeyElement) seriesName)){
        Series series = NULL;
        series = mapGet(SeriesMap, (MapKeyElement) seriesName);
        return series;
    }
    return NULL;
}
/* for the use of the map that holds series, this compares series by the key
 * (which is the name string)*/
int SeriesCompare(MapKeyElement seriesName1, MapKeyElement seriesName2){
    if(!seriesName1 || !seriesName2){
        return 0;
    }
    return strcmp((char*) seriesName1,(char*) seriesName2);
}
/* for the use of the map that holds series copies the name of series*/
MapKeyElement SeriesCopyKey(MapKeyElement seriesName){
    if(!seriesName){
        return NULL;
    }
    MapKeyElement key = malloc(sizeof((char*) seriesName)*
            strlen((char*)seriesName));
    if(!key || !strcpy((char*) key, (char*) seriesName)){
        return NULL;
    }
    return key;
}
/* for the use of the map that holds series, copies the struct of the series*/
MapDataElement SeriesCopyData(MapDataElement series){
    if(!series){
        return NULL;
    }
    Series series_p = *(Series*) series;
    SeriesResult result;
    Series data = SeriesCreate(series_p->name, series_p->episodeNum,
    series_p->genre,series_p->ages,series_p->episodeDuration, &result);
    if(!data){
        return NULL;
    }
    if(result != SERIES_SUCCESS){
        SeriesDestroy(data);
        return NULL;
    }
    return data;
}
/* for the use of the map that holds series frees data*/
void SeriesFreeData(MapDataElement series){
    if(!series){
        return;
    }
    SeriesDestroy((Series) series);
}
/* for the use of the map that holds series frees key*/
void SeriesFreeKey(MapKeyElement seriesName){
    if(!seriesName){
        return;
    }
    free(seriesName);
}
/*calculates how many shows exist in the system with the same genre of a
 * given genre*/
int GenreGetSeriesAmount(Genre genre, Set usersLikes, Map allSeries){
    int count = 0;
    SET_FOREACH(char*,seriesName,usersLikes){
        if(SeriesGetGenre(NameGetSeries(seriesName,allSeries)) == genre){
            count++;
        }
    }
    return count;
}
/* check in a given set of friends how many of the friends like this show*/
int SeriesGetAmountOfFriendLikes(Set usersFriends, Map allUsers,
char* seriesName){
    int count = 0;
    UserResult result;
    SET_FOREACH(char*,friendName,usersFriends){
        if(setIsIn(UserGetLikes(NameGetUser(friendName,allUsers),&result),
                   (SetElement) seriesName)){
            count++;
        }
    }
    return count;
}
/* gives an array of the genre ralated to a given series which is located by
 * its name*/
char* SeriesNameGetGenreArray(char* seriesName, Map allSeries){
    if(!seriesName || !allSeries){
        return NULL;
    }
    Genre genre = SeriesGetGenre(NameGetSeries(seriesName, allSeries));
    switch (genre){
        case COMEDY:
            return "COMEDY";
        case DRAMA:
            return "DRAMA";
        case CRIME:
            return "CRIME";
        case DOCUMENTARY:
            return "DOCUMENTARY";
        case SCIENCE_FICTION:
            return "SCIENCE_FICTION";
        case MYSTERY:
            return "MYSTERY";
        case ROMANCE:
            return "ROMANCE";
        case HORROR:
            return "HORROR";
    }
    return NULL;
}

char* SeriesGetGenreArray(Series series){
    if(!series){
        return NULL;
    }
    Genre genre = SeriesGetGenre(series);
    switch (genre){
        case COMEDY:
            return "COMEDY";
        case DRAMA:
            return "DRAMA";
        case CRIME:
            return "CRIME";
        case DOCUMENTARY:
            return "DOCUMENTARY";
        case SCIENCE_FICTION:
            return "SCIENCE_FICTION";
        case MYSTERY:
            return "MYSTERY";
        case ROMANCE:
            return "ROMANCE";
        case HORROR:
            return "HORROR";
    }
    return NULL;
}
/*copy function for a set element of a series*/
void* SeriesCopy(void* series){
    if(!series){
        return NULL;
    }
    Series series_p = series;
    SeriesResult result;
    Series data = SeriesCreate(series_p->name, series_p->episodeNum,
            series_p->genre,series_p->ages,series_p->episodeDuration, &result);
    if(!data){
        return NULL;
    }
    if(result != SERIES_SUCCESS){
        SeriesDestroy(data);
        return NULL;
    }
    return data;
}
/*free function for a set element of a series*/
void SeriesFree(void* series){
    if(!series){
        return;
    }
    SeriesDestroy((Series) series);
}
/* the set which uses this compare function is made to be in alphabetical
 * order of the genre (from low to high)these series belong to and in a state
 * of equal genres the lower name of show in alphabetical order is chosen to be
 * first*/
int CompareSeriesAndGenre(void* series1, void* series2){
    Series s1 = (Series) series1;
    Series s2 = (Series) series2;
    if(!strcmp(SeriesGetGenreArray(s1),SeriesGetGenreArray(s2))){
        return strcmp(SeriesGetName(s1), SeriesGetName(s2));
    }
    else if(strcmp(SeriesGetGenreArray(s1),SeriesGetGenreArray(s2)) > 0){
        return 1;
    }
    else{
        return -1;
    }
}
/* sorts all the series in a given map by there genre and in the genre by
 * alphabetical order, for more info go to the mtmFlixReportSeries function*/
void SeriesSortByGenreAndName(Map allSeries, int seriesNum, FILE* outputStream){
    if(!seriesNum){
        seriesNum = mapGetSize(allSeries);
    }
    Set sortedSeries = setCreate(SeriesCopy,SeriesFree,CompareSeriesAndGenre);
    MAP_FOREACH(char* ,iterator,allSeries){
        setAdd(sortedSeries, (Series) mapGet(allSeries,iterator));
    }
    int genreCount =0;
    Genre currGenre = SeriesGetGenre(setGetFirst(sortedSeries));
     SET_FOREACH(Series,iterator,sortedSeries){
         if(SeriesGetGenre(iterator) == currGenre){
             genreCount++;
             if(genreCount <= seriesNum){
                 const char* d = mtmPrintSeries(SeriesGetName(iterator),
                         SeriesGetGenreArray(iterator));
                 fprintf(outputStream,"%s",d);
             }
         }else{
             genreCount = 1;
             currGenre = SeriesGetGenre(iterator);
             const char* d = mtmPrintSeries(SeriesGetName(iterator),
                                            SeriesGetGenreArray(iterator));
             fprintf(outputStream,"%s",d);
         }
    }
    setDestroy(sortedSeries);
}
/* destroys a series*/
void SeriesDestroy(Series series){
    if(!series){
        return;
    }
    if(series->name){
        free(series->name);
    }
    free(series);
}