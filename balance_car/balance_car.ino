#include <Wire.h>

#define LED_BUILTIN 13
volatile bool ledState = false;
volatile int timer2IsrCounter = 0;
extern int16_t ax, ay, az, gx, gy, gz;


// Timer1 中斷函式（每秒觸發一次）
ISR(TIMER2_COMPA_vect) {
  timer2IsrCounter++;
  if (timer2IsrCounter >= 10){
    // 切換 LED 狀態
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);

    timer2IsrCounter=0;
  }
}


void setup() {
  // Init Serial
  Serial.begin(115200);
  Wire.begin();

  // Init MPU6050
  setup_mpu6050();

  // Init Motors
  setup_t6612_motors();

  // Init Timer ISR
  timer_isr_setup();
}

void loop() {
  
  // Read MPU6050 Data
  get_mpu6050_raw_data();
  show_mpu6050_raw_data();

  // Controll motors
  setMotorSpeed(/*Left*/100,/*Right*/100);
  delay(3000); 

  setMotorSpeed(/*Left*/0,/*Right*/0);
  delay(1000);

  setMotorSpeed(/*Left*/-100,/*Right*/-100);
  delay(3000);

  setMotorSpeed(/*Left*/0,/*Right*/0);
  delay(2000);

  delay(100);
}