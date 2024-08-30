#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

#define sizetx 10


byte TX_buffer[sizetx]={0};
byte i;
int status = 0;

void setup()
{
  Serial.begin(9600);
  Radio.Init();
  Radio.SetDataRate(4); // Needs to be the same in Tx and Rx
  Radio.SetLogicalChannel(5); // Needs to be the same as receiver
  Radio.SetTxPower(0);
  for(i=0;i<sizetx;i++)
  {
     TX_buffer[i]=0;
  }
  pinMode(RED_LED, OUTPUT);
}
unsigned long burstnum=0;
void loop()
{
  // Send 10 packets, then wait 1 second
  // Put burst number in first data field
  TX_buffer[0] = burstnum++;
  for(int i=0;i<10;i++) {
    TX_buffer[1] = i;
    Radio.SendData(TX_buffer,sizetx);
    delay(50);
  }
  status= !status;
  digitalWrite(RED_LED, status);
  Serial.println("Burst sent.");
  
  delay(1000);
  
}
