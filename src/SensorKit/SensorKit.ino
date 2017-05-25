/*
 * Author: Tolga Buz
 * Date: May 25, 2017
 * 
A single file that combines a kit of different sensors
Creates a unified console output that can be exported into csv
by copying from the terminal

Sensors used:
DHT_11 (Temperature and Humidity)
T18B20 (Temperature)
Small/Big Sound
SW-420 (Shock)
KY-002 (Shock)

The code is neither complete nor giving precise results.
This is just an example to show how multiple sensors can be combined in a single program.

*/
#include<dht.h>
dht DHT;
#include<TimeLib.h>
#define DHT11_PIN A0
#include <OneWire.h>
OneWire  ds(11);  // on pin 10 (a 4.7K resistor is necessary)
int lightPin = A2;
int value = 0;

//for sound:
int analog_sensor  = A5; // select the input pin for the potentiometer
int digit_sensor = 12; // select the input pin for the potentiometer
int ledPin = 3; // select the pin for the LED
int analogValue ; // value from the analog input pin
int digitValue ;  //  value from the digit input pin

//for shock_sw:
int ledPin2 = 8;
int EP = 9;

//for shock_ky:
int Led = 7;
int Shock = 6;
int val;

struct shock {
  long a;
  int b;
} shock;
typedef struct shock Giver;

struct res {
  long a;
  int b;
  double c;
  int d;
  int e;
} res;
typedef struct res Result;

void setup() {
  pinMode (ledPin, OUTPUT);
  pinMode (digit_sensor, INPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(EP, INPUT); //set EP input for measurment
  Serial.begin(9600);
  pinMode(Led, OUTPUT);
  pinMode(Shock, INPUT);
  Serial.println("'Timestamp';'Humidity';'Temperature1C';'Temperature2C';'Light';'Vibration';'SoundValue'");
  setTime(10, 00, 00, 20, 04, 2017);
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
  Serial.print(";");
  avgloop();
  Serial.println();
  delay(2000 - 24.5 - 1000);
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

void avgloop() {
  Result values; //long - int - double - int
  long shockS1 = 0; //for shock_SW
  int shockS2 = 0; //for shock_SW 2
  boolean shockK1 = LOW; //for shock_KY
  double sound1 = 0.0; //for sound voltage
  int sound2 = 0; //for sound threshold
  int count = 0;
  for ( int i = 0; i < 1500; i++) {
    //shockS1 = shockS1 + shock_SW().a;  //summing up
    //Serial.print("for loop 1 done ");
    //shockS2 = shockS2 + shock_SW().b; 
    shockK1 = shockK1 || shock_KY();
    sound1 = sound1 + sound().c;
    sound2 = sound2 + sound().d;
    count = i;
    //delay(1);
  }  
  //calculating averages
  double avgSound = sound1/count;
  Serial.print(shockK1);
  Serial.print(";");
  Serial.print(avgSound);
  
}

Result sound() {
  analogValue = analogRead (analog_sensor);
  digitValue = digitalRead(digit_sensor);
  Result sound_instance;
  sound_instance.a = 0;
  sound_instance.b = 0;
  //Conversion of the voltage value to dB is not easily possible
  //Voltage strongly depends on how far the sensor's screw has been turned
  //sound_instance.c = (20. * log10 (analogValue / 5.)); 
  sound_instance.c = analogValue;
  if (digitValue == 1) {
    digitalWrite (ledPin, HIGH);
    sound_instance.d = 1;
  }
  else {
    digitalWrite (ledPin, LOW);
    sound_instance.d = 0;
  }
  return sound_instance;
}

Result shock_SW() {
  long measurement = pulseIn (EP, HIGH);
  Result shock_instance;
  shock_instance.a = measurement;
  if (measurement > 1000) {
    digitalWrite(ledPin2, HIGH);
    shock_instance.b = 1;
  }
  else {
    digitalWrite(ledPin2, LOW);
    shock_instance.b = 0;
  }
  return shock_instance;
}

//Left empty because of unreliable performance
//shock_SW does a much better job
boolean shock_KY() { 
  val = digitalRead(Shock);
  if (val == HIGH)
  {
    digitalWrite(Led, HIGH);
  }
  else
  {
    digitalWrite(Led, LOW);
  }
  return val;
}

