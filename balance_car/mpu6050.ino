#include <MPU6050.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;

void setup_mpu6050() {
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 Connect Failed");
  }
}

void get_mpu6050_raw_data()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}

void show_mpu6050_raw_data()
{
  Serial.print("aX="); Serial.print(ax);
  Serial.print("\t | aY="); Serial.print(ay);
  Serial.print("\t | aZ="); Serial.print(az);
  Serial.print("\t || gX="); Serial.print(gx);
  Serial.print("\t | gY="); Serial.print(gy);
  Serial.print("\t | gZ="); Serial.println(gz);
}
