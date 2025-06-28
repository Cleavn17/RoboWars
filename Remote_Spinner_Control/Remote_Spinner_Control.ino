#include <Servo.h>
Servo esc;

const int ch6_pin = 6;
const int spinner_write_pin = 9;

void setup() {
  // put your setup code here, to run once:
  esc.attach(spinner_write_pin);                // Yellow signal wire to D9
  esc.writeMicroseconds(1000); // Arm
  Serial.println("Arming ESC...");
  delay(3000);                  // Give ESC time to arm
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int spinner_raw = pulseIn(ch6_pin, HIGH); // Read spinner range 950 - 2000
  esc.writeMicroseconds(spinner_raw); // Write spinner value

  delay(20);  // Small delay for stability
}
