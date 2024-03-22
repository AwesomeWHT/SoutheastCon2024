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

bool last_threshold = 0;


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
  
  int ema = EMA(rawValue);
  
  if (rawValue >= ema) {
      digitalWrite(REDLED, HIGH);
	  if (!last_threshold) beatsDetected++;
	  last_threshold = 1;
  } else {
      digitalWrite(REDLED, HIGH);
	  last_threshold = 0;
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

int old_ema = 700;
int EMA(int value) {
    return value / 10 + old_ema * 9 / 10;
}
