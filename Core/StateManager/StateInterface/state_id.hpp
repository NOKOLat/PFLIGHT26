#ifndef STATE_ID_HPP
#define STATE_ID_HPP

#include <cstdint>


enum class StateID : uint8_t {

    // InitStateBase
    INIT_STATE = 1,

    // PreFlightStateBase
    CALIBRATION_STATE = 2,
    PRE_FLIGHT_STATE = 3,

    // ManualFlightStateBase
    MANUAL_FLIGHT_STATE = 4,
    LEVEL_TURN_STATE = 5,
    CLIMBING_TURN_STATE = 6,
    FUGUE_EIGHT_STATE = 7,

    // AutoFlightStateBase
    PRE_AUTO_FLIGHT_STATE = 8,
    AUTO_TAKEOFF_STATE = 9,
    AUTO_FLIGHT_STATE = 10,
    AUTO_LANDING_STATE = 11,

    // PostFlightStateBase
    POST_FLIGHT_STATE = 12,
    EMERGENCY_STATE = 13,

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

#endif // STATE_ID_HPP
