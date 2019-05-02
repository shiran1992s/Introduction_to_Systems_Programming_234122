#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "season.h"

static int FindIndexDrivers(Season season1,int id);
static int FindIndexResults(Season season1,int id,int* results);
static void MatchDriverTeam(Season season, const char* season_info,
SeasonStatus* status);
static void SwapDrivers(Driver* driver1, Driver* driver2);
static Driver* DriversMaxSort(Season season1);
static int DriversIndexOfMax(Driver driver1[], int drivers_n,int jump);
static void CreateSeasonDriver(SeasonStatus* status,DriverStatus driver_status1,
Season season1,char* line,int drivers_count);
static void CreateSeasonTeam(SeasonStatus* status,TeamStatus team_status1,
Season season1,char* line,int teams_count);
static Season SeasonSetUp(SeasonStatus* status,Season season1,
const char* season_info);
static Team* TeamsMaxSort(Season season);
static int IndexOfMaxTeams(Team* team_arr,int teams_size,int jump);
static void SwapTeams(Team* team1, Team* team2);
static Driver* SeasonSetDriversInfo(SeasonStatus* status,
DriverStatus driver_status1,Season season1,const char* season_info);
static Team* SeasonSetTeamsInfo(SeasonStatus* status,TeamStatus team_status1,
Season season1,const char* season_info);
static void IdMaxSort(int* id, int counter);
static int IndexOfMaxId(int* id, int counter);
static void SwapId(int * id1, int* id2);
static int FindIdByPosition(int position,Season season1,int* results);
static Driver* DriversMaxSortAfterPoints(Season season1,int* results);
static int CountPointDrivers(Season season1,int* index);
static Team* TeamsMaxSortAfterPoints(Season season1,int* results);
static int CountPointTeams(Season season1,int* index);
static int FindIndexTeams(Season season1,const char* name);
int TeamCmp(char** team1, const char* team2, int size);
void DestroyTempTeams(char** teams,int size);
static void FillResultsId(Season season1,int* results,int* id_points,
int* balance,int* index,int number_of_drivers);
static void DriverCounter(Season season1, int* index, int* balance,
int* number_of_drivers, int* counter);



struct season{
    int year;
    int teams_n;
    Team *teams;
    int drivers_n;
    Driver *drivers;
};

