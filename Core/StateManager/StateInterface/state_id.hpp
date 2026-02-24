#ifndef STATE_ID_HPP
#define STATE_ID_HPP

#include <cstdint>
#include <cstring>


enum class StateID : uint8_t {

    // InitStateBase
    INIT_STATE = 1,

    // PreFlightStateBase
    CALIBRATION_STATE = 2,
    PRE_FLIGHT_STATE = 3,
    SERVO_TEST_STATE = 15,
    SENSOR_TEST_STATE = 16,

    // ManualFlightStateBase
    MANUAL_FLIGHT_STATE = 4,
    LEVEL_FLIGHT_STATE = 5,
    LEVEL_TURN_STATE = 6,
    CLIMBING_TURN_STATE = 7,
    FUGUE_EIGHT_STATE = 8,

    // AutoFlightStateBase
    PRE_AUTO_FLIGHT_STATE = 9,
    AUTO_TAKEOFF_STATE = 10,
    AUTO_FLIGHT_STATE = 11,
    AUTO_LANDING_STATE = 12,

    // PostFlightStateBase
    POST_FLIGHT_STATE = 13,
    EMERGENCY_STATE = 14,

    // Invalid
    INVALID_STATE = 255
};


enum class StateBaseID : uint8_t {

    INIT_STATE_BASE = 1,
    PRE_FLIGHT_STATE_BASE = 2,
    MANUAL_FLIGHT_STATE_BASE = 3,
    AUTO_FLIGHT_STATE_BASE = 4,
    POST_FLIGHT_STATE_BASE = 5,
    INVALID_STATE_BASE = 255
};

// ヘルパー関数: StateIDを文字列に変換
inline const char* stateIdToString(StateID state_id) {
    switch (state_id) {
        case StateID::INIT_STATE:
            return "INIT_STATE";
        case StateID::CALIBRATION_STATE:
            return "CALIBRATION_STATE";
        case StateID::PRE_FLIGHT_STATE:
            return "PRE_FLIGHT_STATE";
        case StateID::MANUAL_FLIGHT_STATE:
            return "MANUAL_FLIGHT_STATE";
        case StateID::LEVEL_FLIGHT_STATE:
            return "LEVEL_FLIGHT_STATE";
        case StateID::LEVEL_TURN_STATE:
            return "LEVEL_TURN_STATE";
        case StateID::CLIMBING_TURN_STATE:
            return "CLIMBING_TURN_STATE";
        case StateID::FUGUE_EIGHT_STATE:
            return "FUGUE_EIGHT_STATE";
        case StateID::PRE_AUTO_FLIGHT_STATE:
            return "PRE_AUTO_FLIGHT_STATE";
        case StateID::AUTO_TAKEOFF_STATE:
            return "AUTO_TAKEOFF_STATE";
        case StateID::AUTO_FLIGHT_STATE:
            return "AUTO_FLIGHT_STATE";
        case StateID::AUTO_LANDING_STATE:
            return "AUTO_LANDING_STATE";
        case StateID::POST_FLIGHT_STATE:
            return "POST_FLIGHT_STATE";
        case StateID::EMERGENCY_STATE:
            return "EMERGENCY_STATE";
        case StateID::SERVO_TEST_STATE:
            return "SERVO_TEST_STATE";
        case StateID::SENSOR_TEST_STATE:
            return "SENSOR_TEST_STATE";
        default:
            return "INVALID_STATE";
    }
}

#endif // STATE_ID_HPP
