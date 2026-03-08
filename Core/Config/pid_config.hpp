#ifndef PID_CONFIG_HPP
#define PID_CONFIG_HPP

namespace PidConfig {

    // ===== Pitch軸 PID ゲイン =====
    namespace Pitch {
        // 角度制御（外側ループ）
        namespace Angle {
            constexpr float KP = 0.9f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
        // 角速度制御（内側ループ）
        namespace Rate {
            constexpr float KP = 1.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
    }

    // ===== Roll軸 PID ゲイン =====
    namespace Roll {
        // 角度制御（外側ループ）
        namespace Angle {
            constexpr float KP = 3.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
        // 角速度制御（内側ループ）
        namespace Rate {
            constexpr float KP = 2.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
    }

    // ===== Yaw軸 PID ゲイン =====
    namespace Yaw {
        // 角度制御（外側ループ）
        namespace Angle {
            constexpr float KP = 1.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
        // 角速度制御（内側ループ）
        namespace Rate {
            constexpr float KP = 1.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
    }

} // namespace PidConfig

#endif // PID_CONFIG_HPP
