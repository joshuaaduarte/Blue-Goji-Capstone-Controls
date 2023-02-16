/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#include "serial.h"
#include <stdint.h>
#include <Arduino.h>

const uint8_t numChars = 32;
char receivedChars[numChars];   // an array to store the received data

const uint8_t numPackets = 32;
int receivedInt[numPackets];

void recvWithEndMarker() {
  static uint8_t ndx = 0;
  static uint8_t commaIdx = 0;
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
