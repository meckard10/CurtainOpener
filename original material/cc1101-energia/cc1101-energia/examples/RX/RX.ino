#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

unsigned long lastrx; 
byte RX_buffer[61]={0};
byte sizerx,i;


 
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
  //polls on the GDO0 pin. Adding delays in loop can cause the example to stop working. 
  if(Radio.CheckReceiveFlag())
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

    //receivedata puts the radio in IDLE mode. Have to put it back into RX mode
    Radio.RxOn();
  } 
}
