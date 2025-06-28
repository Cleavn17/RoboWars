const int channel = 3;      // RX input
unsigned long pulseWidth;
void setup() {
  Serial.begin(9600);
  pinMode(channel, INPUT);
}

void loop() {
  pulseWidth = pulseIn(channel, HIGH, 25000);  // Timeout after 25ms
  Serial.print("Read Value: ");
  Serial.println(pulseWidth);

  delay(50);
}
