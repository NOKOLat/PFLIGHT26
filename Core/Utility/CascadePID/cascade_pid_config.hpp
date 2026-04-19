#pragma once

namespace CascadePidConfig {
    namespace Pitch {
        namespace Angle {
            constexpr float KP = 2.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
        namespace Rate {
            constexpr float KP = 0.5f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
    }

    namespace Roll {
        namespace Angle {
            constexpr float KP = 1.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
        namespace Rate {
            constexpr float KP = 2.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
    }

    namespace Yaw {
        namespace Angle {
            constexpr float KP = 1.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
        namespace Rate {
            constexpr float KP = 1.0f;
            constexpr float KI = 0.0f;
            constexpr float KD = 0.0f;
        }
    }
}
