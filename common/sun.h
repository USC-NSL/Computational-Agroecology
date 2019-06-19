#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
#include <iostream>
#include <cassert>
class Sun {
public:
	Sun(int year, int month, int day, int hour, int minute, int second, float longitude, float latitude)
		:_year(year), _month(month), _day(day), _hour(hour), _second(second), _longitude(longitude, _latitude(latitude){
		assert(year > 0);
		assert(month > 0 && month <= 12);
		assert(day > 0 && day <= 31);
		assert(hour >= 0 && hour <= 23);
		assert(minute >= 0 && minute <= 59);
		assert(second >= 0 && second <= 59);
		assert(longitude > -180.0f && longtitude <= 180.0f);
		assert(latitude >= -90.0f && latitude <= 90.0f);
	}
private:
	const float PI = 3.14159265359f;
	int _year, _month, _day, _hour, _minute, _second;
	float _longitude, _latitude;
	float _degree_to_radians const (float _degree);
	float _radians_to_degree const (float _radians);
	void first_step();
	void second_step();
	void third_step();
	void fourth_step();
	void fifth_step();
	void sixth_step();

	bool is_leapYear;
	int t_d;
	float sigma, gama_standard_meridian, th, delta, B, EoT, solar_angel_radians, lamda, beta, alpha;

	float t_ss, t_sr, DL;

	float Ic, epsilon_0, Ic_prime, a, b, I_et_d, b_0, b_1, I_t_d;
	int s;

	float I_df_d, I_dr_d;

	float psi, A, B, I_t, I_et;

	float R, K, I_df, I_dr;
}
#endif