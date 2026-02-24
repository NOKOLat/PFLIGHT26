#ifndef STATE_FACTORY_HPP
#define STATE_FACTORY_HPP

#include <memory>
#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"
#include "States/StateHeaders.hpp"


// 状態オブジェクトを生成するファクトリークラス
class StateFactory {

    public:

        // StateID に対応する State オブジェクトを生成して返す
        static std::unique_ptr<StateInterface> createState(StateID state_id) {

            switch (state_id) {

                // InitStateBase
                case StateID::INIT_STATE:
                    return std::make_unique<InitState>();

                // PreFlightStateBase
                case StateID::CALIBRATION_STATE:
                    return std::make_unique<CalibrationState>();

                case StateID::PRE_FLIGHT_STATE:
                    return std::make_unique<PreFlightState>();

                case StateID::SERVO_TEST_STATE:
                    return std::make_unique<ServoMotorTestState>();

                case StateID::SENSOR_TEST_STATE:
                    return std::make_unique<SensorTestState>();

                // ManualFlightStateBase
                case StateID::MANUAL_FLIGHT_STATE:
                    return std::make_unique<ManualFlightState>();

                case StateID::LEVEL_FLIGHT_STATE:
                    return std::make_unique<LevelFlightState>();

                case StateID::LEVEL_TURN_STATE:
                    return std::make_unique<LevelTurnState>();

                case StateID::CLIMBING_TURN_STATE:
                    return std::make_unique<ClimbingTurnState>();

                case StateID::FUGUE_EIGHT_STATE:
                	return std::make_unique<FugueEightState>();

                // AutoFlightStateBase
                case StateID::PRE_AUTO_FLIGHT_STATE:
                    return std::make_unique<PreAutoFlightState>();

                case StateID::AUTO_TAKEOFF_STATE:
                	return std::make_unique<AutoTakeoffState>();

                case StateID::AUTO_FLIGHT_STATE:
                    return std::make_unique<AutoFlightState>();

                case StateID::AUTO_LANDING_STATE:
                    return std::make_unique<AutoLandingState>();

                // PostFlightStateBase
                case StateID::POST_FLIGHT_STATE:
                    return std::make_unique<PostFlightState>();

                case StateID::EMERGENCY_STATE:
                    return std::make_unique<EmergencyState>();

                default:
                    // 無効な状態ID
                    return nullptr;
            }

            return nullptr;
        }
};

#endif // STATE_FACTORY_HPP
