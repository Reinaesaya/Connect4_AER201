//int lightPin1 = A5;
int lightPin2 = A0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
   //Serial.print(analogRead(lightPin1));
   //Serial.print(" ");
   Serial.println(analogRead(lightPin2));
   delay(10);
}
