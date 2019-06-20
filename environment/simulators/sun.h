#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
#include <iostream>
#include <cassert>
class Sun {
public:
	Sun(int year, int month, int day, int hour, int minute, int second, float longitude, float latitude);
	void updateTime(int year, int month, int day, int hour, int minute, int second);
	void updatePosition(float longitude, float latitude);
	void getResult();
	float get_sunAzimuth() const;
	float get_solarAltitude() const;
	float get_hourlyIrradiance() const;
	float _degree_to_radians(float _degree) const;
	float _radians_to_degree(float _radians) const;
private:
	const float PI = 3.14159265359f;
	int _year, _month, _day, _hour, _minute, _second;
	float _longitude, _latitude;
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

	float Ic, epsilon_0, Ic_prime, a, b, I_et_d, b_0, b_1, I_t_d, s;

	float I_df_d, I_dr_d;

	float psi, A_prime, B_prime, I_t, I_et;

	float R, K, I_df, I_dr;
};
#endif