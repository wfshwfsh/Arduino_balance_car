#include <MPU6050.h>

#define FUSION_RATE_ANGLE 0.98

MPU6050 mpu;
int16_t ax, ay, az, _gx, _gy, _gz;
int16_t gx, gy, gz;
float curAngle=0;
float angleOffset=0;
extern volatile int dt_intr_ms;

void setup_mpu6050() {
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 Connect Failed");
  }
}

void get_mpu6050_raw_data()
{
  mpu.getMotion6(&ax, &ay, &az, &_gx, &_gy, &_gz);
}

void show_mpu6050_raw_data()
{
  Serial.print("aX="); Serial.print(ax);
  Serial.print("\t | aY="); Serial.print(ay);
  Serial.print("\t | aZ="); Serial.print(az);
  Serial.print("\t || _gX="); Serial.print(_gx);
  Serial.print("\t | _gY="); Serial.print(_gy);
  Serial.print("\t | _gZ="); Serial.println(_gz);
}

void angle_calculate()
{
  float accAngle = -atan2(ax, az) * 180 / PI - angleOffset;
  gy = _gy / 131.0;

  curAngle = FUSION_RATE_ANGLE * (curAngle + gy * dt_intr_ms/1000) + (1-FUSION_RATE_ANGLE) * accAngle;
}

void show_angle()
{
  Serial.print("curAngle="); Serial.println(curAngle);
}