#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

unsigned long lastrx; 
unsigned long elapsed; 

#define sizetx 61
byte TX_buffer[sizetx]={0};
byte i;

byte RX_buffer[61]={0};
byte sizerx,flag;

unsigned long previous = 0;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Radio.Init();
  Radio.SetDataRate(4); // Needs to be the same in Tx and Rx
  Radio.SetLogicalChannel(1); // Needs to be the same as receiver
	lastrx=millis();
  Radio.SetTxPower(0);
  Radio.RxOn();
  pinMode(RED_LED, OUTPUT);

  for(i=0;i<sizetx;i++)
  {
     TX_buffer[i]=0;
  }
}



void loop()
{

  if (millis() - previous > 1000)
  {
    
    Radio.SendData(TX_buffer,sizetx);
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    previous = millis();
  }
  
  if(Radio.CheckReceiveFlag())
  {
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    previous = millis();
    //lastrx = millis();
    sizerx=Radio.ReceiveData(RX_buffer);
    // Should be one byte
    //Serial.write(RX_buffer[0]);
    //Serial.println( RX_buffer[0], DEC);
    Radio.RxOn();
  }
}


