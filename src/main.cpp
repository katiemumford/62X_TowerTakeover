#include "vex.h"
#include "auton.h"
#include <cmath>
#include <ratio>
#include <vector>
using namespace vex;

//custom variable-type to define an autonomous program
struct Auton {  
  void(*ref)(); //function reference
  const char* name; //program name
};

int autonNum = 0; //the index (number) of the auton chosen
bool preAutonBool = true;

//array of auton programs
std::vector<Auton> autons = { 
  //{noAuton, "No Auton"},
  {RedAuto2, "Red 6-7"},
  {BlueAuto2, "Blue 6-7"}
  //{blue1, "Blue 1"},
  //{blue2, "Blue 2"},
  //{test, "test"}
};

//*draws the buttons on the Brain screen representing autons
void drawButtons() {  
  Brain.Screen.setPenColor(color::white);

  //goes through array and draws a button for each program
  for (int i = 0; i < autons.size(); i++) {

    //calculates each button's x and y position on screen
    int x = (i > 3) ? (16 + 116*(i-4)) : (16 + 116*i);  
    int y = (i > 3) ? 127 : 14; 

    //if auton is chosen, color its button green instead of blue
    vex::color c = (autonNum == i) ? (color::green) : (color::blue);  

    //display button and auton program name
    Brain.Screen.drawRectangle(x, y, 100, 99, c);
    Brain.Screen.printAt(x+15,y+45, autons[i].name);
  }
}

//*checks if an auton button was recently pressed on screen
void screenPress(int xp, int yp) {  

  //goes through array and checks button for each auton
  for (int i = 0; i < autons.size(); i++) {

    //calculates x and y positions of button for each auton
    int x = (i > 3) ? (16 + 116*(i-4)) : (16 + 116*i);
    int y = (i > 3) ? 127 : 14;

    //if the x and y coordinates of the screen press fall within the button,
    if (xp >= x && xp <= x + 100 && yp >= y && yp < y + 100) {
      autonNum = i; //make that auton the chosen auton
      break;
    }
  }
}

int pre_autonTask() { //the auton selection runs in pre-auton
  while (preAutonBool) {
    if (Brain.Screen.pressing()) {  //if screen is pressed, check if a button was pressed
      screenPress(Brain.Screen.xPosition(), Brain.Screen.yPosition());
    }
    drawButtons();
    Brain.Screen.render();
    task::sleep(200);
    Brain.Screen.clearScreen();
  }
  return 0;
}
bool IsaacDriving = false;
bool prevAutoState = false;
int preAutonGyro(){
  bool ran = false;
  while(!ran && preAutonBool){
    Controller.Screen.clearScreen();
    Controller.Screen.newLine();
    Controller.Screen.print("preAutonBool: ");
    Controller.Screen.print(preAutonBool);
    if(Brain.Screen.pressing() && !ran){
      ran = true;
      resetGyro();
    } 
  }
  ran = false;
  Controller.Screen.clearScreen();
  Controller.Screen.newLine();
  Controller.Screen.print("Isaac Driving: ");
  Controller.Screen.print(IsaacDriving);
  while(preAutonBool){
    Controller.Screen.clearScreen();
    Controller.Screen.newLine();
    Controller.Screen.print("preAutonBool: ");
    Controller.Screen.print(preAutonBool);
    
    if(Brain.Screen.pressing()){
      if(prevAutoState == IsaacDriving){
        IsaacDriving = !IsaacDriving;
      }
      Controller.Screen.newLine();
      Controller.Screen.print("Isaac Driving: ");
      Controller.Screen.print(IsaacDriving);
    } else {
      prevAutoState = IsaacDriving;
    }
  }
  return 0;
}
vex::task p;
void pre_auton( void ) {  
  setBraking(); 
  p.priority(preAutonGyro);
}
bool movingArmsManually = false;
void moveArmsManually(){
  while(true){
    if(Controller.ButtonUp.pressing()){
      moveArm(100);
      movingArmsManually = true;
    } else if(Controller.ButtonDown.pressing()){
      moveArm(-25);
      movingArmsManually = true;
    } else {
      movingArmsManually = false;
      moveArm(0);
    }
  }
}
bool isBraking = false;
bool running4 = false;
bool running4B = false;
void armController(){
  bool running5 = false, running6 = false, running7 = false;
  bool moveOn = false;
  vex::thread moveManual(moveArmsManually);
  while (true && !movingArmsManually){
      if (Controller.ButtonA.pressing()){
        running4 = true;
        running4B = false;
      } else if(Controller.ButtonB.pressing()){
        running4B = true;
        running4 = false;
      }
      uint32_t t1 = timer::system();
      uint32_t t2 = timer::system();
      while((running4 || running4B) && !movingArmsManually){
        t2 = timer::system();
        uint32_t t3 = t2 - t1;
        if(!moveOn){
          if(t3 < 750){
            moveArm(100);
            moveTray(-95);
          } else if(t3 < 800) {
            moveArm(100);
            moveTray(0);
          } else {
            if(running4B){
              moveArm(2);
            }
            moveOn = true;
          }
        } 
         else {
            uint32_t t4 = timer::system();
            uint32_t t5 = timer::system();
            if(Controller.ButtonA.pressing()){
              running5 = true;
            }
            while(running5 && !movingArmsManually){
              t5 = timer::system();
              if(t5-t4 < 500){
                spinIntake(-75);
              } else {
                 uint32_t t6 = timer::system();
                 uint32_t t7 = timer::system();
                 if(Controller.ButtonA.pressing()){
                  running6 = true;
                 } else if (Controller.ButtonB.pressing()){
                   running7 = true;
                 }
                 while(running7 && !movingArmsManually){
                   t7 = timer::system();
                   spinIntake(0);
                   if(t7 - t6 > 1000){
                     if(Controller.ButtonB.pressing()) {
                      running7 = false;
                      running6 = true;
                      t6 = timer::system();
                    }
                   }
                 }
                 while(running6 && !running7 && !movingArmsManually){
                  uint32_t t7 = timer::system();
                  if(t7-t6 < 1250) {
                    spinIntake(0);
                    moveArm(-80);
                  } else if(t7-t6 < 1750) {
                    spinIntake(0);
                    moveTray(80);
                 } else {
                    running4 = false;
                    running5 = false;
                    running6 = false;
                    running7 = false;
                    running4B = false;
                    moveOn = false;
                 }
              }
            }
          } 
        }
      }
      if(movingArmsManually){
        running4 = false;
        running5 = false;
        running6 = false;
        running7 = false;
        running4B = false;
        moveOn = false;
      }
      vex::task::sleep(10); 
  }
}

