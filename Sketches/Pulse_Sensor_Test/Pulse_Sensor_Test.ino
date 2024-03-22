const int interruptA = 0;
int CLK = 2;     // PIN2
int DAT = 3;     // PIN3
int BUTTON = 4;  // PIN4
int COUNT = 0;
int ledPin = 13;
int sensorPin = 0;
double alpha = 0.75;
int period = 2000;
double change = 0.0;
void setup() {
  attachInterrupt(interruptA, RoteStateChanged, FALLING);
  pinMode(CLK, INPUT);
  digitalWrite(2, HIGH);  // Pull High Resistance
  pinMode(DAT, INPUT);
  digitalWrite(3, HIGH);  // Pull High Resistance
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead(sensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  Serial.print(rawValue);
  Serial.print(",");
  Serial.println(value);
  oldValue = value;
  delay(period);
}