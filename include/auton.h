#include "vex.h"
#include "./common.h"
#include <cmath>
#include <ratio>
#include <vector>
using namespace vex;

double squareDistance = 1.79;
double rightTurnDistance = 0.75;
//double fullTurnDistance = 2.0;

double distance1a = 3; //to pick up cubes
double distance1b = 3-1.133; //backward
double turn1a = 0.99; //turn to face zone
double distance1c = 1.91-1.139; //drive to zone
double distance1d = 2; //back away from zone

double distance2a = 1; //distance to  cube
double turn2a = 0.5; //turn toward cube
double distance2b = 2.269-0.7; //drive to cube
double turn2b = 0.5; //turn toward zone
double distance2c = 0.2*squareDistance; //drive to zone
double distance2d = distance2d;//back away from zone

void noAuton() {
  deployTray();
}

void redAuto(){
  setBraking();
  resetGyro();
  
  deployTray();
  spinIntake(100);

  basicEncoderDrive(30,4,true);
  
  basicEncoderDrive(30,-4,true);

  spinIntake(0);

  gyroTurn2(95,25,25);

  basicEncoderDrive(30,.35,true, 2);

  moveTray(-90);

  wait(3000);

  moveTray(0);

  basicEncoderDrive(35,-1,true, 2);
}

void redAuto2(){
  setBraking();
  resetGyro();
  
  //deployTray();
  spinIntake(100);

  basicEncoderDrive(30,4,true);
  
  basicEncoderDrive(30,-3,true);

  gyroTurn2(90,25,25);

  basicEncoderDrive(30, 1.5,true, 2);

  gyroTurn2(10,-10,25);

  basicEncoderDrive(30,3,true);

  basicEncoderDrive(30,-4,true);

  spinIntake(0);

  gyroTurn2(95,25,25);

  basicEncoderDrive(30,.35,true, 2);

  moveTray(-90);

  wait(3000);

  moveTray(0);

  basicEncoderDrive(35,-1,true, 2);
}

void blueAuto(){
  setBraking();
  resetGyro();
  
  deployTray();
  spinIntake(100);

  basicEncoderDrive(30,4,true);
  
  /**
  rF.setVelocity(30,velocityUnits::pct);
  rB.setVelocity(30,velocityUnits::pct);
  lF.setVelocity(30,velocityUnits::pct);
  lB.setVelocity(30,velocityUnits::pct);
  clock_t start;
  start = clock();
  while((clock() - start)/CLOCKS_PER_SEC < 3){
    lF.spin(directionType::rev); // Assuming this is the polarity needed for a clockwise turn
    lB.spin(directionType::rev);
    rF.spin(directionType::rev);
    rB.spin(directionType::rev);
  }
  */
  basicEncoderDrive(30,-4,true);

  spinIntake(0);

  gyroTurn2(-95,25,25);

  basicEncoderDrive(30,.5,true, 2);

  moveTray(-90);

  wait(3000);

  moveTray(0);

  basicEncoderDrive(35,-1,true, 2);

}

void red1() {
  deployTray();
  
  spinIntake(100);
  basicEncoderDrive(48, distance1a, true); //drive and pick up cubes
  wait(690);

  basicEncoderDrive(50, -distance1b, true); //drive back
  wait(200);
  
  basicEncoderTurn(50, turn1a, true); //turn left to face zone
  wait(500);
  spinIntake(0);

  basicEncoderDrive(50, distance1c, true); //drive to zone
  wait(100);

  outtakeSome();

  while(trayPot.value(vex::analogUnits::range12bit) > maxPotVal) {
    moveTray(60);
  }
  moveTray(0);
  wait(500);
  
  basicEncoderDrive(40, -distance1d, true);
}


void red2() {
  deployTray();
  /*
  spinIntake(100);
  basicEncoderDrive(50, distance2a, true); // drive pick up cube
  wait(100);
  basicEncoderTurn(50, -turn2a, true); //turn left
  wait(100);
  basicEncoderDrive(50, distance2b, true); //drive pick up cube
  wait(500);
  spinIntake(0);
  basicEncoderTurn(50, -turn2b, true); //turn to face zone
  basicEncoderDrive(50, distance2c, true); //drive to zone

  while(trayPot.value(vex::analogUnits::range12bit) > maxPotVal) {
    moveTray(50);
  }
  moveTray(0);
  wait(500);
  basicEncoderDrive(40, -distance2d, true);
  */
}



void blue1() {
  deployTray();
  
  spinIntake(100);
  basicEncoderDrive(48, distance1a, true); //drive and pick up cubes
  wait(690);

  basicEncoderDrive(50, -distance1b, true); //drive back
  wait(200);
  
  basicEncoderTurn(50, -turn1a, true); //turn left to face zone
  wait(500);
  spinIntake(0);

  basicEncoderDrive(50, distance1c, true); //drive to zone
  wait(100);

  outtakeSome();

  while(trayPot.value(vex::analogUnits::range12bit) > maxPotVal) {
    moveTray(60);
  }
  moveTray(0);
  wait(500);
  
  basicEncoderDrive(40, -distance1d, true);
}


void blue2() {
  deployTray();
  /*
  spinIntake(100);
  basicEncoderDrive(50, distance2a, true); // drive pick up cube
  wait(100);
  basicEncoderTurn(50, turn2a, true); //turn right
  wait(100);
  basicEncoderDrive(50, distance2b, true); //drive pick up cube
  wait(500);
  spinIntake(0);
  basicEncoderTurn(50, turn2b, true); //turn right to face zone
  basicEncoderDrive(50, distance2c, true); //drive to zone

  while(trayPot.value(vex::analogUnits::range12bit) > maxPotVal) {
    moveTray(50);
  }
  moveTray(0);
  wait(500);
  basicEncoderDrive(40, -distance2d, true);
  */
}

void test() {
  outtakeSome();
}

