#define startButton 2  // Yellow
#define emergencyButton 3 // Green
#define relay 4 

// global flags
volatile int startFlag = 0;
volatile int stopFlag = 0;
enum state { STOP, NORMAL, IDLING };
volatile state currState = IDLING;

static unsigned long last_interrupt_time = 0;

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);  // turn off relay
  Serial.println(digitalRead(relay));
  attachInterrupt(digitalPinToInterrupt(emergencyButton), emergencyStopISR, RISING);  // setup emergency button interrupt pin, mode=RISING/FALLING (trigger at rising edge)
  attachInterrupt(digitalPinToInterrupt(startButton), startupISR, RISING);
}

void loop() {
  switch (currState) {
    case IDLING:
      Serial.println("IDLE");
      digitalWrite(relay, LOW);  // turn on relay
      if (stopFlag) {
        currState = STOP;
      } else if (startFlag) {
        currState = NORMAL;
      }
      break;
    case NORMAL:
      Serial.println("NORMAL");
      if (stopFlag) {
        currState = STOP;
      }
      break;
    case STOP:
      Serial.println("STOP");
      digitalWrite(relay, HIGH);  // turn off relay
      break;
  }
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
