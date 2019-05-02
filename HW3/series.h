
#ifndef _SERIES_H
#define _SERIES_H

#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "mtmflix.h"
#include "map.h"
#include "user.h"

#define MIN_AGE_CELL 0
#define MAX_AGE_CELL 1

typedef struct series_t* Series;

typedef enum {
    SERIES_SUCCESS,
    SERIES_OUT_OF_MEMORY,
    SERIES_NULL_ARGUMENT,
    SERIES_ILLEGAL_NAME,
    SERIES_ILLEGAL_DURATION,
    SERIES_ILLEGAL_GENRE,
    SERIES_ILLEGAL_EPISODES_NUM,
    SERIES_ALREADY_EXISTS,
} SeriesResult;

/*creates a struct of a series by given parameters*/
Series SeriesCreate(const char* name, int episodesNum, Genre genre, int* ages,
                    int episodeDuration, SeriesResult* result);
/* destroys a series*/
void SeriesDestroy(Series series);
/*an external function used to pull a string of the name of a series from the
 * pointer to the struct*/
char* SeriesGetName(Series series);
/*an external function used to pull the enum genre of a show from the pointer
 * to the struct of the series*/
Genre SeriesGetGenre(Series series);
/*an external function used to get the episode duration of a valid series*/
int SeriesGetDuration(Series series);
/*an external function used to get the episodes number of a valid series*/
int SeriesGetEpisodeNum(Series series);
/*an external function used to get the ages of a valid series into a given
 * int array of the size of 2, it will not fill more slots as that is all the
 * info held by it.*/
void SeriesGetAgeLimits(Series series,int* Ages);
/*an external function used to get pointer to the struct of a series if the
 * string given matches an existing series in a given map*/
Series NameGetSeries(char* seriesName, Map SeriesMap);
/*calculates how many shows exist in the system with the same genre of a
 * given genre*/
int GenreGetSeriesAmount(Genre genre, Set usersLikes, Map allSeries);
/* check in a given set of friends how many of the friends like this show*/
int SeriesGetAmountOfFriendLikes(Set usersFriends, Map allUsers,
char* seriesName);
/* gives an array of the genre ralated to a given series which is located by
 * its name*/
char* SeriesNameGetGenreArray(char* seriesName, Map allSeries);
/* sorts and prints all the series in a given map by there genre and in the
 * genre by alphabitical order, uses a sorted set with the relevant compare
 * function (inner function made to sertain spesiphications) in order to sort
 * all the names of shows*/
void SeriesSortByGenreAndName(Map allSeries, int seriesNum, FILE* outputStream);

/**for use of mtmFlix struct**/
/* for the use of the map that holds series, copies the struct of the series*/
MapDataElement SeriesCopyData(MapDataElement series);
/* for the use of the map that holds series frees data*/
void SeriesFreeData(MapDataElement series);
/* for the use of the map that holds series copies the name of series*/
MapKeyElement SeriesCopyKey(MapKeyElement seriesName);
/* for the use of the map that holds series frees key*/
void SeriesFreeKey(MapKeyElement seriesName);
/* for the use of the map that holds series, this compares series by the key
 * (which is the name string)*/
int SeriesCompare(MapKeyElement seriesName1, MapKeyElement seriesName2);
/* used to swap arrays, mainly to swap names of shows while sorting in the
 * function below*/

#endif
