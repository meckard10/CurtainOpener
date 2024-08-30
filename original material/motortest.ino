
#define motor1 P8_0//2A
#define motor2 P4_7//1A
#define motorEnable P6_3 //1-2E-N

#define led1 RED_LED
#define led2 GREEN_LED

volatile bool light = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motorEnable, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  //digitalWrite(motor1, LOW);
  //digitalWrite(motor2, HIGH);
  Serial.println("finished setup");
  digitalWrite(motorEnable, HIGH);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2,LOW);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  delay(500);
  
  digitalWrite(motorEnable, LOW);
  
}
