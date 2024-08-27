#define MQ2pin A0
#define relaypin 8

float sensorValue;  

void setup()
{
  Serial.begin(9600); 
  Serial.println("Gas sensor warming up!");
  delay(20000); 
  pinMode(relaypin, OUTPUT);
  
}

void loop()
{
  sensorValue = analogRead(MQ2pin);
  delay(2000);
  
  
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  
  //Serial.print("    ");
  //Serial.print(digitalRead(8));
  
  if(sensorValue > 300 )
  {
    Serial.print(" | Smoke detected!");
    digitalWrite(relaypin, LOW);
    delay(2000);

  }
  else
  {
    digitalWrite(relaypin, HIGH);
  }
  
  Serial.println("");
  delay(2000); 
}