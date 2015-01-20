int no_cover_led = 8;
int cover_led = 9;

void setup() {
  Serial.begin(9600);
  pinMode(no_cover_led, OUTPUT);
  pinMode(cover_led,OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A0);
  if (sensorValue > 600) {
    digitalWrite(no_cover_led, HIGH);
    digitalWrite(cover_led, LOW);
  } else {
    digitalWrite(cover_led, HIGH);
    digitalWrite(no_cover_led, LOW);
  }
  Serial.println(sensorValue);
  delay(1);
}
