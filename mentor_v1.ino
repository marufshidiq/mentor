#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MServo.h"
#include "MButton.h"
#include "menu.h"
#include "variable.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
MServo* axis = new MServo[numAxis];
MButton btn;

void setup() {
  lcd.begin(16, 2);
  btn.btnNum = 4;
  btn.attach(btnPin, INPUT_PULLUP);
  btn.setState(btnState);
  btn.setActive(LOW);
  for (int n = 0; n < numAxis; n++) {
    axis[n].attach(potPin[n], dirPin[n], pwmPin[n]);
    axis[n].setAngleConstrain(minAngle[n], maxAngle[n]);
    axis[n].setPotConstrain(minPot[n], maxPot[n]);
  }
  axis[BASE].write(100);
  Serial.begin(115200);
}

void loop() {
  mainMenu();
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

void mainMenu() {
  String menu[] = {"Run System", "Calibrate Sensor", "About", "               "};
  lcd.clear();
  int index = 0;
  btn.setIndex(index);
  while (1) {
    index = btn.getIndex(3);
    showMenu(lcd, menu, index);
    if (btn.getBtn() == OK) {
      switch (index) {
        case 0:
          runSystem();
          break;
        case 1:
          calibrateMenu();
          break;
      }
      btn.setIndex(index);
    }
  }
}

void runSystem() {

}

void calibrateMenu() {
  lcd.clear();
  int index = 0;
  btn.setIndex(index);
  while (1) {
    index = btn.getIndex(5);
    showMenu(lcd, nameAxis, index);
    if (btn.getBtn() == OK) {
      calibrateAxis(index);
    }
    else if (btn.getBtn() == CANCEL) {
      break;
    }
  }
}

void calibrateAxis(int num) {
  lcd.clear();
  while (1) {
    lcd.setCursor(0, 0);
    lcd.print("Calibrate ");
    lcd.print(nameAxis[num]);
    lcd_print(0, 1, "<    %4d      >", axis[num].movingAverage());

    if (btn.getBtn() == CANCEL) {
      break;
    }
    else if (btn.getBtn() == UP) {
      axis[num].move(200);
    }
    else if (btn.getBtn() == DOWN) {
      axis[num].move(-200);
    }
    if (!btn.isPressed()) {
      axis[num].move(0);
    }
  }
}

