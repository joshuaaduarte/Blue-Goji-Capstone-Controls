#define pwmA 6
#define in1A 7
#define in2A 8 
#define pwmB 11
#define in3B 9
#define in4B 10

#define FORWARD 0
#define BACKWARD 0

const byte dirA = FORWARD;
const byte dirB = FORWARD;
volatile byte speedA = 0;
volatile boolean increment = true;

void setup() {
  Serial.begin(9600);
  //Setup Channel A
  pinMode(pwmA, OUTPUT); 
  pinMode(in1A, OUTPUT); 
  pinMode(in2A, OUTPUT);

  //Setup Channel B
  pinMode(pwmB, OUTPUT); 
  pinMode(in3B, OUTPUT); 
  pinMode(in4B, OUTPUT);

  //setup direction for a
  digitalWrite(in1A, dirA);
  digitalWrite(in2A, ~dirA);
  analogWrite(pwmA, 0);
  Serial.println("start");
}

void loop(){
  if(increment){
    if (speedA < 255){
      speedA++;
    } else {
      Serial.println("speed maxed");
      increment = false;
    }
  } else {
    if (speedA > 0) {
      speedA--;
    } else {
      Serial.println("speed is 0");
      increment = true;
    }
  }
  Serial.println(speedA);
  analogWrite(pwmA, 100);
  delay(50);
}
