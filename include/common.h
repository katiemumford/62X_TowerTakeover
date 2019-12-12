#include "./vex.h"
#include "./config.h"
#include "./vex_global.h"
#include <cmath>
#include <ratio>
#include <vector>
#include <time.h>

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
bool current = false, prev = false;
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
bool in, out,slow;
/////////////////INTAKE////////////////////////////////////////////////////////////////////////////////////////////
void intakeControl(bool running, int32_t rValue, int32_t lValue, bool isaac) {
  //if R1 is pressed, intake intakes at 100 until unpressed
  //if R2 pressed, intake outakes at -100 until unpressed
  //if R1 and (shiftKey) L2 pressed, intake intakes at 50 until unpressed
  //if R2 and (shiftKey) L2 pressed, intake outakes at -50 until unpressed
  slow = Controller.ButtonY.pressing();
  if(isaac){
    in = Controller.ButtonR1.pressing();
    out = Controller.ButtonL1.pressing();
  } else {
    in = Controller.ButtonR1.pressing();
    out = Controller.ButtonR2.pressing();
  }
  if(!running){
    if(out && in){
      spinIntake(0);
      moveArm(0);
    }
    else if(in){
      spinIntake(100);
      moveArm(-5);
    } 
    else if(out){
      spinIntake(-100);
      moveArm(0);
    } 
    else if(slow){
      spinIntake(-15);
      moveArm(0);
    }
    else {
      spinIntake(0);
      moveArm(0);
    }
  }
}

/////////////////ARM CONTROL///////////////////////////////////////////////////////////////////////////////////////

