#include "./vex.h"
#include "./config.h"
#include "./vex_global.h"
#include <cmath>
#include <ratio>
#include <vector>
using namespace vex;

//ALL BASIC AND DRIVER FUNCTIONS FOUND HERE

//katie code, adapted dean's intake toggle code for the arm
int minPct = 5; //minimum controller value (%) for drive, accounts for stick drift

bool tiltingForward = false; //whether the tilt has been toggled to go forward
int tiltWait = 0; // time since last titl forward toggle

int maxPotVal = 1288; //sensor values for when tray is down and up
int restPotVal = 2780;

// Limit switch for hitting the tray
bool trayLimitHit = false;

bool intaking = false;  //for armControl to know whether to apply downword force or just brake

//values and bools for armControl
bool armMoving = false; 
double trayForwardArmUp = 1.5;      //amount of rev tray moves forward for arms
double lowTowerHeight = 1.9;       //amount of rev arm moves up for low tower
double midTowerHeight = 2.5; 
double bottomHeight = 0;      //amount of rev arm moves up for mid tower

//tray moves back the same amount
//arms move back down to zero revs

//////////BASIC_FUNCTIONS//////////
#pragma region...

void drive(double l,  double r) { //percent drive
  if (l < minPct && l > -minPct) { l = 0; }
  if (r < minPct && r > -minPct) { r = 0; } //accounts for stick drift

  lF.spin(vex::directionType::fwd, l, vex::percentUnits::pct);
  lB.spin(vex::directionType::fwd, l, vex::percentUnits::pct);
  rF.spin(vex::directionType::fwd, r, vex::percentUnits::pct);
  rB.spin(vex::directionType::fwd, r, vex::percentUnits::pct);
}

void vdrive(double l, double r) { //voltage drive
  if (l < minPct && l > -minPct) { l = 0; }
  if (r < minPct && r > -minPct) { r = 0; } //accounts for stick drift
  l *= 12.0/100;
  r *= 12.0/100; //converts to volts

  lF.spin(vex::directionType::fwd, l, vex::voltageUnits::volt);
  lB.spin(vex::directionType::fwd, l, vex::voltageUnits::volt);
  rF.spin(vex::directionType::fwd, r, vex::voltageUnits::volt);
  rB.spin(vex::directionType::fwd, r, vex::voltageUnits::volt);
}

void spinIntake(int pct) {
  if (pct != 0) {
    rin.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    lin.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
  } else {
    rin.stop(vex::brakeType::brake);
    lin.stop(vex::brakeType::brake);
  }
}

void moveArm(int pct) {
  if (pct != 0) {
    arm.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    Controller.Screen.print("bruh");
  } else {
    arm.stop(vex::brakeType::brake);
  }
}


void moveTray(int pct) {
  if (pct!= 0) {

    // If the limit switch is pressed and we are trying to drive tray into the robot frame,
    //  then we don't want to actually allow the tray to move
    int trayLimitHit = trayLimit.value();
    if (!trayLimitHit || pct < 0) {
      tray.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    } else {
      tray.stop(vex::brakeType::brake);
    }
  } else {
    tray.stop(vex::brakeType::brake);
  } 
}
#pragma endregion

//////////DRIVER_FUNCTIONS//////////
#pragma region

/////////////////INTAKE////////////////////////////////////////////////////////////////////////////////////////////
void intakeControl() {

//if R1 is pressed, intake intakes at 100 until unpressed
//if R2 pressed, intake outakes at -100 until unpressed
//if R1 and (shiftKey) L2 pressed, intake intakes at 50 until unpressed
//if R2 and (shiftKey) L2 pressed, intake outakes at -50 until unpressed

if (Controller.ButtonL2.pressing() && Controller.ButtonR1.pressing()){ //DO THESE WORK??? LOL WUT
  spinIntake(40);
  intaking = true;
} else if (Controller.ButtonL2.pressing()&& Controller.ButtonL2.pressing()){
  spinIntake(-40);
  intaking = true;
}
else if (Controller.ButtonR1.pressing()){
  spinIntake(100);
  intaking = true;
}
else if(Controller.ButtonR2.pressing()){
  spinIntake(-100);
  intaking = true;
}
else{
  spinIntake(0);
  intaking = false;
}
}

/////////////////ARM CONTROL///////////////////////////////////////////////////////////////////////////////////////


void armControl() {   //big function for controlling arms

 if (Controller.ButtonX.pressing()){   //if X is continously pressed
  moveArm(80);
  if (tray.rotation(rev) > -1){
      moveTray(-80);
    } else {
      moveTray(0);
    }
 } else if (Controller.ButtonB.pressing()){
   moveArm(-50);
   Controller.Screen.print("i should be going down");
 } else {
   moveArm(0);
 }
}

/** works
if (Controller.ButtonX.pressing()){
   Controller.Screen.print("i am pressed");
  armMoving =true;
  moveArm(100);
} else if (Controller.ButtonB.pressing()){
  moveArm(-100);
  armMoving = true;
} else {
  moveArm(0);

  armMoving = false;
}
**/
 

