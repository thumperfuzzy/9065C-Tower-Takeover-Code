using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern controller Controller2;
extern motor RFD;
extern motor RBD;
extern motor LFD;
extern motor LBD;
extern motor rLift;
extern motor lLift;
extern motor claw;
extern motor claw2;
extern signature visionSensor__SIG_1;
extern signature visionSensor__SIG_2;
extern signature visionSensor__SIG_3;
extern signature visionSensor__SIG_4;
extern signature visionSensor__SIG_5;
extern signature visionSensor__SIG_6;
extern signature visionSensor__SIG_7;
extern vision visionSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );