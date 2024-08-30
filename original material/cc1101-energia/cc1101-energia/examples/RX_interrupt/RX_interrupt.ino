#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

unsigned long lastrx; 
byte RX_buffer[61]={0};
byte sizerx,i;


volatile byte gdo0_flag = 0;

void radio_isr(void) {
  gdo0_flag = 1;
}
 
void setup()
{
  Serial.begin(9600);
  Serial.println("start");
  Radio.Init();
  Radio.SetDataRate(4); // Needs to be the same in Tx and Rx
  Radio.SetLogicalChannel(5); // Needs to be the same as receiver
  Radio.SetTxPower(1);
	lastrx=millis();
  Radio.RxOn();
  
  attachInterrupt(P3_4, radio_isr, FALLING);  
}

void printMsg(byte *buffer, uint16_t size) {
  for (int i=0; i < size; i++) {
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void loop()
{
  LPM3;  //zzzzzzzz going to sleep
  
  
  if(gdo0_flag)
  {
    lastrx = millis();
    Serial.print("check = ");
    Serial.println(lastrx);
    sizerx=Radio.ReceiveData(RX_buffer);
    // Should be one byte
    //Serial.write(RX_buffer[0]);
    Serial.print(sizerx);
    Serial.println(" bytes!");
    printMsg(RX_buffer, sizerx);

    gdo0_flag=0;
    //receivedata puts the radio in IDLE mode. Have to put it back into RX mode
    Radio.RxOn();
    
  } 
}
