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
void PIDTest2(){
  inert.startCalibration();
  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Calibrating Inert...");
  while(inert.isCalibrating()){
    vex::this_thread::sleep_for(10);
  }
  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Done!");
  while(1){
    inert.heading();
  }
}
void PIDTest() {
  PID * pPID= new  PID(.45, 0, 0);

  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Calibrating Inert...");
  inert.startCalibration();
  while(inert.isCalibrating()){
    vex::this_thread::sleep_for(10);
  }
  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Done!");

  pPID->turn(45);
  wait(1000);
  pPID->turn(0);
  
  wait(1000);
  pPID->turn(90);
  wait(1000);
  pPID->turn(0);
  /**
  wait(1000);
  pPID->turn(180);
  wait(1000);
  pPID->turn(0);
  */
}
//Helper classes for SkillsAuto
void placeCubes(){
  spinIntake(-40);//Outake to get cube at the right spot
  wait(900);
  spinIntake(0);
  moveTray(-90);//place cubes
  wait(1800);
  moveTray(-50);
  wait(1000);
  moveTray(0);
  basicEncoderDrive(18,-1,true);
  spinIntake(0);
}
void placeTower(){
  spinIntake(-30);
  wait(150);
  spinIntake(0);
  basicEncoderDrive(30,-.5,true);//Place cube in tower
  running = true;
  wait(2000);
  basicEncoderDrive(30,.4,true);
  running2 = true;
  wait(500);
  basicEncoderDrive(30,-.2,true);
  running3 = true;
  wait(2000);
}
//6 cubes on unprotected side
void RedAuto(){
  basicEncoderDrive(70,.5,true);
  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread
  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power
  wait(200); //Wait for 200ms for arm to fall

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(30,4,true);

  spinIntake(70); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(50,-3.75,true);

  spinIntake(0);
  gyroTurn3(100,35,35,10);

  moveTray(-80);
  spinIntake(-25);
  basicEncoderDrive(30,.75,true);

  spinIntake(-5);
  wait(2500);
  spinIntake(-60);
  moveTray(0);

  basicEncoderDrive(18,-1,true);
}
void BlueAuto(){
  basicEncoderDrive(70,.5,true);
  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread
  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power
  wait(200); //Wait for 200ms for arm to fall

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(30,4,true);

  spinIntake(70); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(50,-3.75,true);

  spinIntake(0);
  gyroTurn3(-100,35,35,10);

  moveTray(-80);
  spinIntake(-25);
  basicEncoderDrive(30,.35,true);

  spinIntake(-5);
  wait(2500);
  spinIntake(-60);
  moveTray(0);

  basicEncoderDrive(18,-1,true);
}
//2 cubes on protected side
void RedAutoTall(){
  setBraking(); //Set motors to break

  /**
  vex::thread t(deployTray);
  resetGyro();
  t.join();
  */

  resetGyro(); //Calibrate the Gyroscope

  basicEncoderDrive(70,.5,true);

  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread

  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power

  wait(200); //Wait for 200ms for arm to fall

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(30,2,true);

  gyroTurn3(-120,35,35,10);

  spinIntake(0);

  basicEncoderDrive(30,1.7,true);

  spinIntake(-100);

  wait(1500);

  basicEncoderDrive(25,-1,true, 2);
}
void BlueAutoTall(){
  setBraking(); //Set motors to break

  /**
  vex::thread t(deployTray);
  resetGyro();
  t.join();
  */

  resetGyro(); //Calibrate the Gyroscope

  basicEncoderDrive(70,.5,true);

  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread

  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power

  wait(200); //Wait for 200ms for arm to fall

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(30,2,true);

  gyroTurn3(120,45,45,10);

  spinIntake(0);

  basicEncoderDrive(30,1,true);

  spinIntake(-100);

  wait(2600);

  basicEncoderDrive(35,-1,true, 2);

}
//8 cubes on unprotected side(WIP)
void RedAuto2(){
   
 setBraking(); //Set motors to break
 
 /**
 vex::thread t(deployTray);
 resetGyro();
 t.join();
 */
 
 resetGyro(); //Calibrate the Gyroscope
  vex::thread t(deployTray); //Start deploy thread
 
 spinIntake(100); //Start intaking and drive forward as deploy is happening
 basicEncoderDrive(40,.8,true);
 
 t.join(); //Wait for deploy thread to finish
 
 moveArm(-30); //Set arm to downward power
 
 wait(200); //Wait for 200ms for arm to fall
 
 basicEncoderDrive(40,2.3,true); //Drive forward to pick up the 3 cubes
 
 //basicEncoderDrive(40,3.2,true); //Drive forward to pick up 3 cubes
 
 gyroTurn3(-35,15,15, 10); // Turn towards the center, back towards the wall
 
 //basicEncoderDrive(99,-3.15,true);
 //gyroTurn3(-11,35,35,15);
 
 turnDrive(99, -2.85, true); //Drive backwards, swerve at the end to face stack of 4
 
 /**
 basicEncoderDrive(50,1.5,true);
 basicEncoderDrive(40,1.2,true);
 basicEncoderDrive(40,1.1,true);
 */
 
 basicEncoderDrive(35,3.8,true);  //Pick up 4 cubes
 
 if(Gyro.value(rotationUnits::deg) > 10){
   gyroTurn3(5,35,35,5);
 } else if(Gyro.value(rotationUnits::deg) < -10){
   gyroTurn3(-5,35,35,5);
 }
 
 basicEncoderDrive(80,-3.35,true); //Drive back to the corner
 
 spinIntake(-10);
 
 //Stop intaking
 //Turn towards the corner
 gyroTurn3(112,50,50,5);
 
 moveTray(-80);
 
 //Drive up to the corner
 basicEncoderDrive(25,.8,true);
 
 //Place our cubes
 wait(400);
 
 spinIntake(-50);
 
 //Back away from our cubes
 basicEncoderDrive(50,-1,true);
 moveTray(0);
 
 
 spinIntake(-10);
}
void BlueAuto2() {

    setBraking(); //Set motors to break

  /**
  vex::thread t(deployTray);
  resetGyro();
  t.join();
  */

  resetGyro(); //Calibrate the Gyroscope
  
  vex::thread t(deployTray); //Start deploy thread

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(40,.8,true);

  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power

  wait(200); //Wait for 200ms for arm to fall

  basicEncoderDrive(40,2.3,true); //Drive forward to pick up the 3 cubes

  //basicEncoderDrive(40,3.2,true); //Drive forward to pick up 3 cubes

  gyroTurn3(28,20,20, 10); // Turn towards the center, back towards the wall

  //basicEncoderDrive(99,-3.15,true);
  //gyroTurn3(-11,35,35,15);

  turnDriveBlue(99, -2.65, true); //Drive backwards, swerve at the end to face stack of 4

  /**
  basicEncoderDrive(50,1.5,true);
  basicEncoderDrive(40,1.2,true);
  basicEncoderDrive(40,1.1,true);
  */

  basicEncoderDrive(35,3.5,true);  //Pick up 4 cubes

  if(Gyro.value(rotationUnits::deg) > 10){
    gyroTurn3(5,35,35,5);
  } else if(Gyro.value(rotationUnits::deg) < -10){
    gyroTurn3(-5,35,35,5);
  }

  basicEncoderDrive(80,-3.7,true); //Drive back to the corner

  spinIntake(-10);

  //Stop intaking
  //Turn towards the corner
  moveTray(-80);

  gyroTurn3(-112,50,50,5);

  //Drive up to the corner
  basicEncoderDrive(30,.5,true);

  //Place our cubes
  wait(400);

  spinIntake(-50);

  //Back away from our cubes
  basicEncoderDrive(50,-1,true);
  moveTray(0);


  spinIntake(-10);
}

