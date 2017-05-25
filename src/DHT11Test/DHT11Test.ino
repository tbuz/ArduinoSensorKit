#include<dht.h>
dht DHT;
#include<TimeLib.h>
#define DHT11_PIN A0
#include <OneWire.h>
OneWire  ds(11);  // on pin 10 (a 4.7K resistor is necessary)
int lightPin = A2; 
int value = 0; 

void setup() {
  Serial.begin(9600);
  //Serial.println("Welcome Osoyoo DHT11 Temperature/Humidity Sample Project"); 
  Serial.println("'Timestamp';'Humidity';'Temperature1C';'Temperature2C';'Light'"); 
  setTime(10, 00, 00, 28, 03, 2017); 
}

void loop() { // READ DATA
  Serial.print(hour());
  Serial.print(":");
  if (minute() < 10) {
    Serial.print("0");
  }
  Serial.print(minute());
  Serial.print(":");
  if (second() < 10) {
    Serial.print("0");
  }
  Serial.print(second());
  Serial.print(";");
  DHT11();
  Serial.print(";");
  T18B20();
  T18B20();
  Serial.print(";");
  light();
  Serial.println();
  delay(2000-24.5);
}

void DHT11() {
  int chk = DHT.read11(DHT11_PIN);
  Serial.print(DHT.humidity, 1);
  Serial.print(";");
  Serial.print(DHT.temperature, 1);
}

void T18B20() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    return;
  }

  type_s = 0;

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print(celsius);
}

void light() {
 value = analogRead(lightPin); 
 Serial.print(value, DEC);
}

