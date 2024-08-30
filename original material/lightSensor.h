#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "minunit.h"

#define sensor A12
#define LIGHTLOOP 100

//Set the min initally to the max value
uint16_t minLightVal = 0xFFFF;
uint16_t lightAvg[LIGHTLOOP] = {0};
uint16_t counter = 0;

void setupLightSensor(){
  for(int i = 0; i < LIGHTLOOP-1; i++){
    lightAvg[i] = 0;
  }
  minLightVal = analogRead(sensor) *1.5;
}

void setMinLightVal(uint16_t num){
  minLightVal = num;
}

bool isLightBright(uint16_t num){
  return (num > minLightVal);
}

int computeLightAvg(){
  uint32_t sum = 0;
  for(int i = 0; i < LIGHTLOOP-1; i++){
    sum += lightAvg[i];
  }
  return sum/LIGHTLOOP;
}

uint16_t readLightOnce(){
  lightAvg[counter] = analogRead(sensor);
  counter = (counter+1)%LIGHTLOOP;
  return analogRead(sensor);
}

// THIS IS ONLY USED FOR TESTING
void addToLightAvg(int x){
  lightAvg[counter] = x;
  counter = (counter+1)%LIGHTLOOP;
}

void waitAndReadLight(){
  for(int i = 0; i < (LIGHTLOOP*2); i++){
    lightAvg[counter] = analogRead(sensor);
    counter = (counter+1)%LIGHTLOOP;
    delay(10);
  }
}

static char* testLightComputeAvg(){
  for (int i = 0; i < LIGHTLOOP; ++i) {
    lightAvg[i] = 0;
  }
  
  addToLightAvg(100);
  mu_assert("average should be 1", computeLightAvg() == 1);
  return NULL;
}

//Test that the light level is changing over time, even slightly
static char* testLightChange() {
  readLightOnce();
  uint16_t prev = lightAvg[counter-1];
  
  delay(100);
  
  readLightOnce();
  uint16_t newVal = lightAvg[counter-1];
  
  mu_assert("light level should be changed between time", newVal != prev);
  return NULL;
}

//Test that the light level average changes over time too
static char* testLightAvgChange() {
  waitAndReadLight();
  int prevAvg = computeLightAvg();
  waitAndReadLight();
  int newAvg = computeLightAvg();

  mu_assert("average light level should have changed over time", prevAvg != newAvg);
  return NULL;
}

//Test to make sure that the min and everything is setup correct
static char* testSetup() {
  mu_assert("the counter should be 0'd out", counter == 0);

  int sum = 0;

  for (int i = 0; i < LIGHTLOOP; ++i) {
    sum += lightAvg[i];
  }
  
  mu_assert("the lightAvg array should all be 0", sum == 0);
  mu_assert("average of an empty array should be 0", computeLightAvg() == 0);

  waitAndReadLight();  
  
  mu_assert("the average value should be less than the minimum before it is set", computeLightAvg() < minLightVal);
  return NULL;
}

static char* allLightSensorTests(){
  mu_run_test(testSetup);
  mu_run_test(testLightComputeAvg);
  mu_run_test(testLightChange);
  mu_run_test(testLightAvgChange);
  return NULL;
}

#endif
