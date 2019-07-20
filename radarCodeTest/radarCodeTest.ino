#define RADAR_PIN 3
#define BUZZER_PIN 12
#define LED_PIN 13
bool radarState;

void setup() {
  Serial.begin(9600);
  pinMode(RADAR_PIN, INPUT);
  Serial.println("Radar test");
}

void loop() {
  radarState = digitalRead(RADAR_PIN);
  Serial.println(radarState);
  if(radarState){
    tone(BUZZER_PIN,100);
    digitalWrite(LED_PIN,HIGH);
  }
  else{
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN,LOW);
  }
  delay(50);
}
