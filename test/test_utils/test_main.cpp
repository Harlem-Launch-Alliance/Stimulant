#include <gtest/gtest.h>
#include "../main/utils.h"

/// @brief test conversion of degrees to radians
TEST(utils, degToRad)
{
    double degrees = 60;
    double radians = toRad(degrees);
    EXPECT_NEAR(0.0000001, 1.04719755, radians);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}