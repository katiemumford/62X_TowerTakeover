#include "config.h"
using namespace vex;

class PID {

  public:
  double Kp, Ki, Kd;

  PID(double newKp, double newKi, double newKd){
    Kp = newKp;
    Ki = newKi;
    Kd = newKd;
  }

  void turn(double target) {
    double speed, current = inert.heading(), dT, prevTime = 0, integral = 0,  derivative = 0;
    double error = target - current;
    double prevError = error;
    bool last = false;
    while(std::abs(error) > 1.5){
      this_thread::sleep_for(10); 

      if(std::abs(error) < .05)
        last = true;
      else
        last = false;

      current = inert.heading();
      if(std::abs(target - current) > std::abs(target - current + 360)){
        error = target - current + 360;
      } else{
        error = target - current;
      }

      dT = vex::timer::system() - prevTime;
      prevTime = vex::timer::system();
      integral = integral + error*dT;
      derivative = ((current - prevError))/dT;
      prevError = error;


      if(error == 0 || std::abs(error) > 45){
        integral = 0;
      }

      speed = (Kp*error) + (Ki*integral) + (Kd*derivative);
  
      if(speed > 100) {
        speed = 100;
      } else if(speed < -100){
        speed = -100;
      }

      Controller.Screen.clearScreen();
      Controller.Screen.newLine();
      Controller.Screen.print(speed);
      
      lF.spin(forward, speed, vex::percentUnits::pct);
      lB.spin(forward, speed, vex::percentUnits::pct);
      rF.spin(reverse, speed, vex::percentUnits::pct);
      rB.spin(reverse, speed, vex::percentUnits::pct);
    }
    lF.stop();lB.stop();rF.stop();rB.stop();
  }
};
