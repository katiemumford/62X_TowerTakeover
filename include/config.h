//#include "C:/Program Files (x86)/VEX Robotics/VEXcode/sdk/vexv5/include/vex_global.h"
#include "./vex_global.h"

//#include "C:/Program Files (x86)/VEX Robotics/VEXcode/sdk/vexv5/include/vex_motorgroup.h"
//#include "C:/Program Files (x86)/VEX Robotics/VEXcode/sdk/vexv5/include/vex_triport.h"
#include "vex.h"
#include <cmath>
#include <ratio>
#include <vector>
using namespace vex;

//FORMALLY KNOWN AS "CONFIG.H"

vex::brain Brain;
vex::competition Competition;
vex::controller Controller;

//drive motors

vex::motor rF (vex::PORT19, true); 
vex::motor rB (vex::PORT9, true);
vex::motor lF (vex::PORT8, false);
vex::motor lB (vex::PORT7, false); 

vex::motor arm (vex::PORT4, false); 
vex::motor tray (vex::PORT20, true);

//right and left intake motors
vex::motor rin (vex::PORT5, true);
vex::motor lin (vex::PORT6, false);

//potentiometer to measure tray's angle of elevation
vex::pot trayPot (Brain.ThreeWirePort.A);

vex::limit trayLimit (Brain.ThreeWirePort.B);

vex::gyro Gyro = vex::gyro(Brain.ThreeWirePort.H);

//vex::gyro gyr (Brain.ThreeWirePort.B);
//vex::accelerometer acc (Brain.ThreeWirePort.C);





