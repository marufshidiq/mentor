#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "MServo.h"
#include "variable.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
MServo* axis = new MServo[numAxis];

void setup() {
  lcd.begin(16,2);
  for (int n = 0; n < numAxis; n++) {
    axis[n].attach(potPin[n], dirPin[n], pwmPin[n]);
    axis[n].setAngleConstrain(minAngle[n], maxAngle[n]);
    axis[n].setPotConstrain(minPot[n], maxPot[n]);
  }  
  axis[BASE].write(100);
  Serial.begin(115200);
}

void loop() {
  Serial.println(axis[BASE].movingAverage());
  axis[BASE].update();
  if (Serial.available() > 0) {
    if (Serial.find("m")) {
      int target = Serial.parseInt();
      axis[BASE].write(target);
    }
  }
}

void lcd_print(uint8_t x, uint8_t y, const char* fmtstr, ...)
{
  char lcd_string[21];
  va_list ap;

  va_start(ap, fmtstr);
  vsprintf(lcd_string, fmtstr, ap);
  va_end(ap);

  lcd.setCursor(x, y);
  lcd.print(lcd_string);
}
