#include <unity.h>
#include "../main/utils.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

/// @brief test conversion of degrees to radians
/// @param void
void test_deg_to_rad(void)
{
    double degrees = 60;
    double radians = toRad(degrees);
    TEST_ASSERT_EQUAL(radians, 1.0472);
}

int main()
{
    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_deg_to_rad);

    UNITY_END(); // stop unit testing
}