//
// Created by Elizabeth Ondula on 2019-01-22.
//

#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_H
#define COMPUTATIONAL_AGROECOLOGY_AGENT_H
#include "landscape.h"

class agent {
public:
    void SelectAction(LandscapeCell cell);
    double GetEvaluation();
};


#endif //COMPUTATIONAL_AGROECOLOGY_AGENT_H
