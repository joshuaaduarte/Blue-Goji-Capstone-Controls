/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#include "motor.h"
#include <Arduino.h>  // to access arduino datatypes

// constructor
Motor::Motor(uint8_t pwm, uint8_t in1, uint8_t in2):pwm(pwm),in1(in1),in2(in2) {
  pinMode(pwm, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  analogWrite(pwm, 0);
}

// getters
uint8_t Motor::getPwmPin() {
  return pwm;
}
uint8_t Motor::getIn1Pin() {
  return in1;
}
uint8_t Motor::getIn2Pin() {
  return in2;
}

// setters
void Motor::setDir(uint8_t forward) {
  Motor::dir = forward;
  digitalWrite(in1, dir);
  digitalWrite(in2, !dir);
}
void Motor::setTargetVel(int vel) {
  targetSpeed = vel;
}

// motor driving functions
void Motor::brake() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  analogWrite(pwm, HIGH);
}
void Motor::coast() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(pwm, LOW);
}
void Motor::updateVel() {
  if(currSpeed < targetSpeed) {
    currSpeed = min(currSpeed+1, 255);
  } else if (currSpeed > targetSpeed) {
    currSpeed = max(currSpeed-1, -255);
  }
  analogWrite(pwm, abs(currSpeed));
  if (currSpeed == 0 && currSpeed > targetSpeed) { // if target is backward
    Motor::setDir(0);
  } else if (currSpeed == 0 && currSpeed > targetSpeed) { // if target is forward
    Motor::setDir(1);
  }
}
