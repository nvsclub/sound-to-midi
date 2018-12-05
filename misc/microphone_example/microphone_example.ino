uint16_t soundPin = A0;
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
    Serial.println(analogRead(soundPin));

}
