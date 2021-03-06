class MServo {
  public:
    MServo();
    void attach(int pot, int dir, int pwm);
    int getPot();
    int movingAverage();
    void move(int val);
    int getAngle();
    int getTarget();
    void resetTarget();
    void setAngleConstrain(int min, int max);
    void setPotConstrain(int min, int max);
    void setTolerance(int tolerance);
    void write(int target);
    void update();
    bool reverse = false;
  private:
    int _pot, _dir, _pwm;

    // Moving Average
    int _numRead = 10; // Default 10
    int _readings[10]; // Default 10
    int _total = 0;
    int _readIndex = 0;
    int _minPot = 0;
    int _maxPot = 1023;
    int _minAngle = 0;
    int _maxAngle = 360;
    int _target = 0;
    int _tolerance = 6;
    int _baseSpeed = 200;
};

// Constructor
MServo::MServo() {}

void MServo::attach(int pot, int dir, int pwm) {
  _pot = pot;
  _dir = dir;
  _pwm = pwm;
  pinMode(_pot, INPUT);
  pinMode(_dir, OUTPUT);
  pinMode(_pwm, OUTPUT);  

  // Ubah semua nilai awal menjadi 0
  for (int thisReading = 0; thisReading < _numRead; thisReading++) {
    _readings[thisReading] = 0;
  }
}

int MServo::getPot() {
  return analogRead(_pot);
}

int MServo::movingAverage() {
  int result;
  _total -= _readings[_readIndex];
  _readings[_readIndex] = getPot();
  _total += _readings[_readIndex];
  _readIndex += 1;
  if (_readIndex >= _numRead) {
    _readIndex = 0;
  }
  result = _total / _numRead;
  return result;
}

void MServo::move(int val) {
  if(reverse){
    val *=-1;
  }
  if (val >= 0) {
    digitalWrite(_dir, 0);
    analogWrite(_pwm, val);
  } else if (val < 0) {
    digitalWrite(_dir, 1);
    analogWrite(_pwm, 255 + val);
  }
}

int MServo::getAngle() {
  int angle = map(movingAverage(), _minPot, _maxPot, _minAngle, _maxAngle);
  return constrain(angle, _minAngle, _maxAngle);
}

int MServo::getTarget(){
  return _target;
}

void MServo::resetTarget(){  
  _target = getAngle();
}

void MServo::setAngleConstrain(int min, int max) {
  _minAngle = min;
  _maxAngle = max;
}

void MServo::setPotConstrain(int min, int max) {
  _minPot = min;
  _maxPot = max;
}

void MServo::setTolerance(int tolerance) {
  if (tolerance < 0) {
    tolerance *= -1;
  }
  _tolerance = tolerance;
}

void MServo::write(int target) {
  _target = target;
}

void MServo::update() {
  int target = _target;
  int feedback = getAngle();
  int diff = feedback - target;
  int tolerance = _tolerance / 2;
  if ((diff < tolerance) && (diff > -tolerance)) {
    move(0);
  }
  else {
    if (diff > tolerance) {
      move(_baseSpeed);
    }
    else if (diff < -tolerance) {
      move(-1 * _baseSpeed);
    }
  }
}
