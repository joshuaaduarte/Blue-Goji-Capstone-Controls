// setup timer interrupt for x, y drive
void timerConfig() {
  // disable all interrupts  
  cli();

  // configure timer0 to the frequency for x-drive (100 Hz)
  TCCR0A = 0; // reset registerA for timer0
  TCCR0B = 0; // reset registerB for timer0
  TCNT0 = 0;  // reset timer0 counter to 0
  OCR0A = 155; // set compare match register trigger value, increment motor by 5
  TCCR0A |= (1 << WGM01); // turn on CTC mode
  TCCR0B |= (1 << CS02) | (1 << CS00); // Set CS02, CS00 bits for 1024 prescaler
  TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt

//  // configure timer1 to the frequency for y-drive (100 Hz)
//  TCCR1A = 0; // reset registerA for timer1
//  TCCR1B = 0; // reset registerB for timer1
//  TCNT1 = 0;  // reset timer1 counter to 0
//  OCR1A = 155; // set compare match register trigger value, increment motor by 5
//  TCCR1B |= (1 << WGM12); // turn on CTC mode
//  TCCR1B |= (1 << CS12)| (1 << CS10); // Set CS02 bit for 1024 prescaler
//  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  // enable all interrupts
  sei(); 
}

unsigned int counterA, counterB = 0;
const unsigned int DIVIDER = 2;  // divider of 100Hz --> 50Hz

// ISR for timer0 interrupt: X-drive motor control -- too fast, will not allocate time to regular sequence
ISR(TIMER0_COMPA_vect){
  counterA++;
  if (counterA == DIVIDER){
    motorX.updateVel();
    motorY.updateVel();
    counterA = 0;
  }
}

// ISR for timer1 interrupt: Y-drive motor control
//ISR(TIMER1_COMPA_vect){
//  counterB++;
//  if (counterB == DIVIDER) {
//    motorY.updateVel();
//    counterB = 0;
//  }
//}
