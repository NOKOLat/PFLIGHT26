#ifndef STATE_HEADERS_HPP
#define STATE_HEADERS_HPP

// InitStateBase
#include "States/InitStateBase/init_state.hpp"

// PreFlightStateBase
#include "States/PreFlightStateBase/calibration_state.hpp"
#include "States/PreFlightStateBase/pre_flight_state.hpp"

// ManualFlightStateBase
#include "States/ManualFlightStateBase/manual_flight_state.hpp"
#include "States/ManualFlightStateBase/level_turn_state.hpp"
#include "States/ManualFlightStateBase/climbing_turn_state.hpp"
#include "States/ManualFlightStateBase/fugue_eight_state.hpp"

// AutoFlightStateBase
#include "States/AutoFlightStateBase/pre_auto_flight_state.hpp"
#include "States/AutoFlightStateBase/auto_takeoff_state.hpp"
#include "States/AutoFlightStateBase/auto_flight_state.hpp"
#include "States/AutoFlightStateBase/auto_landing_state.hpp"

// PostFlightStateBase
#include "States/PostFlightStateBase/post_flight_state.hpp"
#include "States/PostFlightStateBase/emergency_state.hpp"

#endif // STATE_HEADERS_HPP
