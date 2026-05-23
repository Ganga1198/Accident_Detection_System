#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int x = A0;
int fire = 7;
int vibration = 8;

void setup() {
  Serial.begin(9600);
  pinMode(fire, INPUT);
  pinMode(vibration, INPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {

  int xVal = analogRead(x);
  int fireVal = digitalRead(fire);
  int vibrationVal = digitalRead(vibration);

  // send data to NodeMCU
  Serial.print(xVal);
  Serial.print(",");
  Serial.print(fireVal);
  Serial.print(",");
  Serial.println(vibrationVal);

  lcd.clear();
  lcd.setCursor(0,0);

  if (xVal < 330 || xVal > 350) {
    lcd.print("Accident Found");
    Serial.println("Accident Detected");
  }
  else if (fireVal == LOW) {
    lcd.print("Fire Detected");
    Serial.println("Fire Detected");
  }
  else if (vibrationVal == HIGH) {
    lcd.print("Vibration Det");
    Serial.println("Vibration Detected");
  }
  else {
    lcd.print("System Normal");
    Serial.println("System Normal");
  }

  delay(1000);
}