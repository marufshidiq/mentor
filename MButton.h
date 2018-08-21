class MButton {
  public:
    MButton();
    int btnNum = 1;
    void attach(int pin[], int mode = 1);
    void setState(char state[]);
    void setActive(bool active);
    char getBtn();
    bool isPressed();
  private:
    int *_pin;
    char *_state;
    bool _active = LOW;
    char _standBy = ' ';
};

MButton::MButton() {}

void MButton::attach(int *pin, int mode = 1) {
  for (int i = 0; i < btnNum; i++) {
    pinMode(pin[i], mode);
  }
  _pin = pin;
}

void MButton::setState(char *state) {
  _state = state;
}

void MButton::setActive(bool active) {
  _active = active;
}

char MButton::getBtn() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(_pin[i]) == _active) {
      return _state[i];
    }
  }
  return _standBy;
}

bool MButton::isPressed() {
  if (getBtn() != _standBy) {
    return true;
  } else {
    return false;
  }
}