//when x is continously pressed, the tray should move forward a little bit 


/**
if (!armMoving){
  arm.stop(hold);
}

void armUpLowTower(){ //move arm from current position to low tower
  Controller.Screen.print("x was pressed i should move up");
  armMoving = true;
  tray.rotateTo(-1, vex::rotationUnits::rev, 50, vex::velocityUnits::pct,false);
  arm.rotateTo(lowTowerHeight, vex::rotationUnits::rev, 50, vex::velocityUnits::pct,true);
  armMoving = false;

}

void armUpMidTower(){
  //move arm from current position to mid tower
  armMoving = true;
  tray.rotateTo(-trayForwardArmUp, vex::rotationUnits::rev, 100, vex::velocityUnits::pct,true);
  arm.rotateTo(midTowerHeight, vex::rotationUnits::rev, 100, vex::velocityUnits::pct,true);
  armMoving = false;

}

void armDownIntakePos(){
  //move arm from current position to intake position
  
  armMoving = true;
  tray.rotateTo(trayForwardArmUp, vex::rotationUnits::rev, 100, vex::velocityUnits::pct,true);
  arm.rotateTo(bottomHeight, vex::rotationUnits::rev, 100, vex::velocityUnits::pct,true);
  armMoving = false;

}
void armControl() {   //big function for controlling arms

if (arm.value()<50 && intaking && !armMoving){
     arm.spin(vex::directionType::rev, 2, vex::voltageUnits::volt);
   }

Controller.ButtonX.pressed(armUpLowTower); //when X is pressed once, move arm to mid twr
Controller.ButtonY.pressed(armUpMidTower); //when Y is pressed once, move arm to low tower
Controller.ButtonB.pressed(armDownIntakePos); //when B is pressed once, return tray to lower

if (intaking && !armMoving && arm.value()<50){                      //apply downword volume when intaking
 arm.spin(vex::directionType::rev, 2, vex::voltageUnits::volt);
} else {
  arm.stop(vex::brakeType::brake);                                //brake any other time
}


Controller.ButtonX.pressed(armUpLowTower); //when X is pressed once, move arm to mid twr
Controller.ButtonY.pressed(armUpMidTower); //when Y is pressed once, move arm to low tower
Controller.ButtonB.pressed(armDownIntakePos); //when B is pressed once, return tray to lower

   if (arm.value()<50 && intaking && !armMoving){
     arm.spin(vex::directionType::rev, 2, vex::voltageUnits::volt);
   }
 }
**/





/////////////////TRAY CONTROL///////////////////////////////////////////////////////////////////////////////////////
bool trayMovingBackAutomat = false;

void moveBackAutomatically(){
  trayMovingBackAutomat = true;
  if (trayLimit.value() == 0){    //if limit switch is not being hit, tray is up in air
    while(trayLimit.value() != 1){  //so...until limit switch is hit, move tray
      tray.spin(fwd, 100, pct);
    }
  }
  trayMovingBackAutomat = false;
  //////////////alternate version
  /**

  if (trayLimit.value() == 0) {         //if limit switch not hit, move tray
    tray.spin(fwd, 100, pct);
  } else if (trayLimit.value() == 1) {    //if limit switch hit, stop moving and set bool to false
    moveTray(0);                          //to signifty that this function is done running
    trayMovingBackAutomat = false;
  }

  **/
}

void trayControl() {
  //if limit switch hit, reset encoder to zero
  if (trayLimit.value() == 1){
    tray.resetRotation();
  }
  Controller.ButtonL2.pressed(moveBackAutomatically);

  if (Controller.ButtonL2.pressing()){    //if L2 continously pressed, move tray towards limit switch
    moveTray(90);                           
  } else if (Controller.ButtonL1.pressing()){    //if L1 continously pressed, move tray away from limit switch
    if (tray.rotation(rev) > -5){
      moveTray(-80);
    } else {
      moveTray(-30);
    }
  } else if (trayMovingBackAutomat == false){
    moveTray(0);
  }
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////AUTON_FUNCTIONS//////////
#pragma region

//ONE SQUARE = 1.79 rev
//RIGHT TURN ~ 1.0rev

void wait(int millis) {
  vex::task::sleep(millis);
}

//drive for a given distance, uses built-in encoder function
//program will wait for the drive to finish if wait == true
void basicEncoderDrive(double pct, double rev, bool wait) {
  lF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  lB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  rF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  if (wait) {
    rB.rotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  } else {
    rB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  }
}

//turn in place for a given distance per wheel, uses built-in encoder function
//program will wait for the turn to finish if wait == true
void basicEncoderTurn(double pct, double rev, bool wait) {
  lF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  lB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  rF.startRotateFor(-rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  if (wait) {
    rB.rotateFor(-rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  } else {
    rB.startRotateFor(-rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  }
}

//drive for a given time (in milliseconds)
void timedDrive(double pct, int millis) {
  drive(pct, pct);
  wait(millis);
  drive(0,0);
}
#pragma endregion

