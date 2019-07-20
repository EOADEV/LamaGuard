#define RADAR_PIN 3

void setup() {
  Serial.begin(9600);
  pinMode(RADAR_PIN, INPUT);
}

void loop() {
  Serial.println(digitalRead(RADAR_PIN));
  delay(50);
}
