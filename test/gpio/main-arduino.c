#include <Arduino.h>

void setup() {
  pinMode(PA0, INPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PC13, OUTPUT);

  digitalWrite(PA1, HIGH);
  digitalWrite(PC13, HIGH);
}

void loop() {
  if (digitalRead(PA0) == HIGH) {
    digitalWrite(PA2, HIGH);
  }
  else {
    digitalWrite(PA2, LOW);
  }
}
