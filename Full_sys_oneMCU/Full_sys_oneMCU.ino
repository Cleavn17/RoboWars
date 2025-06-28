//This code controls all peripherals from one microcontroller but is limited by PWM pins

#include <Servo.h>
Servo esc;

// RC Car Control Loop with Independent Motor Control
// We can successfully turn with this code

const int ch3_pin = 3;      // RC input (CH3) - Left Motor (M1) throttle
const int ch4_pin = 9;      // RC input (CH4) - Right Motor (M2) throttle
const int ch6_pin = 6;

// Motor 1 (Left Motor)
const int M1_LPWM = 5;       // IBT-2 Motor 1 direction A
// const int M1_RPWM = 6;       // IBT-2 Motor 1 direction B
const int M1_L_EN = 7;       // IBT-2 Motor 1 enable A
const int M1_R_EN = 8;       // IBT-2 Motor 1 enable B

// Motor 2 (Right Motor)
const int M2_RPWM = 11;      // IBT-2 Motor 2 direction A
const int M2_LPWM = 10;      // IBT-2 Motor 2 direction B
const int M2_R_EN = 13;      // IBT-2 Motor 2 enable A
const int M2_L_EN = 12;      // IBT-2 Motor 2 enable B

// Control parameters
const int DEADZONE = 50;     // Deadzone around center position
const int CENTER_VALUE = 1475; // Center position of joystick (950-2000 range)
const int MIN_THROTTLE = 950;  // Minimum throttle value
const int MAX_THROTTLE = 2000; // Maximum throttle value
const int MAX_SPEED = 255;   // Maximum PWM value

void setup() {
  // Initialize motor pins as outputs
  pinMode(M1_LPWM, OUTPUT);
  // pinMode(M1_RPWM, OUTPUT);
  pinMode(M1_L_EN, OUTPUT);
  pinMode(M1_R_EN, OUTPUT);
  
  pinMode(M2_LPWM, OUTPUT);
  pinMode(M2_RPWM, OUTPUT);
  pinMode(M2_L_EN, OUTPUT);
  pinMode(M2_R_EN, OUTPUT);
  
  // Enable motors
  digitalWrite(M1_L_EN, HIGH);
  digitalWrite(M1_R_EN, HIGH);
  digitalWrite(M2_L_EN, HIGH);
  digitalWrite(M2_R_EN, HIGH);

  esc.attach(8);                // Yellow signal wire to D9
  esc.writeMicroseconds(1000); // Arm
  Serial.println("Arming ESC...");
  delay(3000);                  // Give ESC time to arm
  
  Serial.begin(9600);
}

void loop() {
  // Read joystick values for each motor
  int left_throttle_raw = pulseIn(ch3_pin, HIGH);   // 950-2000 range for M1
  int right_throttle_raw = pulseIn(ch4_pin, HIGH);  // 950-2000 range for M2
  int spinner_raw = pulseIn(ch6_pin, HIGH); // Read spinner range 950 - 2000
  esc.writeMicroseconds(spinner_raw); // Write spinner value
  // Serial.print("Throttle: ");
  // Serial.println(spinner_raw);
  // delay(1000);
  
  // Convert to -525 to +525 range for easier math
  int left_throttle = left_throttle_raw - CENTER_VALUE;   // -525 to +525
  int right_throttle = right_throttle_raw - CENTER_VALUE; // -525 to +525
  
  // Apply deadzone
  if (abs(left_throttle) < DEADZONE) left_throttle = 0;
  if (abs(right_throttle) < DEADZONE) right_throttle = 0;
  
  // Calculate motor speeds and directions
  int left_speed = map(abs(left_throttle), 0, 525, 0, MAX_SPEED);
  int right_speed = map(abs(right_throttle), 0, 525, 0, MAX_SPEED);
  
  // Determine direction for each motor
  bool left_forward = (left_throttle >= 0);
  bool right_forward = (right_throttle >= 0);
  
  // Control motors independently
  controlMotor(1, left_speed, left_forward);   // Left motor (M1)
  controlMotor(2, right_speed, right_forward); // Right motor (M2)
  
  // Debug output
  Serial.print("Left: "); Serial.print(left_throttle);
  Serial.print(" (Speed: "); Serial.print(left_speed);
  Serial.print(", Dir: "); Serial.print(left_forward ? "FWD" : "REV");
  Serial.print(") Right: "); Serial.print(right_throttle);
  Serial.print(" (Speed: "); Serial.print(right_speed);
  Serial.print(", Dir: "); Serial.println(right_forward ? "FWD" : "REV");
  
  delay(20);  // Small delay for stability
}

void controlMotor(int motor, int speed, bool forward) {
  if (motor == 1) {  // Left Motor (M1)
    if (speed == 0) {
      // Stop motor
      analogWrite(M1_LPWM, 0);
      // analogWrite(M1_RPWM, 0);
    } else if (forward) {
      // Forward direction
      analogWrite(M1_LPWM, speed);
      // analogWrite(M1_RPWM, 0);
    } else {
      // Reverse direction
      analogWrite(M1_LPWM, 0);
      // analogWrite(M1_RPWM, speed);
    }
  } else if (motor == 2) {  // Right Motor (M2)
    if (speed == 0) {
      // Stop motor
      analogWrite(M2_LPWM, 0);
      analogWrite(M2_RPWM, 0);
    } else if (forward) {
      // Forward direction
      analogWrite(M2_RPWM, speed);
      analogWrite(M2_LPWM, 0);
    } else {
      // Reverse direction
      analogWrite(M2_RPWM, 0);
      analogWrite(M2_LPWM, speed);
    }
  }
}