void armControl() {   //big function for controlling arms
  /**
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
  */

  if(Controller.ButtonA.pressing()){

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
}
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
bool trayMovingUpAutomat = false;
void moveUpAutomatically(){
  trayMovingUpAutomat = true;
  while(tray.rotation(rev) > -5.95){
    if (tray.rotation(rev) > -4){
      moveTray(-60);
    } else {
      moveTray(-30);
    }
  }
  if (tray.rotation(rev) > -4){
    moveTray(-60);
  } else if (tray.rotation(rev) > -5.95){
      moveTray(-30);
  } else {
      if(Controller.ButtonUp.pressing()){
        moveTray(-30);
      } else {
        moveTray(0);
      }
  }
  trayMovingUpAutomat = false;
}

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
bool up, down, autoRunUp;
void trayControl(bool isaac) {
  //if limit switch hit, reset encoder to zero
  if (trayLimit.value() == 1){
    tray.resetRotation();
  }
  autoRunUp = isaac;
  if(isaac){
    if(Controller.ButtonUp.pressing()){
      moveTray(-30);
      up = false;
      down = false;
    } else {
      up = Controller.ButtonR2.pressing();
      down = Controller.ButtonL2.pressing();
    }
  } else {
    down = Controller.ButtonL2.pressing();
    up = Controller.ButtonL1.pressing();
  }
  
  if (!trayMovingBackAutomat && !trayMovingUpAutomat){
    if(down){
      vex::thread buttonAToggleTask(moveBackAutomatically);
    }
    else if (up){ 
        if(!isaac){
          if (tray.rotation(rev) > -4){
            moveTray(-60);
          } else if (tray.rotation(rev) > -5.95){
            moveTray(-30);
          } else {
            if(Controller.ButtonUp.pressing()){
              moveTray(-30);
            } else {
                moveTray(0);
            }
          }
        } else {
            vex::thread UpToggleTask(moveUpAutomatically);
        }   
      } else if (trayMovingBackAutomat == false ){
        moveTray(0);
      }
  }
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////AUTON_FUNCTIONS//////////


void wait(int millis) {
  vex::task::sleep(millis);
}

bool running = false, running2 = false, running3 = false;
bool inAuto;

void armControllerAuton(){
  while (inAuto){
      uint32_t t1 = timer::system();
      uint32_t t2 = timer::system();
      while(running == true){
        t2 = timer::system();
        uint32_t t3 = t2 - t1;
        if(t3 < 500){
          moveArm(100);
          moveTray(-80);
        } else if(t3 < 1000) {
          moveArm(100);
          moveTray(0);
        } else {
            uint32_t t4 = timer::system();
            uint32_t t5 = timer::system();
            while(running2){
              t5 = timer::system();
              if(t5-t4 < 500){
                spinIntake(-75);
              } else {
                 uint32_t t6 = timer::system();
                 uint32_t t7 = timer::system();
                 while(running3){
                  uint32_t t7 = timer::system();
                  if(t7-t6 < 1000) {
                    spinIntake(0);
                    moveArm(-80);
                  } else if(t7-t6 < 1500) {
                    spinIntake(0);
                    moveTray(80);
                 } else {
                   running3 = false;
                   running2 = false;
                   running = false;
                 }
              }
            }
          } 
        }
      }
      vex::task::sleep(10); 
  }
}

int deployTray() {
  moveArm(100);
  vex::this_thread::sleep_for(600);
  moveArm(-100);
  vex::this_thread::sleep_for(400);
  moveArm(0);
  return(0);
}

void outtakeSome() {
  spinIntake(-50);
  wait(300);
  spinIntake(0);
}

void setBraking() {
  lF.setStopping(vex::brakeType::brake);
  rF.setStopping(vex::brakeType::brake);
  lB.setStopping(vex::brakeType::brake);
  rB.setStopping(vex::brakeType::brake);
}

void setHolding() {
  lF.setStopping(vex::brakeType::hold);
  rF.setStopping(vex::brakeType::hold);
  lB.setStopping(vex::brakeType::hold);
  rB.setStopping(vex::brakeType::hold);
}

void setCoasting() {
  lF.setStopping(vex::brakeType::coast);
  rF.setStopping(vex::brakeType::coast);
  lB.setStopping(vex::brakeType::coast);
  rB.setStopping(vex::brakeType::coast);
}
void stopAll() {
  lF.stop();
  lB.stop();
  rF.stop();
  rB.stop();
}

void turn2(double speed) { drive(speed, -speed); }

void gyroTurn(double target) {
  target = Gyro.value(vex::analogUnits::range12bit) + target * 10 + target * 41 / 90;
  double error = target - Gyro.value(vex::analogUnits::range12bit);
  double totalError = 0;

  double kp = 0.064;
  double ki = 0.009;

  while (std::abs(error) > 2) {
    vex::task::sleep(6);
    error = target - Gyro.value(vex::analogUnits::range12bit);
    totalError += error;
    if (std::abs(error) > 69) {
      totalError = 0;
    }
    turn2(error * kp + totalError * ki);  
  }
  setHolding();
  stopAll();
}
//DegreeAmount (0 - 360) degrees robot will turn
//veloc (0 - 100) percent of motor power given
void gyroTurn2 (double DegreeAmount, int velocL, int velocR){
    //Set speeds of both Drive motors
    lF.setVelocity(velocL,velocityUnits::pct);
    rF.setVelocity(velocR,velocityUnits::pct);
    lB.setVelocity(velocL,velocityUnits::pct);
    rB.setVelocity(velocR,velocityUnits::pct);
    
    //Prints the DegreeAmount for debugging puroses to ensure that it is going for the right degree amount
    Controller.Screen.clearScreen();
    Controller.Screen.print(DegreeAmount);
    Controller.Screen.print(Gyro.value(rotationUnits::deg));

    //While loop to do the spin
    if(Gyro.value(rotationUnits::deg) < DegreeAmount){
      while (Gyro.value(rotationUnits::deg) < DegreeAmount)
      {
        /**
        if(Gyro.value(rotationUnits::deg) > DegreeAmount - 25){
          lF.setVelocity(velocL/2,velocityUnits::pct);
          rF.setVelocity(velocR/2,velocityUnits::pct);
          lB.setVelocity(velocL/2,velocityUnits::pct);
          rB.setVelocity(velocR/2,velocityUnits::pct);
        }
        */
        
        Controller.Screen.clearScreen();
        Controller.Screen.print(DegreeAmount);
        Controller.Screen.print(Gyro.value(rotationUnits::deg));

        lF.spin(directionType::fwd); // Assuming this is the polarity needed for a clockwise turn
        lB.spin(directionType::fwd);
        rF.spin(directionType::rev);
        rB.spin(directionType::rev);
        
        this_thread::sleep_for(10);
      }
    } else if (Gyro.value(rotationUnits::deg) > DegreeAmount) {
      while (Gyro.value(rotationUnits::deg) > DegreeAmount)
      {
        /**
        if(Gyro.value(rotationUnits::deg) < DegreeAmount + 25){
          lF.setVelocity(velocL/2,velocityUnits::pct);
          rF.setVelocity(velocR/2,velocityUnits::pct);
          lB.setVelocity(velocL/2,velocityUnits::pct);
          rB.setVelocity(velocR/2,velocityUnits::pct);
        }
        */

        Controller.Screen.clearScreen();
        Controller.Screen.print(DegreeAmount);
        Controller.Screen.print(Gyro.value(rotationUnits::deg));

        lF.spin(directionType::rev); // Assuming this is the polarity needed for a counterclockwise turn
        lB.spin(directionType::rev);
        rF.spin(directionType::fwd);
        rB.spin(directionType::fwd);
        
        this_thread::sleep_for(10);
      }
    }
    //Stop motors after reached degree turn
    stopAll();
    
    Controller.Screen.clearScreen();
    Controller.Screen.print("Gyro Turn Finished");
}

//DegreeAmount (0 - 360) degrees robot will turn
//veloc (0 - 100) percent of motor power given
void gyroTurn3 (double DegreeAmount, int velocL, int velocR, int min){

    //Prints the DegreeAmount for debugging puroses to ensure that it is going for the right degree amount
    Controller.Screen.clearScreen();
    Controller.Screen.print(DegreeAmount);
    Controller.Screen.print(Gyro.value(rotationUnits::deg));

    //While loop to do the spin
    if(Gyro.value(rotationUnits::deg) < DegreeAmount){
      while (Gyro.value(rotationUnits::deg) < DegreeAmount)
      {
        /**
        double val = Gyro.value(rotationUnits::deg);
        lF.setVelocity(velocL*((DegreeAmount - val)/(DegreeAmount)),velocityUnits::pct);
        lB.setVelocity(velocL*((DegreeAmount - val)/(DegreeAmount)),velocityUnits::pct);
        rF.setVelocity(velocR*((DegreeAmount - val)/(DegreeAmount)),velocityUnits::pct);
        rB.setVelocity(velocR*((DegreeAmount - val)/(DegreeAmount)),velocityUnits::pct);
        lF.spin(directionType::fwd); // Assuming this is the polarity needed for a clockwise turn
        lB.spin(directionType::fwd);
        rF.spin(directionType::rev);
        rB.spin(directionType::rev);
        */
        double val = Gyro.value(rotationUnits::deg);
        
        lF.spin(vex::directionType::fwd, fmax(velocL*((DegreeAmount - val)/(DegreeAmount)),min), vex::percentUnits::pct);
        lB.spin(vex::directionType::fwd, fmax(velocL*((DegreeAmount - val)/(DegreeAmount)),min), vex::percentUnits::pct);
        rF.spin(vex::directionType::rev, fmax(velocR*((DegreeAmount - val)/(DegreeAmount)),min), vex::percentUnits::pct);
        rB.spin(vex::directionType::rev, fmax(velocR*((DegreeAmount - val)/(DegreeAmount)),min), vex::percentUnits::pct);
        
        Controller.Screen.clearScreen();
        Controller.Screen.print(DegreeAmount);
        Controller.Screen.print(Gyro.value(rotationUnits::deg));

        this_thread::sleep_for(10);
      }
    } else if (Gyro.value(rotationUnits::deg) > DegreeAmount) {
      double initial = Gyro.value(rotationUnits::deg);
      while (Gyro.value(rotationUnits::deg) > DegreeAmount)
      {
        double val = Gyro.value(rotationUnits::deg);

        lF.spin(vex::directionType::rev, fmax(velocL*((val - DegreeAmount)/(initial - DegreeAmount)),min), vex::percentUnits::pct);
        lB.spin(vex::directionType::rev, fmax(velocL*((val - DegreeAmount)/(initial - DegreeAmount)),min), vex::percentUnits::pct);
        rF.spin(vex::directionType::fwd, fmax(velocR*((val - DegreeAmount)/(initial - DegreeAmount)),min), vex::percentUnits::pct);
        rB.spin(vex::directionType::fwd, fmax(velocR*((val - DegreeAmount)/(initial - DegreeAmount)),min), vex::percentUnits::pct);

        Controller.Screen.clearScreen();
        Controller.Screen.print(DegreeAmount);
        Controller.Screen.print(Gyro.value(rotationUnits::deg));
        
        this_thread::sleep_for(10);
      }
    }
    //Stop motors after reached degree turn
    stopAll();
    
    Controller.Screen.clearScreen();
    Controller.Screen.print("Gyro Turn Finished");
}

int resetGyro() {
  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Calibrating Gyro...");
  Gyro.startCalibration();
  while(Gyro.isCalibrating()){
    vex::this_thread::sleep_for(10);
  }
  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Done!");
  return(0);
}

//drive for a given distance, uses built-in encoder function
//program will wait for the drive to finish if wait == true
void basicEncoderDrive(double pct, double rev, bool wait, double timeLimit) {
  clock_t start;
  start = clock();
  lF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  lB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  rF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  if (wait) {
      rB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
        while (clock() - start / CLOCKS_PER_SEC < timeLimit) {}
    return;
  }
  rB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
}

void basicEncoderDrive(double pct, double rev, bool wait) {
  lF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  lB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  rF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  if (wait) {
      rB.rotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
    return;
  }
  rB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
}

void turnDrive(double pct, double rev, bool wait) {
  rF.startRotateFor(rev - .55, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  lB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  lF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  if (wait) {
      rB.rotateFor(rev - .55, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
      return;
  }
  rB.startRotateFor(rev - .55, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
}

void turnDriveBlue(double pct, double rev, bool wait) {
  lF.startRotateFor(rev - .65, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  rB.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  rF.startRotateFor(rev, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
  if (wait) {
      lB.rotateFor(rev - .65, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
      return;
  }
  lB.startRotateFor(rev - .65, vex::rotationUnits::rev, pct, vex::velocityUnits::pct);
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