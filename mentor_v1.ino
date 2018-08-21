#include "MServo.h"
#include "variable.h"

MServo* axis = new MServo[numAxis];

void setup() {
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
