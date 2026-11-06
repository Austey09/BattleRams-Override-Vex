#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/optical.hpp"
#include <cstdio>

using namespace lemlib;

pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({-14,-19}, pros::MotorGears::blue); // left motor group
pros::MotorGroup rightMotors({11, 12}, pros::MotorGears::blue); // right motor group
pros::MotorGroup intakeMotors({ -2,9}, pros::MotorGears::blue); // intake motor group
pros::MotorGroup liftMotors({3, -8}, pros::MotorGears::green); // lift motor group
pros::MotorGroup armMotors({-5,6}, pros::MotorGears::green); // arm motor group
pros::MotorGroup rollerMotors({1}, pros::MotorGears::green); // roller motor group

//color sensor
pros::Optical colorSensor(7);

//Inertial Sensor
pros::Imu imu(15);

//Pneumatics
pros::adi::Pneumatics Claw('A',false,false);
//Distance Sensors
//pros::Distance dNorth(10);
//pros::Distance dSouth(14);
//pros::Distance dEast(0);
//pros::Distance dWest(0);

// tracking wheels
// horizontal tracking wheel encoder.
pros::Rotation horizontalEnc(20);
// vertical tracking wheel encoder.
pros::Rotation verticalEnc(17);
// horizontal tracking wheel.
lemlib::TrackingWheel horizontalTrackingWheel(&horizontalEnc,lemlib::Omniwheel::NEW_2, -2.75);
// vertical tracking wheel.
lemlib::TrackingWheel verticalTrackingWheel(&verticalEnc, 2.0, 0);//do not change the 2.0, it is the distance from the center of the robot to the tracking wheel in inches

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              12.75, // 12 inch track width
                              lemlib::Omniwheel::NEW_325, // using 3.25" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3.5, // derivative gain (kD)
                                            0, // anti windup
                                            0.25, // small error range, in inches
                                            150, // small error range timeout, in milliseconds
                                            0.40, // large error range, in inches
                                            600, // large error range timeout, in milliseconds
                                            0 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2.3, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             8, // derivative gain (kD)
                                             0, // anti windup
                                             0.5, // small error range, in degrees
                                             150, // small error range timeout, in milliseconds
                                             0.75, // large error range, in degrees
                                             600, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
                                             //DONT CHANGE THESE SETTINGS UNLESS YOU KNOW WHAT YOU ARE DOING
);

// sensors for odometry
lemlib::OdomSensors sensors(&verticalTrackingWheel, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontalTrackingWheel, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

//Pneumatics
