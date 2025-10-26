#include <gtest/gtest.h>
#include "EarthCore.hpp"
#include <cmath>

using namespace earthcore;

TEST(EarthCoreTest, DecimalYearWithinValidRange) {
    double year = getDecimalYear();
    EXPECT_GT(year, 2024.0);
    EXPECT_LT(year, 2026.0);
}

TEST(EarthCoreTest, DecimalYearFractionalPart) {
    double year = getDecimalYear();
    double frac = year - std::floor(year);
    EXPECT_GE(frac, 0.0);
    EXPECT_LT(frac, 1.0);
}

TEST(EarthCoreTest, MagneticFieldReturnsFiniteValues) {
    auto field = computeMagneticField(0.0, 0.0, 400.0);
    EXPECT_TRUE(std::isfinite(field.x_nT));
    EXPECT_TRUE(std::isfinite(field.y_nT));
    EXPECT_TRUE(std::isfinite(field.z_nT));
    EXPECT_TRUE(std::isfinite(field.f_nT));
    EXPECT_GT(field.f_nT, 10000);
    EXPECT_LT(field.f_nT, 70000);
}