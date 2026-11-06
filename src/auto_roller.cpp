#include "auto_roller.hpp"
#include "globals.h"
#include <vector>

namespace auto_roller {
namespace {
Control control;
std::vector<pros::MotorBrake> savedLeftModes;
std::vector<pros::MotorBrake> savedRightModes;
bool driveHoldEnabled = false;
bool driveHolding = false;

void restoreModes(pros::MotorGroup& motors,
                  const std::vector<pros::MotorBrake>& modes) {
    for (std::size_t i = 0; i < modes.size(); ++i) {
        motors.set_brake_mode(modes[i], static_cast<std::uint8_t>(i));
    }
}

void setDriveHold(bool enabled) {
    if (enabled == driveHoldEnabled) return;
    if (enabled) {
        savedLeftModes = leftMotors.get_brake_mode_all();
        savedRightModes = rightMotors.get_brake_mode_all();
        leftMotors.set_brake_mode_all(pros::MotorBrake::hold);
        rightMotors.set_brake_mode_all(pros::MotorBrake::hold);
    } else {
        restoreModes(leftMotors, savedLeftModes);
        restoreModes(rightMotors, savedRightModes);
        // Release the automatic hold using the restored brake settings.
        leftMotors.brake();
        rightMotors.brake();
    }
    driveHoldEnabled = enabled;
    driveHolding = false;
}
}

void initialize() {
    colorSensor.set_led_pwm(50);
    colorSensor.set_integration_time(10);
    rollerMotors.set_brake_mode_all(pros::MotorBrake::brake);
    reset();
}

void reset() {
    control.reset();
    setDriveHold(false);
    rollerMotors.brake();
}

void update() {
    const int power = control.update(
        controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN),
        controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y),
        colorSensor.get_hue(),
        colorSensor.get_saturation(),
        colorSensor.get_proximity(),
        pros::millis()
    );
    setDriveHold(control.enabled());
    if (power == 0) {
        rollerMotors.brake();
    } else {
        rollerMotors.move(power);
    }
}

void drive(int throttle, int steering) {
    // Match the configured joystick deadband of 3.
    if (control.enabled() && std::abs(throttle) <= 3 && std::abs(steering) <= 3) {
        // Issue brake once when stopping; preserve its position target while
        // holding instead of sending arcade(0, 0) each driver loop.
        if (!driveHolding) {
            leftMotors.brake();
            rightMotors.brake();
            driveHolding = true;
        }
        return;
    }
    driveHolding = false;
    chassis.arcade(throttle, steering);
}

} // namespace auto_roller
