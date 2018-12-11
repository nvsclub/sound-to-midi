int soundPin = A0;
int LED = 13;
uint16_t sample = 0;
 
void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}
 
void loop()
{
    sample = analogRead(soundPin);
    Serial.println(sample);

    if(sample > 600){
      digitalWrite(LED, HIGH);
    }else{
      digitalWrite(LED, LOW);
    }

}
