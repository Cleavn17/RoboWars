#include <Servo.h>
Servo esc;

void setup() {
  Serial.begin(9600);
  esc.attach(9);                // Yellow signal wire to D9
  esc.writeMicroseconds(1000); // Arm
  Serial.println("Arming ESC...");
  delay(3000);                  // Give ESC time to arm
}

void loop() {
  int throttle = 1500;         // Low throttle
  esc.writeMicroseconds(throttle);
  Serial.print("Throttle: ");
  Serial.println(throttle);
  delay(1000);
}
