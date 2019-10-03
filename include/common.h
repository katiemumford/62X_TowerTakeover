#include "./vex.h"
#include "./config.h"
#include "./vex_global.h"
#include <cmath>
#include <ratio>
#include <vector>
using namespace vex;

//ALL BASIC AND DRIVER FUNCTIONS FOUND HERE

/**dean code
int minPct = 5; //minimum controller value (%) for drive, accounts for stick drift
bool intaking = false;  //whether the intake has been toggled
int intakeWait = 0; //time since last toggle
**/

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

//while program running, look at cortex, and see all things that are plugged in, look 
//at ports in question for potentiometer value...want the revolutions value (encoder value)
//normally and then what you want


//////////BASIC_FUNCTIONS//////////
#pragma region

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

void intakeControl() {

//if R1 is pressed, intake intakes at 100 until unpressed
//if R2 pressed, intake outakes at -100 until unpressed
//if R1 and (shiftKey) L2 pressed, intake intakes at 50 until unpressed
//if R2 and (shiftKey) L2 pressed, intake outakes at -50 until unpressed

if (Controller.ButtonL2.pressing() && Controller.ButtonL1.pressing()){
  spinIntake(40);
} else if (Controller.ButtonL2.pressing()&& Controller.ButtonL2.pressing()){
  spinIntake(-40);
}
else if (Controller.ButtonR1.pressing()){
  spinIntake(100);
}
else if(Controller.ButtonR2.pressing()){
  spinIntake(-100);
}
else{
  spinIntake(0);
}
}
  
  /** dean code
  //if R1 is pressed 200ms since last press, toggle intake on/off
  if (Controller.ButtonR1.pressing() && vex::timer::system() > intakeWait + 200) {
    intaking = !intaking;
    intakeWait = vex::timer::system();
  } 
  if (Controller.ButtonR2.pressing()) { //hold R2 to rev intake
    spinIntake(-100);
    intaking = false;
  } else if (intaking || Controller.ButtonB.pressing()) { //hold B to intake (optional)
    spinIntake(100);
  } else {
    spinIntake(0);
  }
  **/


/**
default you press R1 once and tray goes to stacked position (tuned)
press the same button again and returns to intake position

hold shift key and press R1 and R2(shift key) to get intake arm up to tower height
hold shift key and press R1 again to get intake back down 
**/

void armUpLowTower(){ //move arm from current position to low tower
Controller.Screen.print("ishould raise arms");
  tray.rotateTo(-1, vex::rotationUnits::rev, 10, vex::velocityUnits::pct, true); //tray move a bit

  //this is where the arm figures out how much to rotate up 
  double lowTowerHeight = XXX; //ending revolution measurement (final position, down all the way, intaking position)
  double currentEncoderPosition = arm.rotation(rev); //current revolutions of arm motor encoder
  double travelTowards = lowTowerHeight - currentEncoderPosition; 
  arm.rotateTo(-travelTowards, vex::rotationUnits::rev, 10, vex::velocityUnits::pct,true);
}

void armUpMidTower(){
  //move arm from current position to mid tower
  double midTowerHeight = XXX; //ending revolution measurement (final position, down all the way, intaking position)
  double currentEncoderPosition = arm.rotation(rev); //current revolutions of arm motor encoder
  double travelTowards = midTowerHeight - currentEncoderPosition;
  arm.rotateTo(-travelTowards, vex::rotationUnits::rev, 10, vex::velocityUnits::pct,true);
}

void armDownIntakePos(){
  //move arm from current position to intake position
  double lowTowerHeight = 0; //ending revolution measurement (final position, down all the way, intaking position)
  double currentEncoderPosition = arm.rotation(rev); //current revolutions of arm motor encoder
  double travelTowards = lowTowerHeight - currentEncoderPosition;
  arm.rotateTo(-travelTowards, vex::rotationUnits::rev, 10, vex::velocityUnits::pct,true);

}

void armControl() {   //big function for controlling arms
Controller.ButtonX.pressed(armUpLowTower); //when X is pressed once, move arm to mid twr
Controller.ButtonY.pressed(armUpMidTower); //when Y is pressed once, move arm to low tower
Controller.ButtonB.pressed(armDownIntakePos); //when B is pressed once, return tray to lower
  /**
//try two 
if (Controller.ButtonL1.pressing()){
  buttonL1Pressed = true;
} else if (buttonL1Pressed) {
  moveArm(100); //add encoder value? talk to dean 
  buttonL1Pressed = false;
} else if (Controller.ButtonL1.pressing() && buttonL1Pressed == false){
  moveArm(-100);
}

 //if L2 and L1 are pressed 200ms since last press, toggle arm on/off
  if (Controller.ButtonL1.pressing() && Controller.ButtonL2.pressing() && vex::timer::system() > armWait + 200) {
    armMoving = true;
    armWait = vex::timer::system();
  } 
  if (Controller.ButtonL1.pressing() && Controller.ButtonL2.pressing() && armMoving ) { //if holding both buttons for the first time
    moveArm(100);  //move arm up to tower height and power 
    armMoving = false;
  } else if (Controller.ButtonL1.pressing() && Controller.ButtonL2.pressing() && !armMoving ) {
    moveArm(-100); 
    //figure out exact amount to go back down
    //moveArm needs macro to tilt tray a little bit
  } else {
    moveArm(0);
  }
  **/ 

  /** dean code
  if (Controller.ButtonL1.pressing()) { //hold L1 to move intake arm up
    moveArm(100);
  } else if (Controller.ButtonL2.pressing()) {  //hold L2 to move intake arm down
    moveArm(-70); 
  } else {
    moveArm(0);
  } **/
}

bool trayMovingBackAutomat = false;

void moveBackAutomatically(){
  trayMovingBackAutomat = true;
  if (trayLimit.value() == 0){    //if limit switch is not being hit, tray is up in air
    while(trayLimit.value() != 1){  //so...until limit switch is hit, move tray
      tray.spin(fwd, 100, pct);
    }
  }
  trayMovingBackAutomat = false;
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
  /**

//if L1 is pressed 200ms since last press, toggle tilt on/off for forwards then backwards
  if (Controller.ButtonL1.pressing() && vex::timer::system() > tiltWait + 200) {
    tiltingForward = true;    //don't totally know what this does but dean had it?
    tiltWait = vex::timer::system();
  } 
  if (Controller.ButtonR1.pressing() && tiltingForward) { //if toggle already pressed and titling forward,
    moveTray(100); //FIGURE OUT PROPER TILt TO GO FORWARD
    tiltingForward = false; 
  } else if (Controller.ButtonL1.pressing() && !tiltingForward) { //hold B to intake (optional)
    moveTray(-100); //FIGURE OUT PROPER TILT TO GO BACK
  } else {
    moveTray(0);
  }

  //manual control buttons 
  if (Controller.ButtonX.pressing()) {  //hold X to move tray up
    moveTray(80);
  } else if (Controller.ButtonA.pressing()) {  //hold A to move tray down
    moveTray(-80);
  } else {
    moveTray(0);
  }
}
**/
#pragma endregion


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

