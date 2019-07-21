#include <SoftwareSerial.h>
SoftwareSerial ESP32(2,3);

void setup() {
  ESP32.begin(115200);
  ESP32.println("Hi...1");
  ESP32.println("Hi...2");
  ESP32.println("Hi...3");
  ESP32.println("Hi...4");
}

void loop() {
}
