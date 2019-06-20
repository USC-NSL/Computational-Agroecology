#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H

#include "simulator.h"

namespace simulator {

class SunSimulator : public Simulator{
public:
	SunSimulator(int year = 0, int month = 0, int day = 0, int hour = 0, int minute = 0, int second = 0, float longitude = 0.0f, float latitude = 0.0f);
	void SimulateToTime(
		environment::Environment* env,
		const std::chrono::system_clock::time_point& time) override;
private:
	void check();
	void updateTime(int year, int month, int day, int hour, int minute, int second);
	void updatePosition(float longitude, float latitude);
	void getResult();
	float get_sunAzimuth() const;
	float get_solarAltitude() const;
	float get_hourlyIrradiance() const;
	float _degree_to_radians(float _degree) const;
	float _radians_to_degree(float _radians) const;
	const float PI = 3.14159265359f;
	int _year, _month, _day, _hour, _minute, _second;
	float _longitude, _latitude;
	void first_step();
	void second_step();
	void third_step();
	void fourth_step();
	void fifth_step();
	void sixth_step();

	//variables for the first step
	bool is_leapYear;
	int t_d;
	float sigma, gama_standard_meridian, th, delta, B, EoT, solar_angel_radians, lamda, beta, alpha;

	//variables for the second step
	float t_ss, t_sr, DL;

	//varibales for the third step
	float Ic, epsilon_0, Ic_prime, a, b, I_et_d, b_0, b_1, I_t_d, s;

	//variables for the fourth step
	float I_df_d, I_dr_d;

	//variables for the fifth step
	float psi, A_prime, B_prime, I_t, I_et;

	//variables for the sixth step
	float R, K, I_df, I_dr;
};

} // namespace simulator
#endif COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SUN_H
