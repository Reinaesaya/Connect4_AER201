void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue1 = analogRead(A8);
  int sensorValue2 = analogRead(A9);
  Serial.print(sensorValue1);
  Serial.print(" ");
  Serial.println(sensorValue2);
  delay(1);
}
