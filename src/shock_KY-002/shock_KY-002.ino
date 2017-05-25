//Sensor KY-002

int Led = 7;
int Shock = 6;
int val;
void setup()
{
  Serial.begin(9600);
  pinMode(Led, OUTPUT);
  pinMode(Shock, INPUT);
}
void loop()
{
  val = digitalRead(Shock);
  if (val == HIGH)
  {
    digitalWrite(Led, HIGH);
    Serial.println("High");
  }
  else
  {
    digitalWrite(Led, LOW);
    Serial.println("Low");
  }
  delay(100);
}

