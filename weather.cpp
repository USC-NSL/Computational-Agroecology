// Copyright 2019

#include "weather.h"
bool tmin_comp(DayWeather a, DayWeather b)
{

    return a.temp_min() < b.temp_min();
}
bool tmax_comp(DayWeather a, DayWeather b)
{
    return a.temp_max() < b.temp_max();
}
bool rainfall_comp(DayWeather a, DayWeather b)
{
    return a.rainfall() < b.rainfall();
}

// Allow user to get weather information according to their location
Weather WeatherParser::GetWeatherByLocation(Location location) {
// Todo(Xiao): Add impl.
	CSVReader reader("../weather data/90007_weather.csv"); 
	std::vector<std::vector<std::string> > data_list = reader.getData();
	std::vector<DayWeather> dweather_list; 


	for(int i = 1; i < data_list.size();i++){
		auto data = data_list[i];
		//std::cout<<data[5]<<"-------------\n";

		DayWeather d_weather(std::stod(data[7]),std::stod(data[6]),std::stod(data[5]));


		dweather_list.push_back(d_weather);
		//DayWeather d = (0,0,0);
	}
	// for(auto a : dweather_list){
	// 		std::cout<<a.temp_min()<<"|"<<a. temp_max() <<"|"<<a.rainfall()<<std::endl;
	// 	}
	
	Weather weather(dweather_list);
	return weather; //
}
Weather::Weather(std::vector<DayWeather> dweather_list):
    day_weather_list_(dweather_list){
    min_temp_year_ =  (*std::min_element(day_weather_list_.begin(), day_weather_list_.end(), tmin_comp)).temp_min();
     max_temp_year_ = (*std::max_element(day_weather_list_.begin(), day_weather_list_.end(), tmax_comp)).temp_max();

    min_rainfall_year_ = (*std::min_element(day_weather_list_.begin(), day_weather_list_.end(), rainfall_comp)).rainfall();

   max_rainfall_year_ = (*std::max_element(day_weather_list_.begin(), day_weather_list_.end(), rainfall_comp)).rainfall();
   // std::cout<<max_rainfall_year_<<std::endl;

}