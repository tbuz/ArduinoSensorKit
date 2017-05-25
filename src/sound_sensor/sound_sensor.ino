/* sound sensor test project
 * sensor project tutorial url: http://osoyoo.com/?p=508
 * 
 * 
 */
int analog_sensor  = A5; // select the input pin for the potentiometer
int digit_sensor = 12; // select the input pin for the potentiometer
int ledPin = 3; // select the pin for the LED
int analogValue ; // value from the analog input pin
int digitValue ;  //  value from the digit input pin
void setup () {
pinMode (ledPin, OUTPUT);
pinMode (digit_sensor, INPUT);
Serial.begin (9600);
}
void loop () {
analogValue = analogRead (analog_sensor);
digitValue = digitalRead(digit_sensor);
//Serial.print("Analog: ");
Serial.print(analogValue);
double db = (20. * log10 (analogValue /5.));
Serial.print(" - ");
Serial.println(db);
if (digitValue==1) {
  digitalWrite (ledPin, HIGH);
}
else digitalWrite (ledPin, LOW);
//Serial.print("Digital: ");
//Serial.println(digitValue);
delay(1000);
}
