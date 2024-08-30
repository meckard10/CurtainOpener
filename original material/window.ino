#include <cc1101.h>
#include <pins.h>
#include <macros.h>
#include <registers.h>
#include <stdint.h>
#include "lightSensor.h"

#define LightSensor A12
#define photoresistor P8_3

#define motor1 P4_7 // 2A
#define motor2 P8_0 // 1A
#define motorEnable P6_3 // 1-2E-N

#define restTimeMillis 6000

volatile bool light = false;
volatile bool calib = false;
volatile bool hasOpened = false;
volatile bool motorRun = false;
volatile bool measure1 = false;
volatile bool measure2 = false;

int tests_run = 0;
int assertions = 0;
int tests_passed = 0;

int motorStopRead = 500;
unsigned long currTime = 0;
int tempLight = 0;

unsigned long lastrx;
byte RX_buffer[61]={0};
byte sizerx, i;

//TODO: change radio pins either in schematic or pins.h file
void setup() {
    setupLightSensor();
    
    Serial.begin(9600);

    Radio.Init();
    Radio.SetDataRate(4);
    Radio.SetLogicalChannel(5);
    Radio.SetTxPower(1);
    lastrx = millis();
    Radio.RxOn();
    
    //Run tests
    allLightSensorTests();

    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    pinMode(motorEnable, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    if (tests_passed == tests_run) {
      Serial.println("ALL TESTS PASSED");
    }
  
    else {
      Serial.print("Tests passed: ");
      Serial.println(tests_passed);
      Serial.print("Tests ran: ");
      Serial.println(tests_run);
    }

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(motor1, LOW);
    digitalWrite(motor2,LOW);
    digitalWrite(motorEnable, LOW);

}

void printMsg(byte *buffer, uint16_t size){
  for (int i=0; i < size; i++){
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println(); 
}

void loop() {
    delay(50);
    if(Radio.CheckReceiveFlag()){
      lastrx = millis();
      Serial.print("check = ");
      Serial.println(lastrx);
      sizerx = Radio.ReceiveData(RX_buffer);

      Serial.print(sizerx);
      Serial.println(" bytes!");
      printMsg(RX_buffer, sizerx);

      Radio.RxOn();
      calib = true;
      measure1 = true;
      
    }
    if (calib){
      digitalWrite(GREEN_LED, LOW);
     //measure light
     if(measure1){
     tempLight = readLightOnce();
     currTime = millis();
     measure1 = false;
     }
     //if we have waited long enough
     measure2 = (millis() > currTime- restTimeMillis);
     if(measure2){
        int temp = readLightOnce();
        //probably a better way to do this, but basically sets the min light val to the midpoint of the two values
        if(tempLight > temp){
          setMinLightVal(temp + ((tempLight-temp)/2));
        }else{
          setMinLightVal(tempLight + ((temp-tempLight)/2));
        }
        calib = false;
     }
      
    }else {
      Serial.println(analogRead(LightSensor));
      Serial.println(analogRead(photoresistor));
      waitAndReadLight();
      double average = computeLightAvg();
    
      if(average > minLightVal){
        light = true;
        //open curtain
      }else{
        light = false;
      }
  
      if(light && !hasOpened){
        //double check light readings
        waitAndReadLight();
        int avg = computeLightAvg();
        if(avg > minLightVal){
          digitalWrite(RED_LED, HIGH);
          //if the light is still bright enough:
          //drive the motor
          digitalWrite(motorEnable, HIGH);
          digitalWrite(motor1, LOW);
          digitalWrite(motor2, HIGH);
          motorRun = true;
          delay(3000);
          Serial.print("curtain is open, stopping");
          digitalWrite(RED_LED, LOW);
          Serial.println(analogRead(photoresistor));
          // stop the motor
          digitalWrite(motorEnable,LOW);
          digitalWrite(motor1, LOW);
          digitalWrite(motor2, LOW);
          motorRun = false;
          light = false;
          //set the open window flag
          hasOpened = true;
          currTime = millis();
            
        }else{
          Serial.print("curtains did not open, light was to low: ");
        }
        //if we sense that the window is open, and we are currently running the motor:
        Serial.println(avg);
        Serial.println(analogRead(LightSensor));
        
      }
      if(currTime > restTimeMillis){
        //if we have waited for long enough, reset the window open flag
        hasOpened = false;
      }
    }
}
  
  //if we have recieved a calibrate signal:
    //start listening for more commands(in low power mode, if possible)
    //recieve nightime signal, take reading
    //low power mode
    //recieve sunrise signal, take reading
    //end calibration
  //if no calibrate signal
    //low power mode until the light sensor reaches a certain reading
    //exit lowpower mode
    // we will probably need to wait for the light sensor to stay above the reading
    //cause otherwise it could open up after a car goes by or something
    //so wait for about 15 seconds
    //spin motor
    //wait until photo resistor sees the curtain get close
    //stop the motor
    //end if
 

void light_isr(){
  light = true;
  wakeup();
  
}
