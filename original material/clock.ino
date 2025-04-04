#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

#include "minunit.h"
#include "segment.h"
#include "button.h"
#include "clock.h"

int tests_run = 0;
int tests_passed = 0;
int assertions = 0;

#define microphone P1_3

#define dataRate 4
#define logicChannel 8
#define sizetx 10
#define burstNum 10

byte TX_buffer[sizetx] = {0};

static char* allGeneralTests() {
  return NULL;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("starting!");

  //Setup debug LEDs
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);

  //Setup radio for transmission
  Radio.Init();
  Radio.SetDataRate(dataRate);
  Radio.SetLogicalChannel(logicChannel);
  Radio.SetTxPower(0);

  for (int i = 0; i < sizetx; ++i) {
    TX_buffer[i] = 0;
  }

  //Setup button stuff
  setupButtons();

  //Run all tests
  allButtonTests();
  theDisplay.allSegmentTests();
  allGeneralTests();

  if (tests_passed == tests_run) {
    Serial.println("ALL TESTS PASSED");
  }

  else {
    Serial.print("Tests passed: ");
    Serial.println(tests_passed);
    Serial.print("Tests ran: ");
    Serial.println(tests_run);
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
  //If in calibration mode, blink 
  if (sendTime) {
    digitalWrite(GREEN_LED, HIGH);

    Serial.println("test");
    
    //Send radio burst
    TX_buffer[0] = 'n';
    TX_buffer[1] = 'o';
    TX_buffer[2] = 'w';
  
    //Send 10 packet burst
    for (int i = 0; i < burstNum; ++i) {
      Radio.SendData(TX_buffer, sizetx);
      delay(50);
    }

    digitalWrite(GREEN_LED, LOW);

    sendTime = false;
  }
  
  else if (calibrating) {
    unsigned long getMillis = millis();

    while (millis() - getMillis < 500) {
      theDisplay.displayTime(hours, minutes);
    }
    
    theDisplay.turnOff();
    delay(500);
  }

  else {
    //Essentially just keep getting and displaying the time until you get interrupts to do differently
    theClock.updateTime();
  
    minutes = theClock.getMinute();
    hours = theClock.getHour();
  
    theDisplay.displayTime(hours, minutes);
  
  //  Serial.print(minute);
  //  Serial.print(", ");
  //  Serial.println(hour);
  }
}
