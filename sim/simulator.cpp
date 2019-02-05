//
//  simulator.cpp
//  
//
//  Created by Julia Cordero on 2/3/19.
//

#include <stdio.h>
#incldue <iostream>
#include "cell.hpp"
#include <random>
#include <ctime>
#include "plant.h"
using namespace std;


void SimulateAction(cell** landscape, Action action, int row, int col) {
    landscape[row][col] = action;
}

void RunPlantGrowth(cell** lanscape, int num_rows, int num_cols) {
    for ( int i = 0; i < num_rows; i++ ) {
        for ( int j = 0; j < num_cols; j++ ) {
            landscape[i][j].plant.transition(/*paramter is int env[], but should be changed to reflect rain/sun*/);
        }
    }
}

void GenerateWeather(int& rain, int& sun, int& weeks, int& season, int**& rain_values, int**& sun_values) {
    if ( weeks % 13 == 0 ) {
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

void simulate(cell** landscape, int num_rows, int num_cols, Action action, int row, int col) {
    //perform action
    SimulateAction(landscape, row, col);
    
    //initialize variables for yearlong sim - can do this outside the function?
    int weeks = 0;
    int season = 0;

    int** rain_values = [[1, 4], [0, 2], [1, 3], [1, 4]];
    int** sun_values = [[1, 3],[2, 4],[1, 2],[0, 1]];
    int rain, sun = 0;
    
    //start the year in spring
    while ( weeks < 52 ) {
        //sim weather
        GenerateWeather(rain, sun, weeks, season, rain_values, sun_values);
        
        //change season & type of weather
        int rain, sun;
        //increment season
        if ( weeks % 13 == 0 ) {
            season++;
        }
        //seed the random value
        srand(time(0));
        //create rain and sun values for weather
        rain = rain_values[season][0] +
        (rand() % (rain_values[season][1] - rain_values[season][0] + 1));
        
        sun = sun_values[season][0] +
        (rand() % (sun_values[season][1] - sun_values[season][0] + 1));
        

        //update growth
        RunPlantGrowth(landscape, num_rows, num_cols);
        //time passes
        weeks++;
    }
    
}
