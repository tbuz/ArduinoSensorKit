// Watch video here: https://www.youtube.com/watch?v=235BLk7vk00

/* Vibration sensor connected to Arduino pins as follows:
 Arduino            Vibration Sensor
   D9                    DOut
   GND                   GND
   +5V                   VCC     
   
   D8                Indication LED  
   
   Sensor SW-420 
*/

int ledPin = 8;
int EP =9;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(EP, INPUT); //set EP input for measurment
  Serial.begin(9600); //init serial 9600
 // Serial.println("----------------------Vibration demo------------------------");
}
void loop(){
  long start = millis();
  long measurement =TP_init();
  long endm = millis();
  delay(9.9999-(endm-start));
  Serial.print(millis());
  Serial.print(" - ");
  Serial.println(measurement);
  if (measurement > 1000){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW); 
  }
}

long TP_init(){
  //delay(10);
  long measurement=pulseIn(EP, HIGH, 10000);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}

