#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// Pins
const int tempPin   = A0;
const int greenLED  = 2;
const int yellowLED = 3;
const int redLED    = 4;
const int buzzerPin = 5;
const int fanPin    = 6;   // PWM pin for transistor base control

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzerPin, LOW);
  analogWrite(fanPin, 0);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temp Control");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {
  // Read LM35DZ
  int sensorValue = analogRead(tempPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = voltage * 100.0;   // LM35DZ formula

  // Serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // LCD line 1
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print((char)223);   // degree symbol
  lcd.print("C   ");

  // Control logic
  if (temperature < 30.0) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzerPin, LOW);
    analogWrite(fanPin, 0);

    lcd.setCursor(0, 1);
    lcd.print("Status: SAFE   ");
  }
  else if (temperature >= 30.0 && temperature < 40.0) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzerPin, LOW);
    analogWrite(fanPin, 150);   // medium speed

    lcd.setCursor(0, 1);
    lcd.print("Status: WARN   ");
  }
  else {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzerPin, HIGH);
    analogWrite(fanPin, 255);   // full speed

    lcd.setCursor(0, 1);
    lcd.print("Status: DANGER ");
  }

  delay(500);
}
