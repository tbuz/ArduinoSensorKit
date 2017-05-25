/* DS18S20 Temperature Sensor Module sample project
   project tutorial url: http://osoyoo.com/?p=409
*/
#include <OneWire.h>
int inPin=2; // define D10 as input pin connecting to DS18S20 S pin
OneWire ds(inPin); 
 
void setup(void) {
  Serial.begin(9600);
}
 
void loop(void) {
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
 
  if ( !ds.search(addr)) {
      ds.reset_search();
      return;
  }
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); 
 
  delay(1000);  
 
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);  
 
  for ( i = 0; i < 9; i++) { 
    data[i] = ds.read();
  }
  Serial.print("Temperature: ");
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  
  if (SignBit)
  {
    TReading = (TReading ^ 0xffff) + 1;
  }
  Tc_100 = (6 * TReading) + TReading / 4; 
  Whole = Tc_100 / 100; 
  Fract = Tc_100 % 100;
  if (SignBit)
  {
     Serial.print("-");
  }
  Serial.print(Whole);
  Serial.print(".");
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print(Fract);
  Serial.print(" C\n");
}
