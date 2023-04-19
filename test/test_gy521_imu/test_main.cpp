#include <gtest/gtest.h>
#include "../main/gy521_imu.h"

/// @brief test setupIMU
/// TODO: set up system to verify logs
TEST(IMUTest, SetupTest)
{
    setupIMU();
}

/// @brief test getIMU
TEST(IMUTest, SampleIMUTest)
{
    imuReading sample = getIMU();

    EXPECT_NEAR(0.0000001, 0.501953125, sample.accel.x);
    EXPECT_NEAR(0.0000001, 0.501953125, sample.accel.y);
    EXPECT_NEAR(0.0000001, 0.501953125, sample.accel.z);
    EXPECT_NEAR(0.0000001, 0.273923431, sample.gyro.x);
    EXPECT_NEAR(0.0000001, 0.273923431, sample.gyro.y);
    EXPECT_NEAR(0.0000001, 0.273923431, sample.gyro.z);
    EXPECT_EQ(21, sample.time);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}