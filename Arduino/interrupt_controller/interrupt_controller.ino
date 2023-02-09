/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 01-25-2023
 */

#include "pin_definition.h"

// global flags
volatile int startFlag;
volatile int stopFlag;
enum state { STOP, NORMAL, IDLING };
volatile state currState = IDLING;
static unsigned long last_interrupt_time = 0;

void setup() {
  timerConfig(); // set timer interrupts on xdrive ydrive
  boardInit(); // setup pinmode on I/O pins
  uartInit(); // configure uart communication
}

void loop() {
  switch (currState) {
    case IDLING:
      Serial.println("I");
      digitalWrite(relay, LOW);  // turn on relay
      if (stopFlag) {
        currState = STOP;
      } else if (startFlag) {
        currState = NORMAL;
      }
      break;
    case NORMAL:
      Serial.println("N");
      if (stopFlag) {
        currState = STOP;
      }
      break;
    case STOP:
      Serial.println("S");
      digitalWrite(relay, HIGH);  // turn off relay
      break;
  }
}

// setup board in/out pins here
void boardInit(){
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);  // turn off relay
  attachInterrupt(digitalPinToInterrupt(emergencyButton), emergencyStopISR, RISING);  // setup emergency button interrupt pin, mode=RISING/FALLING (trigger at rising edge)
  attachInterrupt(digitalPinToInterrupt(startButton), startupISR, RISING);
}

void uartInit() {
  Serial.begin(9600);
}

// setup flag to signal stop sequence
void emergencyStopISR(){
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) { // debouncing, ignore any triggers within 100ms
    // code for ISR
    stopFlag = 1;
  }
  last_interrupt_time = interrupt_time;
}

// setup flag to enable functions
void startupISR(){
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) { // debouncing, ignore any triggers within 100ms
    // code for ISR
    if (currState == STOP) {  // reset state
      currState = IDLING;
      startFlag = 0;
      stopFlag = 0;
    } else {
      startFlag = 1;
    }
  }
  last_interrupt_time = interrupt_time;
}

unsigned int counterA = 0;
unsigned int counterB = 0;
// ISR for timer0 interrupt: X-drive motor control -- too fast, will not allocate time to regular sequence
ISR(TIMER0_COMPA_vect){
  counterA++;
  if (counterA == 10) {
    counterA = 0;
    Serial.println("0");
  }
}

// ISR for timer1 interrupt: Y-drive motor control
ISR(TIMER1_COMPA_vect){
  counterB++;
  if (counterB == 10) {
    counterB = 0;
    Serial.println("1");
  }
}
