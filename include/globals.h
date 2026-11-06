#ifndef GLOBALS_H
#define GLOBALS_H

#include "api.h"
#include "lemlib/api.hpp"
#include "pros/adi.hpp"
#include "pros/optical.hpp"

//controller
extern pros::Controller controller;

//drivetrain
extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;
extern lemlib::Drivetrain drivetrain;

extern pros::Imu imu;

extern pros::Distance dNorth;
extern pros::Distance dSouth;
extern pros::Distance dEast;
extern pros::Distance dWest;

extern pros::Rotation horizontalEnc;
extern lemlib::TrackingWheel horizontalTrackingWheel;
extern pros::Rotation verticalEnc;
extern lemlib::TrackingWheel verticalTrackingWheel;

extern lemlib::ControllerSettings linearController;
extern lemlib::ControllerSettings angularController;

extern lemlib::OdomSensors sensors;
extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;
extern lemlib::Chassis chassis;

extern pros::Optical colorSensor;

//motors
extern pros::MotorGroup intakeMotors;
extern pros::MotorGroup liftMotors;
extern pros::MotorGroup armMotors;
extern pros::MotorGroup rollerMotors;
//extern pros::Motor outtakeMotor;

extern pros::adi::Pneumatics Claw;
//pneumatics

#endif // GLOBALS_H