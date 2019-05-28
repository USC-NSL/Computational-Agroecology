// Copyright 2019
//#include <time.h>
#include <iostream>
#include "../common/config.h"
#include "../common/config.cpp"
#include <string>
#include <vector>

// This file is temporarily here to represent the game which allow user to interact with component
void sleepcp(int milliseconds) // Cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}

enum PlantOption {
    CORN,
    BEAN,
    SQUASH
};

std::vector<std::string> split(const std::string& s, const std::string& delimiter)
{
    std::vector<std::string> tokens;

    for (size_t start = 0, end; start < s.length(); start = end + delimiter.length())
    {
        size_t position = s.find(delimiter, start);
        end = position != std::string::npos ? position : s.length();

        std::string token = s.substr(start, end - start);
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

ActionAdapter* chooseAction(std::string action, std::string crop, int x, int y, int time_tick, int duration)
{
    PlantType *newCrop;
    if (crop == "Corn")
        newCrop = new Corn();
    if (crop == "Squash")
        newCrop = new Squash();
    if (crop == "Bean")
        newCrop = new Bean();
    if (action == "AddCrop") {
        AddCrop *addCrop1 = new AddCrop(newCrop, x, y);
        return addCrop1;
    }

}

int main()
{
    int day = 0; // initialize the start of day
    Location loc = Location(100, 200);
    std::vector<PlantType> v;
    Config config = Config(loc, v);
    std::string action_input;
    while(true) {
        sleepcp(3000);
        Corn *corn = new Corn();
        AddCrop *addCrop1 = new AddCrop(corn, day, day);
        config.add_daily_action(addCrop1);
        day++;
        while(true){
            std::cout << "Input all the actions for the day:"
                      << std::endl << "(format: actiontype/plantType/coordination_x/coordination_y,time_tick/duration)" << std::endl
                      << "Action Type options:AddCrop/RemoveCrop/WaterCrop/HarvestCrop" << std::endl <<
                      "Plant Type option:Corn/Bean/Squash" <<std::endl << "coordination <= 10"
                                                                          "time tick <= 23, " << "duration <=24"
                      << std::endl;
            std::cin >> action_input;
            if (action_input == "end")
            {
                break;
            }
            if (action_input == "harvest")
            {
                config.harvest_produce();
                break;
            }
            std::vector<std::string> string_vector = split(action_input, "/");
            config.add_daily_action(chooseAction(string_vector[0], string_vector[1], std::stoi(string_vector[2]),
                                                 std::stoi(string_vector[3]), std::stoi(string_vector[4]), std::stoi(string_vector[5])));
            std::cout << string_vector[0];
        }
        config.perform_daily_actions();
        config.state_display(day);

    }

}