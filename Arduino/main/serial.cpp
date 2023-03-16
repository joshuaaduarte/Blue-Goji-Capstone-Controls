/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#include "serial.h"
#include <stdint.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

// Define the SoftwareSerial object
SoftwareSerial port(A4, A5);  // RX, TX

const uint8_t numChars = 32;
char receivedChars[numChars];   // an array to store the received data

const uint8_t numPackets = 32;
float receivedFloat[numPackets];

void recvWithEndMarker() {
  static uint8_t ndx = 0;
  static uint8_t commaIdx = 0;
  char endMarker = '\n';
  char rc;
  while (port.available() > 0) {  //WARNING: Blocking statement but resolves the timing issue
    rc = port.read();
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
      Serial.println(tempChars);
      // this temporary copy is necessary to protect the original data
      //   because strtok() used in parseData() replaces the commas with \0
      memset(receivedFloat, 0, sizeof(int)*numPackets);
      parseData(tempChars);
      Serial.println("serial:\t"+(String)receivedFloat[0] + ", " + (String)receivedFloat[1]);
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
