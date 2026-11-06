#pragma once

#include <initializer_list>

namespace robot_ui {

using AutonFunction = void (*)();

struct AutonOption {
    const char* name;
    AutonFunction function;
};

void initialize(
    std::initializer_list<AutonOption> autonomousRoutines
);

void runSelectedAutonomous();

const char* getSelectedAutonomousName();

bool isAutonTestRunning();
void stopRobotAfterAutonTest();

}  // namespace robot_ui