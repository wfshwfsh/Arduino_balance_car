/*
	Get All MPU6050 IMU data
*/

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
#define ENCODER_L_B 3
#define ENCODER_R_A 10  // MEGA 用
#define ENCODER_R_B 11  // MEGA 用



void setup_t6612_motors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(ENCODER_L_A, INPUT_PULLUP);
  pinMode(ENCODER_L_B, INPUT_PULLUP);
  pinMode(ENCODER_R_A, INPUT_PULLUP);
  pinMode(ENCODER_R_B, INPUT_PULLUP);

  //digitalWrite(STBY, HIGH); // 啟用馬達模組
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  setup_t6612_motors();
}

void loop() {
  // 正轉
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 200); // 速度：0~255

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 200); // 速度：0~255

  delay(2000); // 轉動 2 秒

  // 停止
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(1000);

  // 反轉
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 200);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 200);

  delay(2000);

  // 停止
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(1000);
}