#include <LiquidCrystal.h>

#define interruptA 0
#define interruptB 1
#define CLK 32     // PIN2
#define period  20
#define DAT  51     // PIN3
#define SW 30
#define ledPin 13
#define sensorPin 0
#define REDLED 42
#define BLUELED 44

int COUNT = 0;
int beatsDetected = 0;
int lastValue = 0;
double alpha = 0.75;
double change = 0.0;
int prevRotaryReading = 0;
bool incValue;
bool decValue;
int minValue;
int maxValue;
int thresholdValue;
LiquidCrystal lcd(8,36, 6, 7, 4, 5);
int secondsCounter = 0;
int idx = 0;
int mydata[3] = {0};



void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("Rotary Value: ");
  lcd.setCursor(0, 1);
  lcd.print("Heart Rate: ");
  attachInterrupt(interruptA, RotaryStateChanged, RISING);
  pinMode(CLK, INPUT);
  digitalWrite(CLK, HIGH);
  pinMode(DAT, INPUT);
  digitalWrite(DAT, HIGH);
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);

  pinMode(REDLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);

  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}
void loop() {

  if(!(digitalRead(SW))){
    maxValue = 0;
    minValue = 999;
    beatsDetected = 0;
    secondsCounter = 0;
  }

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
  if(rawValue > maxValue){
    maxValue = rawValue;
  }
  else if(rawValue < minValue){
    minValue = rawValue;
  }
  thresholdValue = ((maxValue - minValue)/4) + minValue;
  int testValue = rawValue;//next(rawValue);
  if((testValue - lastValue) > 3){
    digitalWrite(REDLED, HIGH);
    beatsDetected++;
  }
  else{
    digitalWrite(REDLED, LOW);
  }
  if(rawValue > thresholdValue){
      digitalWrite(BLUELED, HIGH);
      Serial.print("ON    ");
    lastValue = testValue;
  }
  else{
    digitalWrite(BLUELED, LOW);
    Serial.print("OFF   ");
    if(!(lastValue < thresholdValue)){
      //beatsDetected++;
    }
    lastValue = testValue;
  }
  //double value = alpha * oldValue + (1 - alpha) * rawValue;
  Serial.print("Min:");
  Serial.print(minValue);
  Serial.print("  Max: ");
  Serial.print(maxValue);
  Serial.print("  T: ");
  Serial.print(thresholdValue);
  Serial.print("     ");
  Serial.println(rawValue);
  //Serial.print(",");
  //Serial.println(value);
  //oldValue = value;
  if(secondsCounter++ >= 20*15){
    secondsCounter = 0;
    int BPM = beatsDetected*2;

    Serial.print("BEATS PER MINUTE: ");
    Serial.println(BPM);
    lcd.setCursor(12,1);
    lcd.print(BPM);
  }
  delay(50);
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



int next(int current)
{
    mydata[idx++] = current;
    idx %= 3;

    int a = mydata[0];
    int b = mydata[1];
    int c = mydata[2];

    // source: https://www.geeksforgeeks.org/middle-of-three-using-minimum-comparisons/
    if ((a < b && b < c) || (c < b && b < a))
       return b;
 
    // Checking for a
    else if ((b < a && a < c) || (c < a && a < b))
       return a;
 
    else
       return c;    
}