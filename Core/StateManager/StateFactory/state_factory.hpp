#ifndef STATE_FACTORY_HPP
#define STATE_FACTORY_HPP

#include <memory>
#include "StateInterface/state_interface.hpp"
#include "StateInterface/state_id.hpp"


// 状態オブジェクトを生成するファクトリークラス
class StateFactory {

    public:

        // StateID に対応する State オブジェクトを生成して返す
        static std::unique_ptr<StateInterface> createState(StateID state_id) {

            switch (state_id) {

                // InitStateBase
                case StateID::INIT_STATE:
                    // return std::make_unique<InitState>();
                    break;

                // PreFlightStateBase
                case StateID::CALIBRATION_STATE:
                    // return std::make_unique<CalibrationState>();
                    break;

                case StateID::PRE_FLIGHT_STATE:
                    // return std::make_unique<PreFlightState>();
                    break;

                // ManualFlightStateBase
                case StateID::MANUAL_FLIGHT_STATE:
                    // return std::make_unique<ManualFlightState>();
                    break;

                case StateID::LEVEL_TURN_STATE:
                    // return std::make_unique<LevelTurnState>();
                    break;

                case StateID::CLIMBING_TURN_STATE:
                    // return std::make_unique<ClimbingTurnState>();
                    break;

                case StateID::FUGUE_EIGHT_STATE:
                    // return std::make_unique<FugueEightState>();
                    break;

                // AutoFlightStateBase
                case StateID::PRE_AUTO_FLIGHT_STATE:
                    // return std::make_unique<PreAutoFlightState>();
                    break;

                case StateID::AUTO_TAKEOFF_STATE:
                    // return std::make_unique<AutoTakeoffState>();
                    break;

                case StateID::AUTO_FLIGHT_STATE:
                    // return std::make_unique<AutoFlightState>();
                    break;

                case StateID::AUTO_LANDING_STATE:
                    // return std::make_unique<AutoLandingState>();
                    break;

                // PostFlightStateBase
                case StateID::POST_FLIGHT_STATE:
                    // return std::make_unique<PostFlightState>();
                    break;

                case StateID::EMERGENCY_STATE:
                    // return std::make_unique<EmergencyState>();
                    break;

                default:
                    // 無効な状態ID
                    return nullptr;
            }

            return nullptr;
        }
};

#endif // STATE_FACTORY_HPP
