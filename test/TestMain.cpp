#include <gtest/gtest.h>

/**
 * @brief GoogleTest entry point
 */
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}