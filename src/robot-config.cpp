#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
motor RFD = motor(PORT16, ratio18_1, false);
motor RBD = motor(PORT12, ratio18_1, false);
motor LFD = motor(PORT13, ratio18_1, false);
motor LBD = motor(PORT14, ratio18_1, false);
motor rLift = motor(PORT8, ratio36_1, true);
motor lLift = motor(PORT4, ratio36_1, false);
motor claw = motor(PORT20, ratio36_1, true);
motor claw2 = motor(PORT10, ratio36_1, false);
/*vex-vision-config:begin*/
signature visionSensor__SIG_1 = signature (1, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature visionSensor__SIG_2 = signature (2, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature visionSensor__SIG_3 = signature (3, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature visionSensor__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature visionSensor__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature visionSensor__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature visionSensor__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vision visionSensor = vision (PORT7, 50, visionSensor__SIG_1, visionSensor__SIG_2, visionSensor__SIG_3, visionSensor__SIG_4, visionSensor__SIG_5, visionSensor__SIG_6, visionSensor__SIG_7);
/*vex-vision-config:end*/

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}