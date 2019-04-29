// Copyright 2019

#ifndef COMPUTATIONAL_AGROECOLOGY_WEATHER_H
#define COMPUTATIONAL_AGROECOLOGY_WEATHER_H

// Weather specify condition on everyday
class Weather {
  public :
    DayWeather( double TMax = 0, double TMin = 0,
                double RFall = 0) :
            temp_min_(TMax), temp_max_(TMin),
            rainfall_(RFall) { }
    double temp_max() {return temp_max_;}
    double temp_min() {return temp_min_;}
    double rainfall() {return rainfall_;}

  private :
    double temp_min_;
    double temp_max_;
    double rainfall_;
};

#endif //COMPUTATIONAL_AGROECOLOGY_WEATHER_H