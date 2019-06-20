#include "sun.h"
#include <cmath>

float Sun::_degree_to_radians const(float _degree) {
	return _degree * PI / 180.0f;
}

float Sun::_radians_to_degree const(float _radians) {
	return _radians / PI * 180.0f;
}

void Sun::first_step() {
	is_leapYear = (((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0);
	t_d = 0;
	switch (month) {
	case 1:break;
	case 2:t_d += 31;
	case 3:t_d += 28 + is_leapYear;
	case 4:t_d += 31;
	case 5:t_d += 30;
	case 6:t_d += 31;
	case 7:t_d += 30;
	case 8:t_d += 31;
	case 9:t_d += 31;
	case 10:t_d += 30;
	case 11:t_d += 31;
	case 12:t_d += 30; break;
	default:break;
	}
	sigma = -23.45f * PI / 180.0f * cos(2.0f * PI * (t_d + 10) / (365 + is_leapYear));
	gama_standard_meridian = ((int)(_longitude / PI * 12)) * PI / 12;
	th = _hour + (gama_standard_meridian - _degree_to_radians(_longitude)) / PI * 12 + EoT;
	delta = PI / 12 * (th - 12);
	B = 2.0f * PI * (_day - 81) / (364 + is_leapYear);
	EoT = 9.87f * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);
	solar_angel_radians = PI / 12.0f * (current_hour - 12);
	lamda = _degree_to_radians(_longitude);
	beta = asin(sin(sigma) * sin(lamda) + cos(sigma) * cos(lamda) * cos(delta));
	alpha = acos((sin(lamda) * sin(beta) - sin(sigma)) / (cos(lamda) * cos(beta)));
}

void Sun::second_step() {
	t_ss = 12 + 12 / PI * acos(-(sin(sigma) * sin(lamda)) / (cos(sigma) * cos(lamda)));
	t_sr = 24 - t_ss;
	DL = 2 * (t_ss - 12);
}

void Sun::third_step() {
	Ic = 1370.0f;
	epsilon_0 = 1 + 0.033cos(2 * PI * (t_d - 10) / (365 + is_leapYear));
	Ic_prime = Ic * epsilon_0;
	a = sin(lamda) * sin(sigma);
	b = cos(lamda) * cos(sigma);
	I_et_d = 3600 * Ic_prime * 24 / PI * (a * acos(-a / b) + b * sqrt(1 - (a / b) * (a / b)));
	{b_0 = 0.18, b_1 = 0.55; }//Cold or temperate
	{b_0 = 0.25, b_1 = 0.45; }//Dry tropical
	{b_0 = 0.29, b_1 = 0.42; }//Humid tropical
	{s = DL; } // have all sunshine in the day time
	I_t_d = I_et_d * (b_0 + b_1 * s / DL);
}

void Sun::fourth_step() {
	if (I_t_d / I_et_d < 0.07)
		I_df_d = I_t_d;
	else if (I_t_d / I_et_d < 0.35)
		I_df_d = I_t_d * (1 - 2.3 * (I_t_d / I_et_d - 0.07) * (I_t_d / I_et_d - 0.07));
	else if (I_t_d / I_et_d < 0.75)
		I_df_d = I_t_d * (1.33 - 1.46 * I_t_d / I_et_d);
	else
		I_df_d = I_t_t * 0.23;
	I_dr_d = I_t_d - I_df_d;
}

void Sun::fifth_step() {
	psi = PI * I_t_d / 86400 / (a * acos(-a / b) + b * sqrt(1 - (a / b) * (a / b)));
	A = -b * psi;
	B = a * psi; 
	I_t = 86400 / PI * (B * acos(-a / b) - A * sqrt(1 - (a / b) * (a / b)));
	I_et = Ic_prime * sin(beta);
}

void Sun::sixth_step() {
	R = (0.847 - 1.61 * sin(beta) + 1.04 * sin(beta) * sin(beta));
	K = (1.47 - R) / 1.66;
	if (I_t / I_et <= 0.22)
		I_df = I_t;
	else if (I_t / I_et <= 0.35)
		I_df = I_t * (1 - 6.4 * (I_t / I_et - 0.22) * (I_t / I_et - 0.22));
	else if (I_t / I_et <= K)
		I_df = I_t * (1.47 - 1.66 * I_t / I_et);
	else
		I_df = I_t * R;
	I_dr = I_t - I_dr;
}