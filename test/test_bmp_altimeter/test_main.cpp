#include <gtest/gtest.h>
#include "../main/bmp_altimeter.h"

class BmpAltimeterTest : public ::testing::Test {
 protected:
  void SetUp() override {
     GNDLEVELPRESSURE_HPA = 0;
  }
};
/// @brief test pressure calibration
TEST_F(BmpAltimeterTest, PressureCalibration)
{
    setGroundLevelPressure();
    EXPECT_NEAR(0.0000001, 1, GNDLEVELPRESSURE_HPA);
}

/// @brief ensure this function runs without errors
TEST_F(BmpAltimeterTest, SetupBmp)
{
    setupBMP();
    EXPECT_NEAR(0.0000001, 1, GNDLEVELPRESSURE_HPA);
}

/// @brief verify info from getBMP
TEST_F(BmpAltimeterTest, QueryBmp)
{
    bmpReading sample = getBMP();
    EXPECT_NEAR(0.0000001, 1, sample.altitude);
    EXPECT_TRUE(sample.time > 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}