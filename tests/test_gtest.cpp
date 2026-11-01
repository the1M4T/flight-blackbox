#include "TelemetryBlackBox.h"
#include <gtest/gtest.h>

TEST(TelemetryBlackBoxTest, EmptyBuffer) {
    TelemetryBlackBox box;
    EXPECT_EQ(box.getLastDataframe(), 0xFFFFFFFF);
}

TEST(TelemetryBlackBoxTest, DisarmAndLandedStatesForceZeroAltitude) {
    TelemetryBlackBox box;
    box.onBaroReading(false, 5000);
    box.recordCurrentState();

    uint32_t frame = box.getLastDataframe();
    EXPECT_EQ((frame >> 8) & 0xFFFF, 0);

    box.onRocketStateChange(LANDED);
    box.onBaroReading(false, 1200);
    box.recordCurrentState();

    frame = box.getLastDataframe();
    EXPECT_EQ((frame >> 8) & 0xFFFF, 0);
}

TEST(TelemetryBlackBoxTest, BaroErrorForcesZeroAltitude) {
    TelemetryBlackBox box;
    box.onRocketStateChange(DISARM);
    box.onBaroReading(true, 500);
    box.recordCurrentState();

    uint32_t frame = box.getLastDataframe();
    EXPECT_EQ((frame >> 8) & 0xFFFF, 0xFFFF);
}

TEST(TelemetryBlackBoxTest, CircularBufferOverwrite) {
    TelemetryBlackBox box;
    box.onRocketStateChange(FLIGHT);

    for (uint16_t i = 1; i <= 20; ++i) {
        box.onBaroReading(false, i * 100);
        box.recordCurrentState();
    }

    uint32_t frame = box.getLastDataframe();
    EXPECT_EQ((frame >> 8) & 0xFFFF, 2000);
}