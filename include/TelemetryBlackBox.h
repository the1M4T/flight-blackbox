#pragma once
#include <cstdint>
#include <cstddef>

enum RocketState : uint8_t {
    DISARM    = 0,
    ARM       = 1,
    FLIGHT    = 2,
    PARACHUTE = 3,
    LANDED    = 4
};

class TelemetryBlackBox {
public:
    TelemetryBlackBox() = default;
    void onRocketStateChange(RocketState state);
    void onParachuteDeployed();
    void onBaroReading(bool error, uint16_t value);
    void recordCurrentState();
    uint32_t getLastDataframe();

private:
    static constexpr size_t BUFFER_SIZE = 16;
    RocketState current_state_ = RocketState::DISARM;
    bool parachute_deployed_ = false;
    bool baro_error_ = false;
    uint16_t altitude_ = 0;
    uint32_t buffer_[BUFFER_SIZE] = {0};
    size_t head_ = 0;
    size_t count_ = 0;
};