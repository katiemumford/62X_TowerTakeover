#include "./vex.h"
#include "./config.h"
#include "./vex_global.h"
#include "C:/Program Files (x86)/VEX Robotics/VEXcode/sdk/vexv5/include/vex_global.h"
#include "C:/Program Files (x86)/VEX Robotics/VEXcode/sdk/vexv5/include/vex_units.h"
#include <cmath>
#include <ratio>
#include <vector>
using namespace vex;

//ALL BASIC AND DRIVER FUNCTIONS FOUND HERE

//katie code, adapted dean's intake toggle code for the arm
int minPct = 5; //minimum controller value (%) for drive, accounts for stick drift
bool armMoving = false;  //whether the arm has been toggled
int armWait = 0; //time since last arm toggle

bool tiltingForward = false; //whether the tilt has been toggled to go forward
int tiltWait = 0; // time since last titl forward toggle

int maxPotVal = 1288; //sensor values for when tray is down and up
int restPotVal = 2780;

// Limit switch for hitting the tray
bool trayLimitHit = false;
//Is the intake Spinning
bool intaking = false;
//Tower heights
double bottomHeight = 0; 
double lowTowerHeight = 0.25; 
double midTowerHeight = 0.5;  

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

/**
Sept 23, 2019
Katie wants to change the program 
**/ 
/////////////////INTAKE////////////////////////////////////////////////////////////////////////////////////////////
void intakeControl() {

//if R1 is pressed, intake intakes at 100 until unpressed
//if R2 pressed, intake outakes at -100 until unpressed
//if R1 and (shiftKey) L2 pressed, intake intakes at 50 until unpressed
//if R2 and (shiftKey) L2 pressed, intake outakes at -50 until unpressed

if (Controller.ButtonL2.pressing() && Controller.ButtonL1.pressing()){ //DO THESE WORK??? LOL WUT
  spinIntake(40);
} else if (Controller.ButtonL2.pressing()&& Controller.ButtonL2.pressing()){
  spinIntake(-40);
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

void armLowTower(){ //move arm from current position to low tower
  arm.startRotateTo(lowTowerHeight, vex::rotationUnits::rev, 100, vex::velocityUnits::pct);
}

void armMidTower(){
  arm.startRotateTo(midTowerHeight, vex::rotationUnits::rev, 100, vex::velocityUnits::pct);
}
void armBottom(){
  arm.startRotateTo(bottomHeight, vex::rotationUnits::rev, 100, vex::velocityUnits::pct);

}

void armControl() {   //big function for controlling arms
if (intaking&& arm.value()){

  
}
if (Controller.ButtonX.pressing() || Controller.ButtonY.pressing() || Controller.ButtonB.pressing()){
  Controller.ButtonX.pressed(armLowTower); //when X is pressed once, move arm to mid twr
  Controller.ButtonY.pressed(armMidTower); //when Y is pressed once, move arm to low tower
  Controller.ButtonB.pressed(armBottom); //when B is pressed once, return tray to lower
}
else{
  arm.stop(vex::brakeType::brake);
}
}

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

