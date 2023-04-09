#include <unity.h>
#include "../main/bmp_altimeter.h"

void setUp(void)
{
    // set stuff up here
    GNDLEVELPRESSURE_HPA = 0;
}

void tearDown(void)
{
    // clean stuff up here
}

/// @brief test pressure calibration
/// @param void
void test_set_ground_level_pressure(void)
{
    setGroundLevelPressure();
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 1, GNDLEVELPRESSURE_HPA);
}

/// @brief ensure this function runs without errors
/// @param void
void test_setup_bmp(void)
{
    setupBMP();
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 1, GNDLEVELPRESSURE_HPA);
}

/// @brief verify info from getBMP
/// @param void
void test_get_bmp(void)
{
    bmpReading sample = getBMP();
    TEST_ASSERT_DOUBLE_WITHIN(0.0000001, 1, sample.altitude);
    TEST_ASSERT_TRUE(sample.time > 0);
}

int main()
{
    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_set_ground_level_pressure);
    RUN_TEST(test_setup_bmp);
    RUN_TEST(test_get_bmp);

    UNITY_END(); // stop unit testing
}