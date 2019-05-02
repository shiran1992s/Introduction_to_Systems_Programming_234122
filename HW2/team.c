#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "team.h"



struct team{
    char* new_name;
    Driver driver1;
    Driver driver2;
};


Team TeamCreate(TeamStatus* status, char* name) {
    if(!name){
        if(status) {
            *status = TEAM_MEMORY_ERROR;
        }
        return NULL;
    }
    Team new_team = malloc(sizeof(*new_team));
    if (!new_team) {
        if(status) {
            *status = TEAM_MEMORY_ERROR;
        }
        return NULL;
    }
        new_team->driver1=NULL;
        new_team->driver2=NULL;
    if(status) {
        *status = TEAM_STATUS_OK;
    }

    new_team->new_name = malloc(sizeof(new_team->new_name)*strlen(name));
    strcpy(new_team->new_name, name);
    return new_team;
}


void TeamDestroy(Team team){
    if(team!=NULL){
        DriverDestroy(team->driver1);
        DriverDestroy(team->driver2);
        free(team->new_name);
        free(team);
    }
}


TeamStatus TeamAddDriver(Team team, Driver driver){
    if (!team || !driver) {
        return TEAM_NULL_PTR;
    }
    if(!team->driver1){
        team->driver1=driver;
        return TEAM_STATUS_OK;
    } else if (!team->driver2){
        if(strcmp(DriverGetName(team->driver1),DriverGetName(driver))==0){
            return TEAM_MEMORY_ERROR;
        }
        team->driver2=driver;
        return TEAM_STATUS_OK;
    } else {
        return TEAM_FULL;
    }
}


const char * TeamGetName(Team  team){
    if(!team){
        return NULL;
    } else{
        return team->new_name;
    }
}


Driver TeamGetDriver(Team  team, DriverNumber driver_number){
    if(!team || (driver_number!=FIRST_DRIVER && driver_number!=SECOND_DRIVER)){
        return NULL;
    }
    if (driver_number==FIRST_DRIVER){
        if(!team->driver1){
            return NULL;
        } else {
            return team->driver1;
        }
    } else {
        assert(driver_number == SECOND_DRIVER);
        if (!team->driver2) {
            return NULL;
        } else {
            return team->driver2;
        }
    }
}

int TeamGetPoints(Team  team, TeamStatus *status){
    if (!team ){
        *status=TEAM_NULL_PTR;
        return 0;
    }
    DriverStatus current_status1,current_status2;
    int total_points,driver1_pts=DriverGetPoints(team->driver1,&current_status1)
    ,driver2_pts=DriverGetPoints(team->driver2,&current_status2);
    if (current_status1!=DRIVER_STATUS_OK && current_status2!=DRIVER_STATUS_OK){
        *status=TEAM_STATUS_OK;
        return 0;
    } else if (current_status1!=DRIVER_STATUS_OK){
        total_points=driver2_pts;
        *status=TEAM_STATUS_OK;
        return total_points;
    } else if(current_status2!=DRIVER_STATUS_OK){
        total_points=driver1_pts;
        *status=TEAM_STATUS_OK;
        return total_points;
    }
    total_points=driver1_pts+driver2_pts;
    *status=TEAM_STATUS_OK;
    return total_points;
}