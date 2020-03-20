/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Controller2          controller                    
// RFD                  motor         16              
// RBD                  motor         12              
// LFD                  motor         13              
// LBD                  motor         14              
// rLift                motor         8               
// lLift                motor         4               
// claw                 motor         20              
// claw2                motor         10              
// visionSensor         vision        7               
// ---- END VEXCODE CONFIGURED DEVICES ----

// define your global instances of motors and other devices here

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\2020cas                                          */
/*    Created:      Wed Jul 24 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cstring>
#include <iostream>
#include <string.h>

using namespace vex;

// A global instance of competition
competition Competition;

/*TODO:
add antistall code to lift
fix antistall code for claw
test auton functions
fix code for claw
make an auton selector screen
make autons
fix issues with the brain screen
find a way to not have the squares' position hardcoded
*/

/*Auton Ideas
push preload cube into goal, then push cube near goal
drive forward, put cube in tower, turn, put cube into goal
grab stack of cubes into goal
*/

// credit to Walshbots 9791 for the basics of the autonomous selector
// I used a modified version of their code to make this selector
typedef struct _button {
  int xpos;
  int ypos;
  int sidelen;
  bool buttonState;
  vex::color offColor;
  vex::color onColor;
  const char *label;
} button;
int size = 60;
int r1Y = 30;
int r2Y = 150;
button buttons[] = {
                    {120, r1Y, size, false, 0x202020, 0x008080, "1-"},
                    {210, r1Y, size, false, 0x202020, 0x008080, "2-"},
                    {300, r1Y, size, false, 0x202020, 0x008080, "3-"},
                    {390, r1Y, size, false, 0x202020, 0x008080, "4-"},
                    {120, r2Y, size, false, 0x202020, 0x008080, "5-"},
                    {210, r2Y, size, false, 0x202020, 0x008080, "6-"},
                    {300, r2Y, size, false, 0x202020, 0x008080, "7-"},
                    {390, r2Y, size, false, 0x202020, 0x008080, "8-"},
                    {30, r2Y, size, false, 0xFF1717, 0x00FF00, "Skills"},
                    {30, r1Y, size, false, 0x0000FF, 0xFF0000, "Color"}
                    };



void myWait(int TimeToWait) { vex::task::sleep(TimeToWait); }
bool testing = false;

bool within(float sensorvalue, float compvalue, float tolerance) {
  if (sensorvalue <= compvalue + tolerance ||
      sensorvalue >= compvalue - tolerance) {
    return true;
  } else {
    return false;
  }

  // return true;
}

bool timeout(vex::motor motorName, float timeoutTime){
  // the idea here was to make the motors go to holding after a certain time period to stop the claw motors from overheating
  // every time I tried to code it I got stuck in an infinite loop
  // and COVID-19 killed any future attempts at it
  return false;
}

// these functions of the screen I wrote
bool touchedInSquare(int squarex, int squarey, int len) {
  if ((Brain.Screen.xPosition() > squarex &&
       Brain.Screen.xPosition() < squarex + len) &&
      (Brain.Screen.yPosition() > squarey &&
       Brain.Screen.yPosition() < squarey + len)) {
    return true;
  } else {
    return false;
  }
}

void drawSquare(float startX, float startY, float len, vex::color sqColor,
                vex::color onColor, bool buttonState,
                std::string textOnButton) {
  if (!buttonState) {
    Brain.Screen.drawRectangle(startX, startY, len, len, sqColor);
    Brain.Screen.printAt(startX + 5, startY + (len / 2), false, "%s",
                         textOnButton.c_str());
  } else {
    Brain.Screen.drawRectangle(startX, startY, len, len, onColor);
    Brain.Screen.printAt(startX + 5, startY + (len / 2), false, "%s",
                         textOnButton.c_str());
  }
}

void initialdrawcubes() {
  for (int i = 0; i < 10; i++) {
    drawSquare(buttons[i].xpos, buttons[i].ypos, buttons[i].sidelen,
               buttons[i].offColor, buttons[i].onColor, buttons[i].buttonState,
               buttons[i].label);
  }
}

int update = 1;

void updateCubes(int i) {
  while (update == 1) {
    myWait(10);
    drawSquare(buttons[i].xpos, buttons[i].ypos, buttons[i].sidelen,
               buttons[i].offColor, buttons[i].onColor, !buttons[i].buttonState,
               buttons[i].label);
    update = 10;
  }
}

