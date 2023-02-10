#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

class Motor {
  private:
    // private fields
    uint8_t pwm, in1, in2;

  public:
    // public fields
    uint8_t targetSpeed;
    uint8_t currSpeed;
    uint8_t dir;
    // constructor
    Motor(uint8_t pwm, uint8_t in1, uint8_t in2);
    // getters
    uint8_t getPwmPin();
    uint8_t getIn1Pin();
    uint8_t getIn2Pin();
    // setters
    void setDir(uint8_t forward);
    void setTargetVel(uint8_t vel);
    // motor driving functions
    void brake();
    void coast();
    void updateVel();
};

#endif
