#include <iostream>
#include "../../simulators/sun.h"
int main() {
	Sun sun_test(2019, 6, 21, 12, 0, 0, 0.0f, 23.5f);
	sun_test.getResult();
	std::cout << sun_test._radians_to_degree(sun_test.get_solarAltitude()) << std::endl;
	std::cout << sun_test._radians_to_degree(sun_test.get_sunAzimuth()) << std::endl;
	std::cout << sun_test.get_hourlyIrradiance() << std::endl;
	sun_test.updateTime(2019, 5, 19, 12, 0, 0);
	sun_test.getResult();
	std::cout << sun_test._radians_to_degree(sun_test.get_solarAltitude()) << std::endl;
	std::cout << sun_test._radians_to_degree(sun_test.get_sunAzimuth()) << std::endl;
	std::cout << sun_test.get_hourlyIrradiance() << std::endl;
	sun_test.updatePosition(-118.2722f, 34.0279f);
	sun_test.getResult();
	std::cout << sun_test._radians_to_degree(sun_test.get_solarAltitude()) << std::endl;
	std::cout << sun_test._radians_to_degree(sun_test.get_sunAzimuth()) << std::endl;
	std::cout << sun_test.get_hourlyIrradiance() << std::endl;
	return 0;
}