int screenfuncts() {
  initialdrawcubes();
  while (!Competition.isAutonomous() || true) {

    if (Brain.Screen.pressing()) {

      for (int i = 0; i < 10; i++) {
        if (touchedInSquare(buttons[i].xpos, buttons[i].ypos,
                            buttons[i].sidelen)) {
          for(int j = 0; j < 9; j++){
              if(buttons[j].buttonState == true){
                buttons[j].buttonState = false;
              }
          }
          buttons[i].buttonState = !buttons[i].buttonState;
          
          updateCubes(i);
        }
      }
    }
    update = 0;
    myWait(10);
  }
  return (0);
}





// auton functions
void resetenc() {
  LFD.resetRotation();
  LBD.resetRotation();
  RFD.resetRotation();
  RBD.resetRotation();
}

void autoturn(float degrees, int motorPower) {
  resetenc();
  LFD.rotateTo(degrees, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  RFD.rotateTo(degrees, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  LBD.rotateTo(degrees, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  RBD.rotateTo(degrees, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, true);
}

void autoMove(int dist, int motorPower) {
  resetenc();
  LFD.rotateTo(dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  RFD.rotateTo(-dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  LBD.rotateTo(dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  RBD.rotateTo(-dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, true);
}

void autoStrafe(int dist, int motorPower){
    resetenc();
    LFD.rotateTo(dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
    RFD.rotateTo(dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
    LBD.rotateTo(-dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
    RBD.rotateTo(-dist, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, true);

}

void autoLift(int height, int motorPower) {
  lLift.rotateTo(-height, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, false);
  rLift.rotateTo(-height, vex::rotationUnits::deg, motorPower, vex::velocityUnits::pct, true);
}

void autoClaw(std::string pos) {
  if (pos == "open" || pos == "Open") {
    claw.rotateTo(0, deg, 100, vex::velocityUnits::pct, false);
    claw2.rotateTo(0, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  } else if (pos == "closed" || pos == "Closed") {
    claw.rotateTo(110, deg, 100, vex::velocityUnits::pct, false);
    claw2.rotateTo(-110, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  }
}

void deploy(int dist, int mPower){
    autoMove(dist, mPower);
    myWait(10);
    autoMove(-dist, mPower);
}




bool halfSpeed = true;
bool secondaryControls = false;
// driver stuff
int driverControls() {
  while (!false && !Competition.isAutonomous()) {
    if (Controller1.ButtonB.pressing()) {
      testing = true;
    }
    if(Controller1.ButtonY.pressing()){
      secondaryControls = false;
    }
    if (!testing && secondaryControls) {
      // lift
      if (Controller1.ButtonR1.pressing() == true || Controller2.ButtonR1.pressing() == true) {
        rLift.spin(forward, 75, percent);
        lLift.spin(forward, 75, percent);
        myWait(10);
      } else if (Controller1.ButtonR2.pressing() == true || Controller2.ButtonR2.pressing() == true) {
        rLift.spin(reverse, 100, percent);
        lLift.spin(reverse, 100, percent);
        myWait(10);
      } else {
        lLift.stop(vex::brakeType::hold);
        rLift.stop(vex::brakeType::hold);
        rLift.rotateTo(lLift.position(deg), deg, false);
        myWait(10);
      }
      // end of lift

    } else if(!secondaryControls){
        if (Controller1.ButtonR1.pressing() == true ) {
        rLift.spin(forward, -100, percent);
        lLift.spin(forward, -100, percent);
        myWait(10);
      } else if (Controller1.ButtonR2.pressing() == true ) {
        rLift.spin(reverse, -100, percent);
        lLift.spin(reverse, -100, percent);
        myWait(10);
      } else {
        lLift.stop(vex::brakeType::hold);
        rLift.stop(vex::brakeType::hold);
        myWait(10);
      }
    }else{

    }
    myWait(10);
  }
  return (0);
}

bool closing = false;

int clawcont() {
  std::string clawpos = "closed";
  while (true && !Competition.isAutonomous()) {
    if(secondaryControls){
    if (Controller1.ButtonL2.pressing() == true || Controller1.ButtonL2.pressing()) { // open
      claw.rotateTo(-60, vex::rotationUnits::deg, -100, vex::velocityUnits::pct, false);
      claw2.rotateTo(60, vex::rotationUnits::deg, -100, vex::velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing() == true || Controller1.ButtonL2.pressing()) {
      claw.rotateTo(0, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
      claw2.rotateTo(0, vex::rotationUnits::deg, 100, vex::velocityUnits::pct);
    } else {
      closing = false;
      claw.stop(vex::brakeType::hold);
      claw2.stop(vex::brakeType::hold);
    }
    myWait(10);
    }
    else if(!secondaryControls){

   
    if(Controller1.ButtonL1.pressing() || Controller2.ButtonL1.pressing()){//open
      clawpos = "open";
    }else if(Controller1.ButtonL2.pressing() || Controller2.ButtonL2.pressing()){
      clawpos = "close";
    }else{
      clawpos = "hold";
    }

    if(clawpos == "open"){
      if(!timeout(claw, 2000)){
        claw.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
        claw2.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
      }else{
        claw.stop(vex::brakeType::hold);
        claw2.stop(vex::brakeType::hold);
      }

    }else if(clawpos == "close"){
      if(!timeout(claw, 2000)){
        claw.rotateTo(0, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
        claw2.rotateTo(0, vex::rotationUnits::deg, 100, vex::velocityUnits::pct);
      }else{
        claw.stop(vex::brakeType::hold);
        claw2.stop(vex::brakeType::hold);
      }
    }else if(clawpos == "hold"){
      claw.stop(vex::brakeType::hold);
      claw2.stop(vex::brakeType::hold);
    }else{
      claw.stop(vex::brakeType::hold);
      claw2.stop(vex::brakeType::hold);
    }

    myWait(10);
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print(timeout(claw, 2000));
    }
  }
  return (0);
}



bool visiontesting = false;
int dcontrols() {
  
  while (!Competition.isAutonomous()) {
    int Ch1 = Controller1.Axis1.position(percent);
    int Ch3 = Controller1.Axis3.position(percent);
    int Ch4 = Controller1.Axis4.position(percent);

    while (!visiontesting) {
      if (Controller1.ButtonY.pressing()) {
        visiontesting ? visiontesting = false: visiontesting = true;
        myWait(150);
      }
      if (Controller1.ButtonA.pressing()) {
        halfSpeed = !halfSpeed;
        myWait(250);
      }
      if (!halfSpeed) {
        LFD.spin(forward, Ch3 + (Ch1 * .5) + Ch4, percent);
        RFD.spin(reverse, Ch3 - (Ch1 * .5) - Ch4, percent);
        LBD.spin(forward, Ch3 + (Ch1 * .5) - Ch4, percent);
        RBD.spin(reverse, Ch3 - (Ch1 * .5) + Ch4, percent);
      } else {
        LFD.spin(forward, Ch3 / 2 + Ch1 / 2  + Ch4 / 2, percent);
        RFD.spin(reverse, Ch3 / 2 - Ch1 / 2  - Ch4 / 2, percent);
        LBD.spin(forward, Ch3 / 2 + Ch1 / 2  - Ch4 / 2, percent);
        RBD.spin(reverse, Ch3 / 2 - Ch1 / 2  + Ch4 / 2, percent);
      }
    } 

      while (visiontesting) {
        visionSensor.takeSnapshot(1);
        if (Controller1.ButtonY.pressing()) {
          visiontesting ? visiontesting = false : visiontesting = true;
          myWait(10);
        }
        if (visionSensor.largestObject.exists) {
          if (visionSensor.largestObject.centerX >= 180) {
            RFD.spin(forward, 10, percent);
            LFD.spin(forward, 10, percent);
            myWait(10);
          } else if (visionSensor.largestObject.centerX <= 120) {
            RFD.spin(forward, -10, percent);
            LFD.spin(forward, -10, percent);
            myWait(10);
          } else {
            RFD.spin(forward, -100, percent);
            LFD.spin(forward, 100, percent);
            myWait(10);
          }
        }
        wait(10, msec);
      }
    }

    myWait(10);
  
  return (0);
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////
                      Auton Functions
        deploy(distance, power) Move forward and backward a specific distance
        autoClaw("open" or "closed") either open or close the claw, MUST use quotes around open or closed
        autoLift(distance, power) move the lift to a certain position
        autoMove(distance, power) Move the robot forward or backwards a certain distance (+ is right, - is left)
        autoturn(distance, power) Turn the robot a distance
        myWait(time) wait some amount of milliseconds
        autoStrafe(distance, power) move a certain distance sideways
///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int autonToRun = 1;
void autonomous(void) {
  //code to make the screen autonomous selector work
  //it neverr worked in a match for us, so we took it out
  // for (int i = 0; i < 9; i++) {

  //   if (buttons[i].buttonState == true) {
  //     autonToRun = i + 1;
  //   }
  // }
  switch (autonToRun) {
  case 0:
    // do nothing
    break;

  case 1:
    //blue front
    autoMove(-50, 15);//start slow because for some reason it helps
    autoMove(-800, 30); //go back to get the cubes
    myWait(100);
    autoturn(365,50);//turn right to face the cubes
    myWait(100);
    autoMove(230, 15);//move to the cubes
    myWait(250);
    autoClaw("closed");//close claw
    myWait(200);
    autoLift(100, 75);//raise lift
    myWait(100);
    autoMove(-350,75);//back up
    myWait(100);
    autoturn(-500, 40);//turn to face the goal
    myWait(100);
    autoMove(800, 35);//move to the goal
    myWait(100);
    autoLift(-50, 75);//lower lift
    myWait(100);
    autoClaw("open");//open claw
    myWait(100);
    autoMove(-250, 75);//back up

    break;

  case 2:
  //red front
    autoMove(-50, 15);//start slow because for some reason it helps
    autoMove(-800, 30); //go back to get the cubes
    myWait(100);
    autoturn(-365,50);//turn left to face the cubes
    myWait(100);
    autoMove(215, 15);//move to the cubes
    myWait(250);
    autoClaw("closed");//close claw
    myWait(200);
    autoLift(100, 75);//raise lift
    myWait(100);
    autoMove(-350,75);//back up
    myWait(100);
    autoturn(449, 45);//turn to face the goal
    myWait(100);
    autoMove(750, 65);//move to the goal
    myWait(100);
    autoLift(-25, 75);//lower lift
    myWait(100);
    autoClaw("open");//open claw
    myWait(100);
    autoMove(-250, 75);//back up
    break;

  case 3:
     // blue back
      deploy(350, 100);//drive forward to open the claw
      myWait(100);//wait 100 mSec
      autoClaw("Open");//open the claw
      autoMove(500, 50);//move forward to push the cube into the line of cubes
      autoClaw("closed");//close the claw
      myWait(100);//wait 100 mSec
      autoLift(100, 75);//raise the lift
      myWait(250);//wait 1/4 sec
      autoMove(215, 50);//move forward to stack a cube on the other cube
      myWait(100);//wait 100 mSec
      autoClaw("Open");//open claw
      myWait(500);//wait .5 seconds
      autoMove(-200, 50);
      myWait(150);
      autoLift(0, 75);//move down the lift
      autoMove(200, 25);
      myWait(100);//wait 
      autoClaw("Closed");//close claw
      autoLift(10, 50);//raise the claw slightly
      myWait(100);//wait
      autoMove(-600, 75);//move back to start
      resetenc();
      autoturn(-500, 50);//turn to face the cube
      autoMove(250, 50);//move to make the cube stack over the goal
      autoClaw("open");//open the claw
      myWait(100);//wait
      autoMove(-250, 75);//back up
  case 4:
      //red back
      deploy(350, 100);//drive forward to open the claw
      myWait(100);//wait 100 mSec
      autoClaw("Open");//open the claw
      autoMove(500, 50);//move forward to push the cube into the line of cubes
      autoClaw("closed");//close the claw
      myWait(100);//wait 100 mSec
      autoLift(100, 75);//raise the lift
      myWait(250);//wait 1/4 sec
      autoMove(215, 50);//move forward to stack a cube on the other cube
      myWait(100);//wait 100 mSec
      autoClaw("Open");//open claw
      myWait(500);//wait .5 seconds
      autoMove(-200, 50);
      myWait(150);
      autoLift(0, 75);//move down the lift
      autoMove(200, 25);
      myWait(100);//wait 
      autoClaw("Closed");//close claw
      autoLift(10, 50);//raise the claw slightly
      myWait(100);//wait
      autoMove(-600, 75);//move back to start
      resetenc();
      autoturn(500, 50);//turn to face the cube
      autoMove(250, 50);//move to make the cube stack over the goal
      autoClaw("open");//open the claw
      myWait(100);//wait
      autoMove(-250, 75);//back up  
    break;

  case 5:
    
    break;

  case 6:
    //filler autonomous for testing
    autoMove(600, 100);
    break;

  case 7:
   //filler autonomous for testing
    autoMove(700, 100);
    break;

  case 8:
   //filler autonomous for testing
    autoMove(800, 100);
    break;
  case 9:
    //skills
    break;


  default:

    break;
    
  }
  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    vex::task driver(driverControls);
    vex::task drivecont(dcontrols);
    vex::task display(screenfuncts);
    vex::task clawc(clawcont);
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  
  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}