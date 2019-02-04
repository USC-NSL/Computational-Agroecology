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

void simulate(cell** landscape, int num_rows, int num_cols, Action action) {
    //perform action
    //assumption made: we only perform this action once in the year
    if ( action.type() == ADD_CROP ) {
        //find first empty spot; assumes that we initialize plant_state to -1 if no plant is there
        for ( int i  = 0; i < num_rows; i++ ) {
            for ( int j = 0; j < num_cols; j++ ) {
                if ( landscape[i][j].plant_state == -1 ) {
                    //ADD_CROP - unsure of this impl.
                }
            }
        }
    }
    else if ( action.type() == ADD_WATER || action.type() == REMOVE_CROP ) {
        for ( int i  = 0; i < num_rows; i++ ) {
            for ( int j = 0; j < num_cols; j++ ) {
                if ( landscape[i][j].plant_state != -1 ) {
                    if ( action.type() == ADD_WATER ) {
                        // ADD_WATER - unsure of this impl.
                    }
                    if ( action.type() == REMOVE_CROP ) {
                        // REMOVE_CROP - unsure of this impl.
                    }
                }
            }
        }
    }
    
    //simulate weather and plant growth for one year
    int weeks = 0;
    int season = 0;
    //2d array holding min and max values of rain and sun per season, arbitrary data for now on scale of 0-4
    int** rain_values = [[1, 4], [0, 2], [1, 3], [1, 4]];
    int** sun_values = [[1, 3],[2, 4],[1, 2],[0, 1]];
    //start the year in spring
    while ( weeks < 52 ) {
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
        (rand() % (rain_values[season][1] - rain_values[season][1] + 1));
        
        sun = sun_values[season][0] +
        (rand() % (sun_values[season][1] - sun_values[season][1] + 1));
        
        //update growth
        for ( int i = 0; i < num_rows; i++ ) {
            for ( int j = 0; j < num_cols; j++ ) {
                landscape[i][j].plant.transition(/*paramter is int env[], but should be changed to reflect rain/sun*/);
            }
        }
        
        //time passes
        weeks++;
    }
    
}
