/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

class Motor {
  private:
    // private fields
    uint8_t pwm, in1, in2;

  public:
    // public fields
    int targetSpeed;
    int currSpeed;
    uint8_t dir;
    // constructor
    Motor(uint8_t pwm, uint8_t in1, uint8_t in2);
    // getters
    uint8_t getPwmPin();
    uint8_t getIn1Pin();
    uint8_t getIn2Pin();
    // setters
    void setDir(uint8_t forward);
    void setTargetVel(int vel);
    // motor driving functions
    void brake();
    void coast();
    void updateVel();
};

#endif
