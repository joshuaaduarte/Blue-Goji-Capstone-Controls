/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#include "motor.h"
#include <Arduino.h>  // to access arduino datatypes

// constructor
Motor::Motor(uint8_t pwm, uint8_t fw, uint8_t en):pwm(pwm),fw(fw),en(en) {
  pinMode(pwm, OUTPUT);
  pinMode(fw, OUTPUT);
  pinMode(en, OUTPUT);
  digitalWrite(en, HIGH);
  analogWrite(pwm, 0);
}

// getters
uint8_t Motor::getPwmPin() {
  return pwm;
}
uint8_t Motor::getFwPin() {
  return fw;
}
uint8_t Motor::getEnPin() {
  return en;
}

// setters
void Motor::setDir(uint8_t forward) {
  Motor::dir = forward;
  digitalWrite(fw, dir);
}
void Motor::setTargetVel(int vel) {
  targetSpeed = vel;
}

// motor driving functions
void Motor::brake() {
  digitalWrite(en, LOW);
  analogWrite(pwm, 0);
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
