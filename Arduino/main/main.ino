/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-15-2023
 */

#include "pin_definition.h"
#include "motor.h"
#include "serial.h"

// global flags
volatile int startFlag;
volatile int stopFlag;
enum state { STOP, NORMAL, IDLING };
volatile state currState = IDLING;
static unsigned long lastInterruptTime = 0;

// instantiate the two motors
Motor motorX(pwmA, fwA, enA);
Motor motorY(pwmB, fwB, enB);

/*************ARDUINO MAIN CODE*************/

void setup() {
  boardInit(); // setup pinmode on I/O pins
  timerConfig(); // set timer interrupts on xdrive ydrive
  uartInit(); // configure uart communication
  motorInit(); // initalizes motor directions
}

void loop() {
  switch (currState) {
    case IDLING:
      // state transition logic
      if (stopFlag) {
        currState = STOP;
      } else if (startFlag) {
        currState = NORMAL;
      }
      // main code
      idleRun();
      break;
    case NORMAL:
      // state transition logic
      if (stopFlag) {
        currState = STOP;
      }
      // main code
      normalRun();
      break;
    case STOP:
      // main code
      stopRun();
      break;
  }
}

/*************STATE MAIN CODE*************/

void idleRun() {
  Serial.println("I ");
  memset(receivedFloat, 0, sizeof(int)*numPackets); // reset serial buffer
  motorX.setTargetVel((unsigned int)0);
  motorY.setTargetVel((unsigned int)0);
  digitalWrite(relay, LOW);  // turn on relay
}

void normalRun() {
//  Serial.println("N ");
  // receive data from serial monitor
  recvWithEndMarker();
  Serial.println("run:\t"+(String)receivedFloat[0] + ", " + (String)receivedFloat[1]);
  if (port.overflow()){ // health check
    Serial.println("OVERFLOWWWW");
  }
  // update motor values
  float targetSpeedX = receivedFloat[0];
  float targetSpeedY = receivedFloat[1];
  motorX.setTargetVel(map(targetSpeedX, 0, 15, 0, 255));
  motorY.setTargetVel(map(targetSpeedY, 0, 15, 0, 255));
  // print speed;
//  Serial.println((String)motorX.targetSpeed + ":" + (String)motorX.currSpeed + ", " + (String)motorY.targetSpeed + ": " + (String)motorY.currSpeed);
}

void stopRun() {
  Serial.println("S");
  // clear receivedInt buffer
  memset(receivedFloat, 0, sizeof(int)*numPackets);
  motorX.setTargetVel((unsigned int)0);
  motorY.setTargetVel((unsigned int)0);
  digitalWrite(relay, HIGH);  // turn off relay
}

/*************INITIALIZATIONS*************/

// setup board in/out pins here
void boardInit(){
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);  // turn off relay
  attachInterrupt(digitalPinToInterrupt(emergencyButton), emergencyStopISR, RISING);  // setup emergency button interrupt pin, mode=RISING/FALLING (trigger at rising edge)
  attachInterrupt(digitalPinToInterrupt(startButton), startupISR, RISING);
}

// begin serial communication channel at 9600 baud rate
void uartInit() {
  Serial.begin(9600);
  port.begin(9600);
}

// setup motor directions and speeds
void motorInit() {
  motorX.setDir(1); //forward
  motorY.setDir(1); //forward (1), backward (0);
  motorX.setTargetVel(0);
  motorY.setTargetVel(0);
}

/*************START/STOP HARDWARE BUTTON ISR*************/

// setup flag to signal stop sequence
void emergencyStopISR(){
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 100) { // debouncing, ignore any triggers within 100ms
    // code for ISR
    stopFlag = 1;
  }
  lastInterruptTime = interruptTime;
}

// setup flag to enable functions
void startupISR(){
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 100) { // debouncing, ignore any triggers within 100ms
    // code for ISR
    if (currState == STOP) {  // reset state
      currState = IDLING;
      startFlag = 0;
      stopFlag = 0;
    } else {
      startFlag = 1;
    }
  }
  lastInterruptTime = interruptTime;
}
