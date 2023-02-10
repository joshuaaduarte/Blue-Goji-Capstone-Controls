#include "motor.h"

#define pwmA 3
#define in1A 7
#define in2A 8 
#define pwmB 11
#define in3B 9
#define in4B 10

Motor motorX(pwmA, in1A, in2A);
Motor motorY(pwmB, in3B, in4B);

const byte numPackets = 32;
int receivedInt[numPackets];

unsigned int targetSpeedX, targetSpeedY; // temporary value holder before implementing multiple data parsing
  
void setup(){
  Serial.begin(9600);
  motorInit();
  timerConfig();
  targetSpeedX = 0;
  targetSpeedY = 0;
}

void loop(){
  // receive data from serial monitor
  recvWithEndMarker();
  targetSpeedX = receivedInt[0];
  targetSpeedY = receivedInt[1];
//   update motor values
  motorX.setTargetVel(targetSpeedX);
  motorY.setTargetVel(targetSpeedY);
  // print speed;
  Serial.println((String)motorX.targetSpeed + " " + (String)motorX.currSpeed);
  Serial.println((String)motorY.targetSpeed + " " + (String)motorY.currSpeed);
  Serial.println();
}

void motorInit() {
  motorX.setDir(1); //forward
  motorY.setDir(1); //forward (1), backward (0);
}

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

void recvWithEndMarker() {
  static byte ndx = 0;
  static byte commaIdx = 0;
  char endMarker = '\n';
  char rc;
  
  if (Serial.available() > 0) {
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
    int intToken = atoi(token); // process previous one to int (atof to float, etc.)
    receivedInt[idx] = intToken;
    idx++;
    token = strtok(NULL, ","); // prepare for the next
  }
}
