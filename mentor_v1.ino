#include "MServo.h"

unsigned long previousMillis = 0;
int arah = 200;

MServo m0;
MServo m1;

void setup() {
  m0.attach(A5, 6, 7);
  m1.attach(A3, 9, 8);
  m1.setAngleConstrain(0, 180);
  m1.setPotConstrain(10, 935);
  m0.setAngleConstrain(0, 210);
  m0.setPotConstrain(47, 662);
  m0.setTolerance(2);
  m1.setTolerance(4);
  m0.write(100);
  Serial.begin(115200);
}

void loop() {
  m1.update();
  if (Serial.available() > 0) {
    if (Serial.find("m")) {
      int target = Serial.parseInt();
      m1.write(target);
    }
  }
}
