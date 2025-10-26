#include <gtest/gtest.h>
#include "OrbitNodes.hpp"


TEST(GeoTest, ConstructorAndGetters) {
    Geo g(45.0, 90.0, 400.0);

    EXPECT_DOUBLE_EQ(g.getLat(), 45.0);
    EXPECT_DOUBLE_EQ(g.getLon(), 90.0);
    EXPECT_DOUBLE_EQ(g.getAlt(), 400.0);
}

TEST(DecayCountingTest, DefaultConstructor) {
    DecayCounting d;
    EXPECT_FALSE(d.getCrossed());
    EXPECT_EQ(d.getTimeUtc(), "");
    EXPECT_DOUBLE_EQ(d.getTimeFromStart(), 0.0);
}

TEST(DecayCountingTest, ParameterConstructor) {
    DecayCounting d("2025-10-26 12:00:00", 123.45);
    EXPECT_TRUE(d.getCrossed());
    EXPECT_EQ(d.getTimeUtc(), "2025-10-26 12:00:00");
    EXPECT_DOUBLE_EQ(d.getTimeFromStart(), 123.45);
}

TEST(DecayCountingTest, AssignmentOperator) {
    DecayCounting d1("2025-10-26 12:00:00", 111.1);
    DecayCounting d2;
    d2 = d1;

    EXPECT_TRUE(d2.getCrossed());
    EXPECT_EQ(d2.getTimeUtc(), "2025-10-26 12:00:00");
    EXPECT_DOUBLE_EQ(d2.getTimeFromStart(), 111.1);
}

TEST(DecayTest, DefaultConstructor) {
    Decay decay;
    EXPECT_DOUBLE_EQ(decay.getThreshold(), 0.0);
    EXPECT_FALSE(decay.getTleForward().getCrossed());
    EXPECT_FALSE(decay.getPhysicsDrag().getCrossed());
}

TEST(DecayTest, ParameterConstructorAndAssignment) {
    DecayCounting tle("2025-10-26 12:00:00", 321.0);
    DecayCounting drag("2025-10-26 13:00:00", 654.0);
    Decay d1(120.0, tle, drag);

    EXPECT_DOUBLE_EQ(d1.getThreshold(), 120.0);
    EXPECT_TRUE(d1.getTleForward().getCrossed());
    EXPECT_EQ(d1.getTleForward().getTimeUtc(), "2025-10-26 12:00:00");
    EXPECT_EQ(d1.getPhysicsDrag().getTimeUtc(), "2025-10-26 13:00:00");

    Decay d2;
    d2 = d1;
    EXPECT_DOUBLE_EQ(d2.getThreshold(), 120.0);
    EXPECT_EQ(d2.getTleForward().getTimeUtc(), "2025-10-26 12:00:00");
}

TEST(OrbitNodesHelpersTest, FromUnixMsConvertsCorrectly) {
    // 2020-01-01T00:00:00Z = 1577836800000 ms
    uint64_t ts = 1577836800000;
    auto dt = fromUnixMs(ts);

    EXPECT_EQ(dt.Year(), 2020);
    EXPECT_EQ(dt.Month(), 1);
    EXPECT_EQ(dt.Day(), 1);
    EXPECT_EQ(dt.Hour(), 0);
    EXPECT_EQ(dt.Minute(), 0);
}

TEST(OrbitNodesTest, BasicInitialization) {
    std::string line1 = "1 99999U 24001A   24280.50000000  .00001234  00000-0  12345-3 0  9991";
    std::string line2 = "2 99999 097.6000 123.4567 0005000  10.0000 350.0000 15.20000000123456";

    OrbitNodes orbit(line1, line2, 1696022400000ULL, 60.0, 10.0, 100.0);
    auto nodes = orbit.getNodes();

    EXPECT_GT(nodes.size(), 0);
    EXPECT_FALSE(orbit.getTleEpoch().empty());
    EXPECT_FALSE(orbit.getDecay().getTleForward().getCrossed());
}
