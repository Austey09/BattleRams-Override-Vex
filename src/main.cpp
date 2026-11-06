#include "main.h"
#include "globals.h"
#include "auto_roller.hpp"
#include "pros/adi.hpp"
#include "robot/auton.h"
#include "pros/apix.h"
#include "display.hpp"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <cmath>
#include <cstdio>
#include <string>
#include <utility>

using namespace lemlib;

std::string team_number = "6079B";

void initialize() {
// Initialize the Brain screen.
     // Keep the robot stationary while this runs.
    chassis.calibrate();
    auto_roller::initialize();

    robot_ui::initialize({
        {"DO NOTHING", nullptr},

        // Replace these function names with your actual routines.
        {"LEFT SIDE", auto_left},
        {"RIGHT SIDE", auto_right},
        {"SKILLS", auto_skills},
        {"TEST", auto_test}
    });
}
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    auto_roller::reset();
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

ASSET(test_txt);

void autonomous() {
    auto_roller::reset();
    //robot_ui::runSelectedAutonomous();
    liftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    armMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    chassis.setPose(0,0,0); 
    chassis.moveToPoint(0,8,1000,{},false);
    armMotors.move(-55);
    pros::delay(100);
    armMotors.move(0);
    liftMotors.move(127);
    pros::delay(500);
    liftMotors.brake();
    chassis.turnToHeading(-270,1000);
    chassis.moveToPoint(-24,8,2000,{.forwards=false, .maxSpeed=80},false);
    liftMotors.move(-60);
    pros::delay(600);
    liftMotors.brake();
    pros::delay(550);
    Claw.set_value(true);
    pros::delay(500);
    chassis.moveToPoint(6,8,1000);
    liftMotors.move(-127);
    pros::delay(200);
    liftMotors.brake();
    chassis.turnToHeading(0,750);
    chassis.moveToPoint(6,-1,2000,{.forwards=false},false);
    Claw.set_value(false);
    chassis.setPose(0,0,0);
    pros::delay(500);
    chassis.moveToPoint(6,10,1000);
    liftMotors.move(127);
    pros::delay(1000);
    liftMotors.brake();
    chassis.turnToHeading(90,1000);
    chassis.moveToPoint(-28,10,2000,{.forwards=false, .maxSpeed=80},false);
    liftMotors.move(-60);
    pros::delay(600);
    liftMotors.brake();
    Claw.set_value(true);


    


    /*
    chassis.moveToPoint(0, 10, 2000);
    Claw.set_value(true);
    chassis.moveToPoint(0,-2,1500,{.forwards = false});
    pros::delay(1000);
    Claw.set_value(false);
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0, 10, 1000);
    liftMotors.move(127);
    pros::delay(900);
    liftMotors.move(0);
    liftMotors.brake();
    chassis.turnToHeading(180,1000,{.maxSpeed= 80});
    chassis.moveToPoint(47, 54, 5000,{.forwards = false,.maxSpeed=80},false);
    pros::delay(2000);
    pros::delay(1000);
    RightMech.set_value(true);
    chassis.moveToPoint(0, 10, 3000,{},true);
    liftMotors.move(-127);
    pros::delay(750);
    liftMotors.brake();
    chassis.turnToHeading(0,2000);
    RightMech.set_value(true);
    chassis.moveToPoint(3,1,2000,{.forwards=false},false);
    chassis.setPose(0,0,0);
    RightMech.set_value(false);
    chassis.moveToPoint(0, 10, 1000);
    chassis.moveToPoint(47, 54, 5000,{.forwards = false,.maxSpeed=80},true);
    liftMotors.move(127);
    pros::delay(1500);
    liftMotors.brake();
    pros::delay(2000);
    pros::delay(1000);
    liftMotors.move(-127);
    pros::delay(750);
    RightMech.set_value(true);
*/
    




}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    auto_roller::reset();
    bool autonTestPaused = false;

    chassis.setPose(0, 0,0);

    bool intakeState = false;
    bool outputState = false;
    bool liftState = false;
    bool armState = false;
    bool ClawState = false;

    while (true) {
        // get joystick positions
        if (robot_ui::isAutonTestRunning()) {
            if (!autonTestPaused) {
                auto_roller::reset();
                autonTestPaused = true;
            }
            pros::delay(20);
            continue;
        }
        autonTestPaused = false;
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // Update the Down toggle before deciding whether to drive or hold.
        auto_roller::update();
        auto_roller::drive(leftY, rightX);
        // delay to save resources
        pros::delay(10); //this is super important, never delete this line
       
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        intakeMotors.move(127);       // Hold R2: intake forward
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        intakeMotors.move(-127);      // Hold L2: intake reverse
        }
        else {
        intakeMotors.move(0);         // Release: stop powering the intake
        } 
        liftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

       
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        liftMotors.move(127);// Move up
        } 
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        liftMotors.move(-100);// Move down
         } 
        else {
        liftMotors.brake();// Hold current position
        }

        armMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

     
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
            armMotors.move(60);// Arm moves up
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            armMotors.move(-60);// Arm moves down
        } else {
            armMotors.brake();// Hold arm in place  
        }

     
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            if (ClawState == false) {
                Claw.set_value(true);
                ClawState = true;
            } else {
                Claw.set_value(false);
                ClawState = false;
            }
        }

    pros::delay(20);
    }
} 