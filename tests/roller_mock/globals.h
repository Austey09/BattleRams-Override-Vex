#pragma once
#include <vector>
#include <cstdint>
namespace pros {
enum class MotorBrake { coast, brake, hold };
constexpr int E_CONTROLLER_DIGITAL_DOWN=0, E_CONTROLLER_DIGITAL_Y=1;
inline std::uint32_t millis() { return 0; }
struct MotorGroup {
    std::vector<MotorBrake> modes{MotorBrake::coast, MotorBrake::brake, MotorBrake::coast};
    int stops=0;
    auto get_brake_mode_all() const { return modes; }
    void set_brake_mode(MotorBrake m, std::uint8_t i) { modes.at(i)=m; }
    void set_brake_mode_all(MotorBrake m) { for(auto& x:modes) x=m; }
    void brake() { ++stops; }
    void move(int) {}
};
struct Controller {
    bool down=false;
    bool get_digital_new_press(int) { bool v=down; down=false; return v; }
    bool get_digital(int) { return false; }
};
struct Optical {
    void set_led_pwm(int) {}
    void set_integration_time(int) {}
    double get_hue() { return 0; }
    double get_saturation() { return 0.8; }
    int get_proximity() { return 150; }
};
}
inline pros::MotorGroup leftMotors, rightMotors, rollerMotors;
inline pros::Controller controller;
inline pros::Optical colorSensor;
struct Chassis {
    int calls=0;
    void arcade(int, int) { ++calls; }
};
inline Chassis chassis;
