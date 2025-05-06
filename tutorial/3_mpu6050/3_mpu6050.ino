/*
	Get All MPU6050 IMU data
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup_mpu6050() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 連接成功！");
  } else {
    Serial.println("MPU6050 連接失敗！");
  }
}

void setup() {
  setup_mpu6050();
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("aX="); Serial.print(ax);
  Serial.print("\t | aY="); Serial.print(ay);
  Serial.print("\t | aZ="); Serial.print(az);
  Serial.print("\t || gX="); Serial.print(gx);
  Serial.print("\t | gY="); Serial.print(gy);
  Serial.print("\t | gZ="); Serial.println(gz);

  delay(500); // 每 500 毫秒更新一次
}