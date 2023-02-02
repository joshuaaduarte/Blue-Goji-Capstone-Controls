/* 
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * 
 * timer_interrupt_submodule
 * Author: Billy Lin 
 * Last-Edit: 01-25-2023
 * 
 * References:
 * https://www.instructables.com/Arduino-Timer-Interrupts/
 * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 */

 

void setup() {
  // put your setup code here, to run once:
  timerConfig();
  // setup uart communication
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0){
    Serial.println(Serial.read());
  }
}


// ISR for timer0 interrupt: X-drive motor control -- too fast, will not allocate time to regular sequence
ISR(TIMER0_COMPA_vect){
  Serial.println("0");
}

// ISR for timer1 interrupt: Y-drive motor control
ISR(TIMER1_COMPA_vect){
  Serial.println("1"); 
}
