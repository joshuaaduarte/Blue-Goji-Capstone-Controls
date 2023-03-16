/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#ifndef SERIAL_H
#define SERIAL_H

#include "stdint.h"
#include <SoftwareSerial.h>

extern SoftwareSerial port;
extern const uint8_t numChars;
extern char receivedChars[];   // an array to store the received data

extern const uint8_t numPackets;
extern float receivedFloat[];      // an array to store the received decoded data

void recvWithEndMarker();
void parseData(char* tempChars);

#endif
