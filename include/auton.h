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

//RedAuto, picks up 4 cubes
void RedAuto(){
  setBraking();

  vex::thread t(deployTray);
  resetGyro();
  
  t.join();

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
//RedAuto2, attempts to pick up 8 cubes rather than 4
void RedAuto2() {

  //Reset gyros and set motors to break
  setBraking();
  /**
  vex::thread t(deployTray);
  resetGyro();
  t.join();
  */
  resetGyro();
  vex::thread t(deployTray);

  //Start intaking
  basicEncoderDrive(40,.8,true);
  spinIntake(100);

  t.join();

  moveArm(-30);

  basicEncoderDrive(40,2.3,true);

  //Drive forward to pick up 3 cubes
  //basicEncoderDrive(40,3.2,true);

  gyroTurn3(-35,15,15, 10);
  //basicEncoderDrive(99,-3.15,true);
  //gyroTurn3(-11,35,35,15);


  turnDrive(99, -3.05, true);
  //Turn towards the corner


  basicEncoderDrive(50,1.5,true);

  basicEncoderDrive(40,1.2,true);

  basicEncoderDrive(40,1.1,true);

  //Pick up 4 cubes
  //basicEncoderDrive(30,3.8,true);

  //Drive back to the corner
  basicEncoderDrive(80,-3.55,true);

  spinIntake(-10);
  moveTray(-80);

  //Stop intaking
  //Turn towards the corner
  gyroTurn3(100,50,50,5);

  //Drive up to the corner
  basicEncoderDrive(30,.8,true);

  //Place our cubes
  wait(400);

  //Back away from our cubes
  basicEncoderDrive(50,-1,true);
  moveTray(0);


  spinIntake(-10);

}
/*
void BlueAuto2() {

  //Reset gyros and set motors to break
  setBraking();
  vex::thread t(deployTray);
  resetGyro();
  t.join();

  //Start intaking
  spinIntake(100);

  //Drive forward to pick up 3 cubes
  basicEncoderDrive(30,3,true);

  gyroTurn3(18,15,15);
  
  //Drive most of the way back to the wall
  basicEncoderDrive(70,-3.4,true);

  //Turn towards the corner
  gyroTurn3(10,25,25);
 
  //Pick up 4 cubes
  basicEncoderDrive(25,3.5,true);

  //Drive back to the corner
  basicEncoderDrive(80,-3.5,true);

  moveTray(-70);

  //Stop intaking
  //Turn towards the corner
  gyroTurn3(-100,60,60);
    
  spinIntake(0);

  //Drive up to the corner
  basicEncoderDrive(30,.5,true);

  //Place our cubes
  wait(1500);
  moveTray(0);

  //Back away from our cubes
  basicEncoderDrive(50,-1,true);
}

void RedAuto3() {

  //Reset gyros and set motors to break
  setBraking();
  resetGyro();

  deployTray();
  //Start intaking
  spinIntake(100);

  //Drive forward to pick up 3 cubes
  basicEncoderDrive(40,3.5,true);

  //gyroTurn2(180,50,-20);

  gyroTurn3(35,25,25);
  
  basicEncoderDrive(40,1.1,true);

  gyroTurn3(70,25,25);

  basicEncoderDrive(30, .6,true);

  gyroTurn3(90,25,25);

  basicEncoderDrive(30, .6,true);

  gyroTurn3(125,40,40);

  gyroTurn2(195, 25,-15);

  basicEncoderDrive(30, -1,true);

  gyroTurn3(210,40,40);

  basicEncoderDrive(30, -1,true);

  gyroTurn3(240,20,20);

  basicEncoderDrive(35,.5,true);

  gyroTurn2(160, -10,25);

  basicEncoderDrive(35,1.5,true);

  //Stop intaking
  spinIntake(0);

  gyroTurn3(100,50,50);

  //Drive up to the corner
  basicEncoderDrive(50,1.2,true);

  moveTray(-100);

  //Place our cubes
  wait(2250);
  moveTray(0);

  //Back away from our cubes
  basicEncoderDrive(50,-1,true);
}

void RedAuto4() {

  //Reset gyros and set motors to break
  setBraking();
  //vex::thread t(deployTray);
  resetGyro();
  //t.join();

  Controller.Screen.clearScreen();

  //Start intaking
  spinIntake(100);

  //Drive forward to pick up 3 cubes
  basicEncoderDrive(30,3.2,true);

  //gyroTurn2(180,50,-20);

  gyroTurn3(-15,25,25);
  
  basicEncoderDrive(40,.6,true);

  gyroTurn3(-70,25,25);

  gyroTurn2(-190, -15,45);

  basicEncoderDrive(30, 1.5,true);

  gyroTurn3(-233,40,40);

  basicEncoderDrive(45, 2.5,true);
  
  //Stop intaking
  spinIntake(0);

  moveTray(-100);
  //Place our cubes
  wait(2750);
  moveTray(0);

  //Back away from our cubes
  basicEncoderDrive(50,-1,true);
}

void blueAuto(){
  setBraking();
  //resetGyro(false);
  
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
  
  basicEncoderDrive(30,-4,true);

  spinIntake(0);

  gyroTurn2(-95,25,25);

  basicEncoderDrive(30,.5,true, 2);

  moveTray(-90);

  wait(3000);

  moveTray(0);

  basicEncoderDrive(35,-1,true, 2);

}

void AutoChoice(){
    if(!Controller.ButtonA.pressing())
      RedAuto2();
    else
      RedAuto();
}

void test(){
  setBraking();
  resetGyro();

  gyroTurn3(90,40,40);
    wait(1000);

  gyroTurn3(0,40,40);
    wait(1000);

  gyroTurn3(90,40,40);
  gyroTurn3(0,40,40);
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
  
}
*/



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