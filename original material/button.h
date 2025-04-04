#ifndef BUTTON_H
#define BUTTON_H

#include "minunit.h"
#include "clock.h"

#define minute P5_3
#define hour P3_7
#define snooze P3_5
#define calibrate P8_1

//State machine for calibration and radio sending
bool calibrating = false;

bool sendTime = false;

//We store the hours and minutes for the overall system here
uint8_t hours = 0;
uint8_t minutes = 0;

//Class to setup the segment display
SegmentDisplay theDisplay;

//Class to setup the clock
Clock theClock;

//Button ISRs
void snoozeISR() {
  //If we already are calibrating and receive this interupt, stop calibrating
  if (calibrating) {
    calibrating = false;
    return;
  }

  //Calibrate the clock now
  calibrating = true;

  hours = theClock.getHour();
  minutes = theClock.getMinute();
  
  Serial.println("snooze?");
}

//Send the calibration signal to the window
void calibrateISR(){
  sendTime = true;
  
  Serial.println("calibrate?");
}

void minuteISR() {
  if (calibrating) {
    minutes += 1;
    theClock.setMinute(minutes);
  }
  
  Serial.println("minute?");
}

void hourISR() {
  if (calibrating) {
    hours += 1;
    theClock.setHour(hours);
  }
  
  Serial.println("hour?");
}

//Setup function to initiate pins and the radio
void setupButtons() {
  //Setup the buttons
  pinMode(minute, INPUT);
  pinMode(hour, INPUT);
  pinMode(snooze, INPUT);
  pinMode(calibrate, INPUT);

  attachInterrupt(digitalPinToInterrupt(snooze), snoozeISR, RISING);
  attachInterrupt(digitalPinToInterrupt(calibrate), calibrateISR, RISING);
  attachInterrupt(digitalPinToInterrupt(minute), minuteISR, RISING);
  attachInterrupt(digitalPinToInterrupt(hour), hourISR, RISING);
}

//Test that each button is currently low, currently only two implemented
static char* testButtonsLow() {
  mu_assert("calibrate button should be low!", digitalRead(calibrate) == LOW);
  mu_assert("snooze button should be low!", digitalRead(snooze) == LOW);
  return NULL;
}

static char* allButtonTests() {
  mu_run_test(testButtonsLow);
  return NULL;
}

#endif
