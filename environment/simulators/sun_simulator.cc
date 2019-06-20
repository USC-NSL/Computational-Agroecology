#include "sun_simulator.h"
#include "../environment.h"
#include <cmath>
#include <iostream>
#include <cassert>
#include <ctime>

namespace simulator {
	SunSimulator::SunSimulator(int year, int month, int day, int hour, int minute, int second, float longitude, float latitude)
		:_year(year), _month(month), _day(day), _hour(hour), _second(second), _longitude(longitude), _latitude(latitude) {
	}

	void SunSimulator::check() {
		assert(_year > 0);
		assert(_month > 0 && _month <= 12);
		assert(_day > 0 && _day <= 31);
		assert(_hour >= 0 && _hour <= 23);
		assert(_minute >= 0 && _minute <= 59);
		assert(_second >= 0 && _second <= 59);
		assert(_longitude > -180.0f && _longitude <= 180.0f);
		assert(_latitude >= -90.0f && _latitude <= 90.0f);
	}

	void SunSimulator::SimulateToTime(
		environment::Environment* env,
		const std::chrono::system_clock::time_point& time) {
		time_t tt = std::chrono::system_clock::to_time_t(time); 
		struct tm* tm = localtime(&tt);
		updateTime(tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		updatePosition(env->longitude_, env->latitude_);
		check();
		getResult();
		env->solarAltitude = get_solarAltitude(); 
		env->sunAzimuth = get_sunAzimuth();
		env->hourlyIrradiance = get_hourlyIrradiance();
	}

	void SunSimulator::updateTime(int year, int month, int day, int hour, int minute, int second) {
		_year = year;
		_month = month;
		_day = day;
		_hour = hour;
		_minute = minute;
		_second = second;
	}

	void SunSimulator::updatePosition(float longitude, float latitude) {
		_longitude = longitude;
		_latitude = latitude;
	}

	void SunSimulator::getResult() {
		check();
		first_step();
		second_step();
		third_step();
		fourth_step();
		fifth_step();
		sixth_step();
	}

	float SunSimulator::get_sunAzimuth() const {
		return alpha + PI;
	}

	float SunSimulator::get_solarAltitude() const {
		return PI / 2.0f - beta;
	}

	float SunSimulator::get_hourlyIrradiance() const {
		return I_t;
	}

	float SunSimulator::_degree_to_radians(float _degree) const {
		return _degree * PI / 180.0f;
	}

	float SunSimulator::_radians_to_degree(float _radians) const {
		return _radians / PI * 180.0f;
	}

	void SunSimulator::first_step() {
		is_leapYear = (((_year % 4 == 0) && (_year % 100 != 0)) || _year % 400 == 0);
		t_d = _day;
		switch (_month) {
		case 12:t_d += 30;
		case 11:t_d += 31;
		case 10:t_d += 30;
		case 9:t_d += 31;
		case 8:t_d += 31;
		case 7:t_d += 30;
		case 6:t_d += 31;
		case 5:t_d += 30;
		case 4:t_d += 31;
		case 3:t_d += 28 + is_leapYear;
		case 2:t_d += 31;
		case 1:break;
		default:break;
		}
		sigma = -23.45f * PI / 180.0f * cos(2.0f * PI * (t_d + 10) / (365 + is_leapYear));
		gama_standard_meridian = ((int)(_longitude / PI * 12)) * PI / 12;
		B = 2.0f * PI * (t_d - 81) / (364 + is_leapYear);
		EoT = 9.87f * sin(2 * B) - 7.53f * cos(B) - 1.5f * sin(B);
		th = _hour + (gama_standard_meridian - _degree_to_radians(_longitude)) / PI * 12 + EoT / 60;
		delta = PI / 12.0f * (th - 12);
		lamda = _degree_to_radians(_latitude);
		beta = asin(sin(sigma) * sin(lamda) + cos(sigma) * cos(lamda) * cos(delta));
		alpha = acos((sin(lamda) * sin(beta) - sin(sigma)) / (cos(lamda) * cos(beta)));
		if (_hour < 12) {
			alpha *= -1.0f;
		}
	}

	void SunSimulator::second_step() {
		t_ss = 12 + 12 / PI * acos(-(sin(sigma) * sin(lamda)) / (cos(sigma) * cos(lamda)));
		t_sr = 24 - t_ss;
		DL = 2 * (t_ss - 12);
	}

	void SunSimulator::third_step() {
		Ic = 1370.0f;
		epsilon_0 = 1.0f + 0.033f * cos(2.f * PI * (t_d - 10) / (365 + is_leapYear));
		Ic_prime = Ic * epsilon_0;
		a = sin(lamda) * sin(sigma);
		b = cos(lamda) * cos(sigma);
		I_et_d = 3600.0f * Ic_prime * 24 / PI * (a * acos(-a / b) + b * sqrt(1 - (a / b) * (a / b)));
		{b_0 = 0.18f, b_1 = 0.55f; }//Cold or temperate
		{b_0 = 0.25f, b_1 = 0.45f; }//Dry tropical
		{b_0 = 0.29f, b_1 = 0.42f; }//Humid tropical
		{s = DL; } // have all sunshine in the day time
		I_t_d = I_et_d * (b_0 + b_1 * s / DL);
	}

	void SunSimulator::fourth_step() {
		if (I_t_d / I_et_d < 0.07f)
			I_df_d = I_t_d;
		else if (I_t_d / I_et_d < 0.35f)
			I_df_d = I_t_d * (1 - 2.3f * (I_t_d / I_et_d - 0.07f) * (I_t_d / I_et_d - 0.07f));
		else if (I_t_d / I_et_d < 0.75)
			I_df_d = I_t_d * (1.33f - 1.46f * I_t_d / I_et_d);
		else
			I_df_d = I_t_d * 0.23f;
		I_dr_d = I_t_d - I_df_d;
	}

	void SunSimulator::fifth_step() {
		psi = PI * I_t_d / 86400 / (a * acos(-a / b) + b * sqrt(1 - (a / b) * (a / b)));
		A_prime = -b * psi;
		B_prime = a * psi;
		I_t = 86400 / PI * (B_prime * acos(-a / b) - A_prime * sqrt(1 - (a / b) * (a / b)));
		I_et = Ic_prime * sin(beta);
	}

	void SunSimulator::sixth_step() {
		R = (0.847f - 1.61f * sin(beta) + 1.04f * sin(beta) * sin(beta));
		K = (1.47f - R) / 1.66f;
		if (I_t / I_et <= 0.22f)
			I_df = I_t;
		else if (I_t / I_et <= 0.35f)
			I_df = I_t * (1 - 6.4f * (I_t / I_et - 0.22f) * (I_t / I_et - 0.22f));
		else if (I_t / I_et <= K)
			I_df = I_t * (1.47f - 1.66f * I_t / I_et);
		else
			I_df = I_t * R;
		I_dr = I_t - I_dr;
	}
}