bool prevDriveState = false;
void usercontrol (void) { 
  inAuto = false;
  preAutonBool = false;
  vex::task::stop(preAutonGyro);
  Brain.Screen.clearScreen(vex::color::black); //stops pre auton and clears screen
  //vex::thread t(deployTray); //Start deploy thread
  //t.join();

  vex::thread buttonAToggleTask(armController);
  int32_t lValue = lin.position(vex::rotationUnits::deg);
  int32_t rValue = rin.position(vex::rotationUnits::deg);
  while (1) {
    if(Controller.ButtonLeft.pressing()){
      if(prevDriveState == IsaacDriving){
        IsaacDriving = !IsaacDriving;
      }
    } else {
      prevDriveState = IsaacDriving;
    }
    if(IsaacDriving)
      vdrive((abs(Controller.Axis3.value())/127.0)*(Controller.Axis3.value()/127.0)*100, (abs(Controller.Axis2.value())/127.0) *(Controller.Axis2.value()/127.0)*100);
    else
      vdrive(Controller.Axis3.value()*100.0/127.0, Controller.Axis2.value()*100/127.0);

    intakeControl(running4 || running4B, rValue, lValue, IsaacDriving);
    trayControl(IsaacDriving);
  
    vex::task::sleep(20); 
  }
}

void theAuton(void) {
  preAutonBool = false;
  autons[autonNum].ref();
}

void auton(void){
  preAutonBool = false;
  RedAutoProt();
}
/**
Universal:
  When not raised:
    A: move arms up to high tower
    B: move arms up to low tower
  When raised:
    A: Outtake
  After Outtake:
    A: Move back down
    B: Stay in place
    When in place:
      B: Move back down
  
  Shortcuts:
    A/A/A: Place tower
    B/A/A: Place low tower
    A/A/B: stay raised on high tower (B again to lower)
    B/A/B: Stay raised on low tower (B again to lower)

  X: Back up automatically
  Y: Move tray up manually
  DpadLeft: Toggle on/off IsaacDriving 
  DpadRight: Move tray up manually
  DpadUp: Move arms up manually
  DpadDown: Move arms down manually

IsaacDriving: 
  R1: Intake on toggle
  L1: Outtake (not on toggle) and cancel intake
  R2: Place Stack, back up, and lower tray
  L2: Lower tray to back
  **Squared drive curve**

KatieDriving (!IsaacDriving)
  R1: Intake (not toggle)
  R2: Outtake (not toggle)
  L1: Move tray up (must be held, goes to point and stops)
  L2: Move tray down automatically
  **1:1 drive**
*/
int main() {
    Competition.autonomous(auton);
    Competition.drivercontrol(usercontrol);
    pre_auton();                        
    while(1) {
      vex::task::sleep(100);
    }        
}
