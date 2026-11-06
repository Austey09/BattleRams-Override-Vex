#pragma once

#include <cmath>
#include <cstdint>

namespace auto_roller {

enum class Color { Red, Blue, Yellow };

// Set this for your team before uploading.
constexpr Color TEAM_COLOR = Color::Red;

// Starting values: tune with the sensor mounted over the actual toggle.
constexpr int AUTO_POWER = -45; // Same direction as the existing Y control.
constexpr int MIN_PROXIMITY = 80; // Optical proximity range: 0..255.
constexpr double MIN_SATURATION = 0.25;
constexpr std::uint32_t RESTART_DELAY_MS = 100;

inline bool isTargetHue(double hue, Color target) {
    switch (target) {
        case Color::Red: return hue <= 25.0 || hue >= 335.0;
        case Color::Blue: return hue >= 190.0 && hue <= 250.0;
        case Color::Yellow: return hue >= 35.0 && hue <= 80.0;
    }
    return false;
}

// Hardware-independent state machine, updated once per driver loop.
class Control {
public:
    void reset() {
        enabled_ = false;
        waiting_ = false;
    }

    bool enabled() const { return enabled_; }

    int update(bool downPressed, bool manualHeld, double hue,
               double saturation, int proximity, std::uint32_t now,
               Color target = TEAM_COLOR) {
        if (downPressed) {
            enabled_ = !enabled_;
            waiting_ = false;
            // A second Down press stops immediately, even if Y is held.
            if (!enabled_) return 0;
        }

        if (!enabled_) {
            waiting_ = false;
            return manualHeld ? -127 : 0;
        }

        const bool valid = std::isfinite(hue) && hue >= 0.0 && hue < 360.0 &&
                           std::isfinite(saturation) &&
                           saturation >= MIN_SATURATION && saturation <= 1.0 &&
                           proximity >= MIN_PROXIMITY && proximity <= 255;
        if (!valid || isTargetHue(hue, target)) {
            waiting_ = false;
            return 0;
        }

        // Stop immediately at our color; require sustained other-color readings
        // before restarting, to avoid chatter at a color boundary.
        if (!waiting_) {
            waiting_ = true;
            changedAt_ = now;
        }
        return static_cast<std::uint32_t>(now - changedAt_) >= RESTART_DELAY_MS
                   ? AUTO_POWER : 0;
    }

private:
    bool enabled_ = false;
    bool waiting_ = false;
    std::uint32_t changedAt_ = 0;
};

void initialize();
void reset();
void update();
// Normal arcade driving, with active position hold at neutral in automatic mode.
void drive(int throttle, int steering);

} // namespace auto_roller
