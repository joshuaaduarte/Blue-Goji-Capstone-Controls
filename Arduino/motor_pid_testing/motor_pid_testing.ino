#include "motor.h"

// logics for motor A
#define pwmA 6
#define in1A 7
#define in2A 8

// logics for motor B
#define pwmB 11
#define in3B 12
#define in4B 10

// instantiate the two motors
Motor motorX(pwmA, in1A, in2A);
Motor motorY(pwmB, in3B, in4B);

// serial data parser buffer
const uint8_t numChars = 32;
char receivedChars[numChars];   // an array to store the received data

const uint8_t numPackets = 32;
float receivedFloat[numPackets];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motorInit();
  timerConfig();
}

void loop() {
  // put your main code here, to run repeatedly:
  recvWithEndMarker();
  float coordinateX = receivedFloat[0];
  float coordinateY = receivedFloat[1];
  PID(coordinateX, coordinateY);
}

// setup motor directions and speeds
void motorInit() {
  motorX.setDir(1); //forward
  motorY.setDir(1); //forward (1), backward (0);
  motorX.setTargetVel(0);
  motorY.setTargetVel(0);
}

void PID(float coordinateX, float coordinateY) {
  // calculate the target velocities in x and y
  float targetSpeedX = 0;
  float targetSpeedY = 0;
  motorX.setTargetVel(map(targetSpeedX, 0, 15, 0, 255));
  motorY.setTargetVel(map(targetSpeedY, 0, 15, 0, 255));
}

void timerConfig() {
  // disable all interrupts  
  cli();

  // configure timer1 to the frequency for y-drive (400 Hz)
  TCCR1A = 0; // reset registerA for timer1
  TCCR1B = 0; // reset registerB for timer1
  TCNT1 = 0;  // reset timer1 counter to 0
  OCR1A = 32; // set compare match register trigger value
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS12)| (1 << CS10); // Set CS02 bit for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  // enable all interrupts
  sei(); 
}

volatile unsigned int counter = 0;
const unsigned int DIVIDER = 1;  // divider of 100Hz --> 20Hz

// ISR for timer1 interrupt: Y-drive motor control
ISR(TIMER1_COMPA_vect){
  counter++;
  if (counter == DIVIDER){
    motorX.updateVel();
    motorY.updateVel();
    counter = 0;
  }
}

void recvWithEndMarker() {
  static uint8_t ndx = 0;
  static uint8_t commaIdx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial.available() > 0) {  //WARNING: Blocking statement but resolves the timing issue
    rc = Serial.read();
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
          ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0'; // terminate the string
      char tempChars[numChars];
      strcpy(tempChars, receivedChars);
      // this temporary copy is necessary to protect the original data
      //   because strtok() used in parseData() replaces the commas with \0
      memset(receivedFloat, 0, sizeof(int)*numPackets);
      parseData(tempChars);
      ndx = 0;
    }
  }
}

void parseData(char* tempChars) {      // split the data into its parts
  char * token; // this is used by strtok() as an index
  int idx = 0;
  token = strtok(tempChars, ",");
  
  while ( token != NULL) {
    float floatToken = atof(token); // process previous one to int (atof to float, etc.)
//    Serial.println(floatToken);
    receivedFloat[idx] = floatToken;
    idx++;
    token = strtok(NULL, ","); // prepare for the next
  }
}
