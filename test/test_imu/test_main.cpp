#include <gtest/gtest.h>
#include "../main/sensors/imu/imu.h"

/// @brief test setupIMU
TEST(IMUTest, SetupTest)
{
    imu imu;

    imu.setup();
}

/// @brief test getIMU
TEST(IMUTest, SampleIMUTest)
{
    imu imu;

    imuReading sample = imu.sample();

    EXPECT_NEAR(0.0000001, 1, sample.accel.x);
    EXPECT_NEAR(0.0000001, 2, sample.accel.y);
    EXPECT_NEAR(0.0000001, 3, sample.accel.z);
    EXPECT_NEAR(0.0000001, 4, sample.gyro.x);
    EXPECT_NEAR(0.0000001, 5, sample.gyro.y);
    EXPECT_NEAR(0.0000001, 6, sample.gyro.z);
    EXPECT_EQ(5, sample.time);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}