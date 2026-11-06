#include "globals.h"
#include "auto_roller.hpp"
#include <cassert>
#include <iostream>
int main() {
    const auto original=leftMotors.modes;
    auto_roller::initialize();
    auto_roller::drive(0,0);
    assert(chassis.calls==1 && leftMotors.stops==0);
    controller.down=true;
    auto_roller::update();
    for(auto m:leftMotors.modes) assert(m==pros::MotorBrake::hold);
    for(auto m:rightMotors.modes) assert(m==pros::MotorBrake::hold);
    auto_roller::drive(0,0);
    assert(leftMotors.stops==1 && rightMotors.stops==1 && chassis.calls==1);
    auto_roller::drive(3,-3);
    assert(leftMotors.stops==1 && chassis.calls==1);
    auto_roller::drive(50,0);
    assert(chassis.calls==2);
    auto_roller::drive(0,0);
    assert(leftMotors.stops==2);
    controller.down=true;
    auto_roller::update();
    assert(leftMotors.modes==original && rightMotors.modes==original);
    auto_roller::drive(0,0);
    assert(chassis.calls==3);
    controller.down=true;
    auto_roller::update();
    auto_roller::reset();
    assert(leftMotors.modes==original && rightMotors.modes==original);
    auto_roller::drive(0,0);
    assert(chassis.calls==4);
    std::cout<<"PASS: drivetrain hold, neutral deadband, manual drive, mode restoration and reset\n";
}
