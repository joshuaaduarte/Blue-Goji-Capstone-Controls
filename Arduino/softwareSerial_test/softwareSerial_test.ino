#include <SoftwareSerial.h>
#include "serial.h"


// Define the SoftwareSerial object
SoftwareSerial port(A4, A5);  // RX, TX

void setup() {
  // Initialize the Serial monitor for debugging
  Serial.begin(9600);
  memset(receivedInt, 0, sizeof(int)*numPackets); // reset serial buffer
  while (!Serial) {}

  // Initialize the SoftwareSerial port
  port.begin(9600);
}

void loop() {
  
//   Check for incoming data on the SoftwareSerial port
//  if (port.isListening()) { //sanity check for port
//    Serial.println("listening");
//  }
  if (port.available()) {
  //  Read the data and print it to the Serial monitor
    char data = port.read();
    Serial.print(data);
  }
//^works

//  recvWithEndMarker();
//  Serial.println((String)receivedInt[0] + ", " + (String)receivedInt[1]);
}
