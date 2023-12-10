#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"

// Pins
#define echoPin  2
#define trigPin  3
#define resetPin 4
const int ResetButtonPin = 6;
const int SETButtonPin = 9;
const int Target30ButtonPin = 10;
const int Target25ButtonPin = 11;
const int Target20ButtonPin = 12;

// LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Variables
float duration; // Microseconds
float cm;
//long numDisplay = 100;
boolean trigUp = false;
boolean trigDown = false;
float counterPushUp = 0;
int buzzer = 7;
int target = 35;
unsigned long startTime;
unsigned long elapsedTime;
const unsigned long requiredTime = 120000; // 2 minutes in milliseconds
int flagbit = 0;
int ResetButtonState = 0;
int SETButtonState = 0;
int Target30ButtonState = 0;
int Target25ButtonState = 0;
int Target20ButtonState = 0;

void setup() {
  // Setup code
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(ResetButtonPin, INPUT_PULLUP);
  pinMode(SETButtonPin, INPUT_PULLUP);
  pinMode(Target30ButtonPin, INPUT_PULLUP);
  pinMode(Target25ButtonPin, INPUT_PULLUP);
  pinMode(Target20ButtonPin, INPUT_PULLUP);

  lcd.begin();
  delay(500);
  lcd.print("PushUp Count: ");
  startTime = millis(); // Start the timer
}

void loop() {
  // Print elapsed time
  lcd.setCursor(10, 1);
  lcd.print("00:00");
  lcd.setCursor(0, 1);
  lcd.print(target);
  if (flagbit == 1) {
    if (millis() - startTime <= requiredTime) {
      unsigned long currentTime = millis(); // Get the current time
      elapsedTime = currentTime - startTime; // Calculate the elapsed time
      unsigned long seconds = elapsedTime / 1000; // Convert milliseconds to seconds
      unsigned long minutes = seconds / 60; // Convert seconds to minutes

      lcd.setCursor(11, 1);
      lcd.print(minutes);
      lcd.print(":");
      if (seconds % 60 < 10) {
        lcd.print("0");
      }
      lcd.print(seconds % 60);

      
    }else{
      if (counterPushUp < target){
          tone(7, 900, 100);
          delay(150);
          tone(7, 100, 500);
          lcd.setCursor(0, 1);
          lcd.print("FAILED");
          delay(5000);
           counterPushUp = 0;
          startTime = millis();
          flagbit = 0;
          elapsedTime = 0;
          lcd.setCursor(0, 1);
          lcd.print("            ");
          lcd.setCursor(13, 0);
          lcd.print("            ");
      }
    }
  }

  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  duration = pulseIn(echoPin, HIGH);
  cm = duration * 0.034 / 2.;

  // Detect push-up
  if (cm > 15 && cm <= 30) {
    trigUp = true;
  }
  else if (cm < 10) {
    trigDown = true;
  }
  else if (cm > 30) {
  }

  if (trigUp == true && trigDown == true) {
    counterPushUp += 0.5;
    trigUp = false;
    delay(500);
    tone(7, 800, 40);
    trigDown = false;
  }

  // Reset button
  ResetButtonState = digitalRead(ResetButtonPin);
  if (ResetButtonState == LOW) {
    
    flagbit = 0;
    elapsedTime = 0;
    counterPushUp = 0;
    tone(7, 366, 500);
    lcd.setCursor(0, 1);
      lcd.print("            ");
      lcd.setCursor(13, 0);
      lcd.print("            ");
  }

  // Set button
  SETButtonState = digitalRead(SETButtonPin);
  if (SETButtonState == LOW) {
    
    flagbit = 1;
    startTime = millis();
    tone(7, 166, 300);
    Serial.print("SET");
    delay(1000);
  }
  // Target buttons
  Target30ButtonState = digitalRead(Target30ButtonPin);
  if (Target30ButtonState == LOW) {
    target = 35;
    lcd.setCursor(0, 1);
    lcd.print(target);
    tone(7, 66, 200);
    Serial.print(target);
    delay(1000);
  }

  Target25ButtonState = digitalRead(Target25ButtonPin);
  if (Target25ButtonState == LOW) {
    target = 25;
    lcd.setCursor(0, 1);
    lcd.print(target);
    tone(7, 66, 200);
    Serial.print(target);
    delay(1000);
  }

  Target20ButtonState = digitalRead(Target20ButtonPin);
  if (Target20ButtonState == LOW) {
    target = 20;
    lcd.setCursor(0, 1);
    lcd.print(target);
    tone(7, 66, 200);
    Serial.print(target);
    delay(1000);
  }

  // Display count
  int count = floor(counterPushUp);
  lcd.setCursor(14, 0);
  lcd.print(count);

  // Check if target is reached
  if (counterPushUp == target) {
    
    if ((elapsedTime + 100) <= requiredTime) {
      tone(7, 966, 1000);
      delay(100);
      tone(7, 266, 500);
      delay(100);
      tone(7, 577, 500);
      delay(100);
      tone(7, 966, 1000);
      delay(100);
      lcd.setCursor(0, 1);
      lcd.print("PASSED!!");
      delay(5000);
    } else {
      tone(7, 261, 1000);
      delay(100);
      tone(7, 277, 1000);
      lcd.setCursor(0, 1);
      lcd.print("FAILED..:(");
      delay(5000);
    }

    counterPushUp = 0;
    startTime = millis();
    flagbit = 0;
    elapsedTime = 0;
    lcd.setCursor(0, 1);
    lcd.print("            ");
    lcd.setCursor(13, 0);
    lcd.print("            ");
  }
}


