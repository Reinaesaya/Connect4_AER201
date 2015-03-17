int lightPin1 = 0;
int lightPin2 = 1;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
   Serial.print(analogRead(lightPin1));
   Serial.print(" ");
   Serial.println(analogRead(lightPin2));
   delay(10);
}