void SeasonDestroy(Season season){
    if(!season) {
        return;
    }
    int i=0;
    while((i<season->teams_n)&&(season->teams[i] != NULL)){
        TeamDestroy(season->teams[i]);
        i++;
    }
    free(season->drivers);
    free(season->teams);
    free(season);
}
//position is set by the place in the array teams in the struct season
//0 being the best position
Driver SeasonGetDriverByPosition(Season season, int position,
                                 SeasonStatus* status){
    if(position <= 0){
        if(status){
            *status = SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    if(season && season->drivers[position-1]){
        if(status) {
            *status = SEASON_OK;
        }
        return season->drivers[position-1];
    }
    if(status) {
        *status = SEASON_NULL_PTR;
    }
    return NULL;
}

Driver* SeasonGetDriversStandings(Season season){
    if(season) {
        return season->drivers;
    }
    return NULL;
}
//position is set by the place in the array teams in the struct season
//0 being the best position
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status){
    if(position <= 0){
        if(status) {
            *status = SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    if(season && season->teams[position-1]){
        if(status) {
            *status = SEASON_OK;
        }
        return season->teams[position-1];
    }
    if(status) {
        *status = SEASON_NULL_PTR;
    }
    return NULL;
}

Team* SeasonGetTeamsStandings(Season season){
    if(season) {
        return season->teams;
    }
    return NULL;
}

int SeasonGetNumberOfDrivers(Season season){
    if(season) {
        return season->drivers_n;
    }
    return 0;
}

int SeasonGetNumberOfTeams(Season season){
    if(season) {
        return season->teams_n;
    }
    return 0;
}
//updates data of the current race the finished
SeasonStatus SeasonAddRaceResult(Season season, int* results){
    if(!season || !results) {
        return SEASON_NULL_PTR;
    }
    for(int i=0; i< season->drivers_n; i++){
        int index=FindIndexDrivers(season,results[i]);
        DriverAddRaceResult(season->drivers[index],i+1);
    }
    season->drivers=DriversMaxSort(season);
    season->drivers=DriversMaxSortAfterPoints(season,results);
    season->teams=TeamsMaxSort(season);
    season->teams=TeamsMaxSortAfterPoints(season,results);
    return SEASON_OK;
}

static void SwapDrivers(Driver* driver1, Driver* driver2){
    Driver temp_driver = *driver1;
    *driver1 = *driver2;
    *driver2 = temp_driver;
}

//max is set by points in race
static int DriversIndexOfMax(Driver driver1[], int drivers_n,int jump){
    DriverStatus status1, status2;
    int i, i_max = 0;
    for (i = 1; i < drivers_n; ++i) {
        if ((DriverGetPoints(driver1[i], &status1) >
             DriverGetPoints(driver1[i_max], &status2)) &&
            status1 == DRIVER_STATUS_OK && status2 == DRIVER_STATUS_OK) {
            i_max = i;
        }
    }
    return i_max+jump;
}

static Driver* DriversMaxSort(Season season1){
    for (int i=0; i < season1->drivers_n; i++){
        SwapDrivers(&(season1->drivers)[i],
         &(season1->drivers)[DriversIndexOfMax((season1->drivers)+i,
          (season1->drivers_n)-i,i)]);
    }
    return season1->drivers;
}
//creates a season, the teams and drivers in it and connects each struct to its
//correct relation
Season SeasonCreate(SeasonStatus* status, const char* season_info){
    Season new_season = malloc(sizeof(*new_season));
    if(!new_season){
        free(new_season);
        if (status) {
            *status = SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    new_season=SeasonSetUp(status,new_season,season_info);
    MatchDriverTeam(new_season,season_info, status);
    return new_season;
}
//counts the teams and drivers given in a season and passes the relevant info
//to the Team and Driver Set functions
static Season SeasonSetUp(SeasonStatus* status,Season season1,
                          const char* season_info){
    int count,none_count=0;
    DriverStatus driver_status1=DRIVER_STATUS_OK;
    TeamStatus team_status1=TEAM_STATUS_OK;
    char* season_info_copy=malloc(sizeof(season_info)*strlen(season_info));
    strcpy(season_info_copy,season_info);
    char* line = strtok(season_info_copy, "\n");
    count=1;
    while((line=strtok(NULL,"\n")) != NULL) {
        if (strcmp(line, "None") == 0) {
            none_count++;
            count++;
            continue;
        }
        count++;
    }
    season1->teams_n = ((count-1)/3);
    season1->drivers_n = (season1->teams_n*2)-none_count;

    season1->teams = malloc(sizeof(*season1->teams)*(season1->teams_n));
    if(!season1->teams){
        if (status)  {
            *status = SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    season1->teams=SeasonSetTeamsInfo(status,team_status1,season1,season_info);

    season1->drivers = malloc(sizeof(*season1->drivers)*(season1->drivers_n));
    if(!season1->drivers){
        if (status)  {
            *status = SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    season1->drivers=SeasonSetDriversInfo(status,driver_status1,season1,
            season_info);
    free(season_info_copy);
    return season1;
}
//extracts the relevant information from season_info to create a driver
//sends that info to a creation function
static Driver* SeasonSetDriversInfo(SeasonStatus* status,
      DriverStatus driver_status1,Season season1,const char* season_info){
    int line_cnt=0,drivers_cnt=0;
    char* season_info_copy=malloc(sizeof(season_info)*strlen(season_info));
    strcpy(season_info_copy,season_info);
    char* line = strtok(season_info_copy, "\n");
    season1->year = atoi(line);
    line_cnt++;
    while((line=strtok(NULL,"\n")) != NULL){
        if((line_cnt%3)!=1) {
            if(strcmp(line,"None")==0) {
                line_cnt++;
                continue;
            }
            CreateSeasonDriver(status,driver_status1,season1,line,drivers_cnt);
            drivers_cnt++;
            line_cnt++;
            continue;
        }
        line_cnt++;
    }
    free(season_info_copy);
    if (status)  {
        *status = SEASON_OK;
    }
    return season1->drivers;
}
//extracts the relevant information from season_info to create a team
//sends that info to a creation function
static Team* SeasonSetTeamsInfo(SeasonStatus* status,TeamStatus team_status1,
                                Season season1,const char* season_info){
    int line_cnt=0,teams_cnt=0;
    char* season_info_copy=malloc(sizeof(season_info)*strlen(season_info));
    strcpy(season_info_copy,season_info);
    char* line = strtok(season_info_copy, "\n");
    season1->year = atoi(line);
    line_cnt++;
    while((line=strtok(NULL,"\n")) != NULL){
        if((line_cnt%3)==1) {
            CreateSeasonTeam(status,team_status1,season1,line,teams_cnt);
            teams_cnt++;
            line_cnt++;
            continue;
        }
        line_cnt++;
    }
    free(season_info_copy);
    if (status)  {
        *status = SEASON_OK;
    }
    return season1->teams;
}
//sets drivers in the matching team struct and the teams in the matching
//driver struct according to the season_info string
static void MatchDriverTeam(Season season, const char* season_info,
                            SeasonStatus* status){
    char* season_info_copy=malloc(sizeof(season_info)*strlen(season_info));
    strcpy(season_info_copy,season_info);
    char* line = strtok(season_info_copy, "\n");
    assert(season->year == atoi(line));
    while((line=strtok(NULL,"\n")) != NULL){
        Team curr_team=NULL;
        Driver curr_driver=NULL;
        for (int j = 0; j < season->teams_n ; j++) {
            if (!strcmp(line, TeamGetName(season->teams[j]))) {
                curr_team = season->teams[j];
            }
        }
        for (int i = 0; i < 2; ++i) {
            if((line=strtok(NULL,"\n")) != NULL){
                if(!strcmp(line,"None")) {
                    continue;
                }
                for (int j = 0; j < season->drivers_n; ++j) {
                    if (!strcmp(line, DriverGetName(season->drivers[j]))) {
                        curr_driver = season->drivers[j];
                    }
                }
                if(TeamAddDriver(curr_team,curr_driver) != TEAM_STATUS_OK){
                    free(season_info_copy);
                    if (status) {
                        *status = BAD_SEASON_INFO;
                    }
                    return;
                }
                DriverSetTeam(curr_driver, curr_team);
            }
        }
    }
    free(season_info_copy);
    if (status) {
        *status = SEASON_OK;
    }
}
//creates a team in a season
static void CreateSeasonTeam(SeasonStatus* status,TeamStatus team_status1,
                             Season season1,char* line,int teams_count){
    season1->teams[teams_count] = TeamCreate(&team_status1, line);
    if (team_status1 == TEAM_MEMORY_ERROR) {
        SeasonDestroy(season1);
        *status = SEASON_MEMORY_ERROR;
        return;
    }
}
//creates a driver in a season and sets the season in the driver struct
static void CreateSeasonDriver(SeasonStatus* status,DriverStatus driver_status1,
                               Season season1,char* line,int drivers_count){
    season1->drivers[drivers_count] = DriverCreate(&driver_status1,
            line, (drivers_count+1));
    DriverSetSeason(season1->drivers[drivers_count],season1);
    if(driver_status1 == DRIVER_MEMORY_ERROR) {
        SeasonDestroy(season1);
        *status = SEASON_MEMORY_ERROR;
        return;
    }
}

static void SwapTeams(Team* team1, Team* team2){
    Team temp_team = *team1;
    *team1 = *team2;
    *team2 = temp_team;
}
//max is set by points that the team has
static int IndexOfMaxTeams(Team* team_arr,int teams_size,int jump){
    TeamStatus status1, status2;
    int i, i_max = 0;
    for (i = 1; i < teams_size; ++i){
        if(TeamGetPoints(team_arr[i],&status1) > TeamGetPoints
                                 (team_arr[i_max],&status2) &&
           status1==TEAM_STATUS_OK && status2==TEAM_STATUS_OK){
            i_max = i;
        }
    }
    return i_max+jump;
}

static Team* TeamsMaxSort(Season season){
    for (int i=0; i < season->teams_n; i++){
        SwapTeams(&((season->teams)[i]),&((season->teams)
        [IndexOfMaxTeams((season->teams)+i,(season->teams_n-i),i)]));
    }
    return season->teams;
}
//finds the index of a driver in drivers array by his id
static int FindIndexDrivers(Season season1,int id) {
    for (int k = 0; k < season1->drivers_n; ++k) {
        if ((DriverGetId(season1->drivers[k]) == id)) {
            return k;
        }
    }
    return -1;
}
//finds the index of a team in teams_n array by his id its name
static int FindIndexTeams(Season season1,const char* name) {
    for (int k = 0; k < season1->teams_n; ++k) {
        if (strcmp(TeamGetName(season1->teams[k]), name) == 0) {
            return k;
        }
    }
    return -1;
}
//finds the index of a driver in results array by his id
static int FindIndexResults(Season season1,int id,int* results) {
    for (int k = 0; k < season1->drivers_n; ++k) {
        if (results[k] == id) {
            return k;
        }
    }
    return -1;
}
//counts the amount of drivers in a row with the same amount of points
static int CountPointDrivers(Season season1,int* index) {
    DriverStatus driverStatus1=DRIVER_STATUS_OK;
    int counter=0,point_value=DriverGetPoints(season1->drivers[*index],
            &driverStatus1);
    while((*index<season1->drivers_n) && (DriverGetPoints
          (season1->drivers[*index],&driverStatus1)==point_value)){
      counter++;
      (*index)++;
    }
    return counter;
}
//sorts an array of drivers that have the same amount of points in order
//by the results from the last race
static Driver* DriversMaxSortAfterPoints(Season season1,int* results) {
    int index = 0, counter = 0;
    while (index < (season1->drivers_n)) {
        counter = CountPointDrivers(season1, &index);
        if(counter==1){
            counter=0;
            continue;
        }
        int *id_points = malloc(sizeof(int) * counter);
        int balance = counter;
        for (int i = 0; i < counter; i++) {
            id_points[i] = FindIndexResults(season1,
                    DriverGetId(season1->drivers[index - balance]), results);
            if (balance > 0) {
                balance--;
            }
        }
        IdMaxSort(id_points, counter);
        balance = counter;
        for (int i = 0; i < counter; i++) {
            SwapDrivers(&(season1->drivers[index - balance]),
                        &(season1->drivers[FindIndexDrivers(season1,
                        FindIdByPosition(id_points[i], season1, results))]));
            if (balance > 0) {
                balance--;
            }
        }
        counter = 0;
        free(id_points);
    }
    return season1->drivers;
}
//finds the id of a driver by his position in the last race
static int FindIdByPosition(int position,Season season1,int* results){
    for(int i=0;i<season1->drivers_n;i++){
        if(position==FindIndexResults(season1, DriverGetId(season1->drivers[i]),
            results)){
            return DriverGetId(season1->drivers[i]);
        }
    }
    return -1;
}

static int IndexOfMaxId(int* id, int counter) {
    int i, i_max = 0;
    for (i = 1; i < counter; i++) {
        if (id[i] > id[i_max]) {
            i_max = i;
        }
    }
    return i_max;
}

static void IdMaxSort(int* id, int counter) {
    for (int i = counter; i > 1; i--) {
        int i_max=IndexOfMaxId(id,i);
        SwapId(&id[i-1],&id[i_max]);
    }

}

static void SwapId(int * id1, int* id2){
    int temp_id = *id1;
    *id1 = *id2;
    *id2 = temp_id;
}
//counts how many teams in a row have the same amount of points (only relevant
// after the teams are in order by points)
static int CountPointTeams(Season season1,int* index) {
    TeamStatus team_status =TEAM_STATUS_OK;
    int counter=0,point_value=TeamGetPoints(season1->teams[*index],
            &team_status);
    while((*index<season1->teams_n) && (TeamGetPoints(season1->teams[*index],
            &team_status)==point_value)){
        counter++;
        (*index)++;
    }
    return counter;
}
//sorts an array of teams that have the same amount of points in order
//by the best driver in the array results
static Team* TeamsMaxSortAfterPoints(Season season1,int* results) {
    int index = 0, counter = 0,number_of_drivers=0;
    while (index < (season1->teams_n)) {
        counter = CountPointTeams(season1, &index);
        if(counter==1){
            counter=0;
            continue;
        }
        int balance = counter;
        DriverCounter(season1,&index,&balance,&number_of_drivers,&counter);
        int *id_points = malloc(sizeof(int) * (number_of_drivers));
        balance = counter;
        FillResultsId(season1,results,id_points,&balance,&index,
                      number_of_drivers);
        IdMaxSort(id_points, number_of_drivers);
        balance = counter;
        char** team_cmp=calloc((size_t) number_of_drivers,sizeof(char*));
        if(!team_cmp){
            return NULL;
        }
        for (int i = 0; i < number_of_drivers; i++) {
            Team team_tmp=DriverGetTeam(season1->drivers[FindIndexDrivers
               (season1,FindIdByPosition(id_points[i], season1, results))]);
            int num=FindIndexTeams(season1,TeamGetName(team_tmp));
            team_cmp[i]=malloc(sizeof(char)*(strlen(TeamGetName(team_tmp))+1));
            if(!team_cmp[i]){
                return NULL;
            }
            strcpy(team_cmp[i],TeamGetName(team_tmp));
            if(TeamCmp(team_cmp,TeamGetName(team_tmp),number_of_drivers)){
                continue;
            }
                SwapTeams(&(season1->teams[index - balance]),
                          &(season1->teams[num]));
                if (balance > 0) {
                    balance--;
                }
        }
        DestroyTempTeams(team_cmp,number_of_drivers);
        counter = 0;
        free(id_points);
    }
    return season1->teams;
}
//compares two team names
static int TeamCmp(char** team1, const char* team2, int size){
    int counter =0;
    for (int j = 0; j < size; ++j) {
        if(team1[j] && !strcmp(team1[j],team2)) {
            counter++;
        }
    }
    if(counter > 1)
        return 1;
    return 0;
}
//destroys an array of teams
static void DestroyTempTeams(char** teams,int size){
    for (int i = 0; i < size; ++i) {
        free(teams[i]);
    }
    free(teams);
}
//fills the id of the drivers that come from groups with the same amount of
//points
static void FillResultsId(Season season1,int* results,int* id_points,
                          int* balance,int* index,int number_of_drivers) {
    for (int i = 0; i < number_of_drivers;) {
        if (DriverGetId(TeamGetDriver(season1->teams[*index - *balance],
                                      FIRST_DRIVER))) {
            id_points[i] = FindIndexResults(season1, DriverGetId
                    (TeamGetDriver(season1->teams[*index - *balance],
                     FIRST_DRIVER)), results);
            i++;
        }
        if (DriverGetId(TeamGetDriver(season1->teams[*index - *balance],
            SECOND_DRIVER))) {
            id_points[i] = FindIndexResults(season1,
            DriverGetId(TeamGetDriver(season1->teams[*index - *balance],
            SECOND_DRIVER)), results);
            i++;
        }
        if (*balance > 0) {
            (*balance)--;
        }
    }
}

//counts the drivers in a group of teams that have the same amount of points
static void DriverCounter(Season season1, int* index, int* balance,
                          int* number_of_drivers, int* counter) {
    for (int k = 0; k < *counter; ++k) {
        if (DriverGetId(TeamGetDriver(season1->teams[*index - *balance],
                                      FIRST_DRIVER))) {
            (*number_of_drivers)++;
        }
        if (DriverGetId(TeamGetDriver(season1->teams[*index - *balance],
                                      SECOND_DRIVER))) {
            (*number_of_drivers)++;
        }
        if (*balance > 0) {
            (*balance)--;
        }
    }
}