//6 cubes on protected side
void RedAutoProt(){
  basicEncoderDrive(70,.5,true);
  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread
  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power
  wait(200); //Wait for 200ms for arm to fall

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  
  basicEncoderDrive(35,3.5,true);
  gyroTurn3(-50,20,20,10);
  
  basicEncoderDrive(40,.6,true);
  gyroTurn3(-150,25,25,10);

  basicEncoderDrive(60,2,true);

  gyroTurn3(Gyro.value(rotationUnits::deg)+5,5,5,5);

  basicEncoderDrive(60,.7,true);

  basicEncoderDrive(30,.35,true);
  
  spinIntake(-20);
  wait(500);
  moveTray(-80);
  wait(2500);
  spinIntake(-40);
  moveTray(0);
  basicEncoderDrive(18,-1,true);
}
void BlueAutoProt(){
  basicEncoderDrive(70,.5,true);
  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread
  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power
  wait(200); //Wait for 200ms for arm to fall
  spinIntake(100); //Start intaking and drive forward as deploy is happening
  
  basicEncoderDrive(35,3.5,true);
  gyroTurn3(50,20,20,10);

  basicEncoderDrive(40,.6,true);
  gyroTurn3(145,50,50,10);

  basicEncoderDrive(60,2.95,true);
  spinIntake(-20);
  gyroTurn3(145,10,10,10);
  spinIntake(0);
  basicEncoderDrive(30,.25,true);

  moveTray(-80);
  wait(2750);
  spinIntake(-60);
  moveTray(0);

  basicEncoderDrive(18,-1,true);
}
//10 cubes, 1 tower, 6 cubes, 1 tower
void SkillsAuto(){
  inAuto = true;
  vex::thread arms(armControllerAuton);

  basicEncoderDrive(70,.5,true);
  basicEncoderDrive(70,-.3,true);

  vex::thread t(deployTray); //Start deploy thread
  t.join(); //Wait for deploy thread to finish

  moveArm(-30); //Set arm to downward power
  wait(200); //Wait for 200ms for arm to fall

  spinIntake(100); //Start intaking and drive forward as deploy is happening
  basicEncoderDrive(40,9,true); //Pick up all 8 cubes

  spinIntake(60); //Start intaking and drive forward as deploy is happening
  gyroTurn3(35,20,20,10);//Turn to blue area

  basicEncoderDrive(30,.8,true); //Drive to blue area

  placeCubes();

  moveTray(80);
  gyroTurn3(-95,45,45,10);//Turn towards tower

  spinIntake(100);
  basicEncoderDrive(40,2.9,true);//Drive towards tower and pick up cube
  spinIntake(0);

  placeTower();

  spinIntake(100);
  gyroTurn3(-190,35,35,10);//Turn towards cubes

  basicEncoderDrive(40,8,true); //Pick up all 6 cubes
  gyroTurn3(-260,30,30,10);//Turn towards Red corner
  basicEncoderDrive(40,2.6,true); //Drive to red corner

  placeCubes();

  moveTray(80);
  gyroTurn3(-110,30,30,10);//Turn towards cubes

  spinIntake(100);
  basicEncoderDrive(40,2.0,true);//Drive towards tower and pick up cube
  moveTray(0);

  spinIntake(0);

  placeTower();
}
//Run deploy method
void deployTest(){
  vex::thread t(deployTray); //Start deploy thread
  t.join(); //Wait for deploy thread to finish
}

/*
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
}
*/
