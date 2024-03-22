#include <LiquidCrystal.h>

LiquidCrystal lcd(8,36, 6, 7, 4, 5);
const int interruptA = 0;
int CLK = 32;     // PIN2
int DAT = 51;     // PIN3
int COUNT = 0;
int SW = 30;
int ledPin = 13;
int sensorPin = 0;
double alpha = 0.75;
int period = 20;
double change = 0.0;
int prevRotaryReading = 0;
bool incValue;
bool decValue;
void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("Rotary Value: ");
  lcd.setCursor(0, 1);
  lcd.print("               ");
  attachInterrupt(interruptA, RotaryStateChanged, RISING);
  pinMode(CLK, INPUT);
  digitalWrite(CLK, HIGH);  // Pull High Resistance
  pinMode(DAT, INPUT);
  digitalWrite(DAT, HIGH);  // Pull High Resistance
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  if(incValue){
    incValue = 0;
    COUNT++;
  }
  if(decValue){
    decValue = 0;
    COUNT--;
  }
  if(COUNT != prevRotaryReading){
    prevRotaryReading = COUNT;
    if(COUNT >= 100){
      COUNT = 0;
    }
    lcd.setCursor(14, 0);
    lcd.print(COUNT);
    Serial.println(COUNT);
  }
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead(sensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  //Serial.print(rawValue);
  //Serial.print(",");
  //Serial.println(value);
  oldValue = value;
  //delay(period);
}


void RotaryStateChanged()  //When CLK FALLING READ DAT
{
  if (digitalRead(DAT))  // When DAT = HIGH IS FORWARD
  {
    incValue = 1;
    //Serial.println(COUNT);
    delay(20);
  } else  // When DAT = LOW IS BackRote
  {
    decValue = 1;
    //Serial.println(COUNT);
    delay(20);
  }
}
