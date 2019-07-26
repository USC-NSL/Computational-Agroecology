#include <gtest/gtest.h>
#include <vector>

#ifdef _WIN32
#include <windows.h>  //GetModuleFileNameW
#else
#include <limits.h>
#include <unistd.h>  //readlink
#endif

#include "environment/simulators/photons/model/model.h"

using namespace simulator;
using namespace photonsimulator;

TEST(ConfigTest, ConstructorTest) {
  char filename[PATH_MAX];
#ifdef _WIN32
  wchar_t path[MAX_PATH] = {0};
  GetModuleFileNameW(NULL, path, MAX_PATH);
  strcpy(filename, path);
#else
  ssize_t count = readlink("/proc/self/exe", filename, PATH_MAX);
  std::string temp(filename, (count > 0) ? count : 0);
  strcpy(filename, temp.substr(0, temp.find_last_of("/\\")).c_str());
#endif

  strcat(filename, "/../../../environment/simulators/photon_simulator/asset/Corn1.obj");
  Model corn(filename);
  EXPECT_TRUE(corn.getTotalFaces() == 90);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
