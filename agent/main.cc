#include <iostream>

// A configuration is defined by a list of actions taken over a time period

class Config {
    Location current_location;
    Action action_list;
};

// This function returns a full agro-ecosystem design given a configuration.
// A design is defined by crop spatial position & *actions taken per crop.

void DoFullDesign(){

    cout << "This will return the full design";

}

// This function returns the initial layout of the land as a *placement,
// taking into account what is already on the land as given by the config

void InitialPlacement(){
    cout << "Initial Placement";
}

// This function returns best action for a given timestep

void PlacementAction(){
    cout << "Best Action";
}

// This function calls the simulation tool that returns current conditions/result from a placement action

void Simulate(){
    cout << "Result from placement action";
}

int main() {

    // Get input values i.e location

    // Get the Config

    // Get the agro-ecosystem design
    DoFullDesign();

    return 0;
}
