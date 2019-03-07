//
//  simulator.cpp
//  
//
//  Created by Julia Cordero on 2/3/19.
//

#include <stdio.h>
#include <iostream>
#include "cell.h"
#include <random>
#include <ctime>
#include <action.h>
#include "plant.h"
using namespace std;


void simulateAction(Cell **landscape, const Action& action, int row, int col) {
    //TODO: How are actions applied to cells? I don't think we should overload
    // the '=' operator to do so, and currently there are no fields in Cell
    // that would handle an operation being done on them

    // landscape[row][col] = action;
}

void runPlantGrowth(Cell **landscape, int num_rows, int num_cols, int *env) {
    for ( int i = 0; i < num_rows; i++ ) {
        for ( int j = 0; j < num_cols; j++ ) {
            landscape[i][j].plant.transition(env[0], env[1], env[2]);
        }
    }
}

void generateWeather(int &rain, int &sun, int &weeks, int &season,
                     int **&rain_values, int **&sun_values) {
    if (weeks % 13 == 0) {
        season++;
    }
    //seed the random value
    srand(time(0));
    //create rain and sun values for weather
    rain = rain_values[season][0] +
    (rand() % (rain_values[season][1] - rain_values[season][1] + 1));
    
    sun = sun_values[season][0] +
    (rand() % (sun_values[season][1] - sun_values[season][1] + 1));
}

void simulate(Cell **landscape, int num_rows, int num_cols, const Action& action,
              int row, int col) {
    //perform action
    simulateAction(landscape, action, row, col);
    
    //initialize variables for yearlong sim - can do this outside the function?
    int weeks = 0;
    int season = 0;

    int rain_values[4][2] = { {1, 4} , {0, 2}, {1, 3}, {1, 4} };
    int sun_values[4][2] = { {1, 3}, {2, 4}, {1, 2}, {0, 1} };
    int rain = 0;
    int sun = 0;
    
    int env[2] = {rain, sun};
    
    //start the year in spring
    while ( weeks < 52 ) {
        //sim weather
        //TODO: No matching function call. How should this be done
        //generateWeather(rain, sun, weeks, season, rain_values, sun_values);

        //update growth
        runPlantGrowth(landscape, num_rows, num_cols, env);
        //time passes
        weeks++;
    }
    
}
