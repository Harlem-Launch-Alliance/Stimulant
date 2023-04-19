#include <unity.h>
#include "../main/gy521_imu.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

/// @brief test setupIMU
/// TODO: set up system to verify logs
/// @param void
void test_setup_imu(void)
{
    setupIMU();
}

/// @brief test getIMU
/// @param void
void test_get_imu(void)
{
    imuReading sample = getIMU();

    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.501953125, sample.accel.x);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.501953125, sample.accel.y);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.501953125, sample.accel.z);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.273923431, sample.gyro.x);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.273923431, sample.gyro.y);
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 0.273923431, sample.gyro.z);
    TEST_ASSERT_EQUAL(21, sample.time);
}

int main()
{
    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_setup_imu);
    RUN_TEST(test_get_imu);

    UNITY_END(); // stop unit testing
}