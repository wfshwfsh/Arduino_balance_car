#include <Wire.h>

// 定義腳位
const int AIN1 = 5;
const int AIN2 = 4;
const int PWMA = 6;
const int BIN1 = 7;
const int BIN2 = 8;
const int PWMB = 9;

// 編碼器
#define ENCODER_L_A 2
#define ENCODER_L_B 10
#define ENCODER_R_A 3  // MEGA 用
#define ENCODER_R_B 11  // MEGA 用

volatile long leftCount = 0, rightCount = 0;



// 讀取編碼器
void leftEncoderISR() {
  //Serial.println("leftEncoderISR");
  if (digitalRead(ENCODER_L_A) == digitalRead(ENCODER_L_B)) {
    leftCount++;
  } else {
    leftCount--;
  }
}

void rightEncoderISR() {
  //Serial.println("rightEncoderISR");
  if (digitalRead(ENCODER_R_A) == digitalRead(ENCODER_R_B)) {
    rightCount++;
  } else {
    rightCount--;
  }
}

void setup_t6612_motors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(ENCODER_L_A, INPUT);
  pinMode(ENCODER_L_B, INPUT);
  pinMode(ENCODER_R_A, INPUT);
  //pinMode(ENCODER_R_B, INPUT_PULLUP);

  //digitalWrite(STBY, HIGH); // 啟用馬達模組

  attachInterrupt(digitalPinToInterrupt(ENCODER_R_A), rightEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_L_A), leftEncoderISR, CHANGE);
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  int motorSpeed_L = constrain(abs(leftSpeed), 0, PWM_Lim);
  int motorSpeed_R = constrain(abs(rightSpeed), 0, PWM_Lim);

  if (leftSpeed > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  analogWrite(PWMA, motorSpeed_L);

  if (rightSpeed > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  analogWrite(PWMB, motorSpeed_R);
}

float Get_Speed_L(void)
{
  static long lastLeftCount = 0;
  float _speed = leftCount - lastLeftCount;
  lastLeftCount = leftCount;
  //Serial.print("leftCount="); Serial.print(leftCount);
  return _speed;
}

float Get_Speed_R(void)
{
  static long lastRightCount = 0;
  float _speed = rightCount - lastRightCount;
  lastRightCount = rightCount;
  //Serial.print("rightCount="); Serial.print(rightCount);
  
  /* Take the negative value to make both sides move in the same direction. */
  return -_speed;
}