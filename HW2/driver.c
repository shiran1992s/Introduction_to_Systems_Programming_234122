#include "driver.h"
#include <malloc.h>
#include <string.h>

struct driver {
    unsigned int id;
    char* name;
    Team team;
    unsigned int points;
    Season season;
};
Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId){
    if(driverId<=0){
        if(status) {
            *status = INVALID_DRIVER;
        }
        return NULL;
    }

    Driver new_driver = malloc(sizeof(*new_driver));
    if(!new_driver){
        free(new_driver);
        if(status) {
            *status = DRIVER_MEMORY_ERROR;
        }
        return new_driver;
    }
    new_driver->name = malloc(sizeof(driver_name)*strlen(driver_name));
    if(!new_driver->name) {
        if (status) {
            *status = DRIVER_MEMORY_ERROR;
        }
    }
    strcpy(new_driver->name, driver_name);
    new_driver->id = (unsigned int)driverId;
    new_driver->season = NULL;
    new_driver->team = NULL;
    new_driver->points = 0;

    if(status) {
        *status = DRIVER_STATUS_OK;
    }
    return new_driver;
}

void DriverDestroy(Driver driver){
    if(!driver){
        return;
    }
    free(driver->name);
    free(driver);
}

const char* DriverGetName(Driver driver){
    if(!driver) {
        return NULL;
    }
    return driver->name;
}

Team DriverGetTeam(Driver driver){
    if(!driver) {
        return NULL;
    }
    return driver->team;
}

int DriverGetId(Driver driver){
    if(!driver) {
        return 0;
    }
    return driver->id;
}

void DriverSetTeam(Driver driver, Team team){
    if(!driver || !team) {
        return;
    }
    driver->team = team;
}

void DriverSetSeason(Driver driver, Season season){
    if(!driver || !season) {
        return;
    }
    driver->season = season;
}

DriverStatus DriverAddRaceResult(Driver driver, int position){
    if(!driver) {
        return INVALID_DRIVER;
    }
    if(!(driver->season)) {
        return SEASON_NOT_ASSIGNED;
    }
    if(position<=0) {
        return INVALID_POSITION;
    }
    driver->points+=((SeasonGetNumberOfDrivers(driver->season))-position);
    return DRIVER_STATUS_OK;
}

int DriverGetPoints(Driver driver, DriverStatus* status){
    if(!driver){
        if(status) {
            *status = INVALID_DRIVER;
        }
        return 0;
    }
    if(status) {
        *status = DRIVER_STATUS_OK;
    }
    return driver->points;
}

