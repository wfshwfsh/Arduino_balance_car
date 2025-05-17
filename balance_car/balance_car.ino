#include <Wire.h>

#define LED_BUILTIN 13
/* 5MS 處理一次平衡... */
#define DT_INTR_CNT5 5
#define PWM_Lim 350
#define Mechanical_Median (0.51)

volatile int timer2IsrCounter = 0;
extern int16_t ax, ay, az, gx, gy, gz;
extern float curAngle;
volatile int dt_intr_ms;

extern float PWM_Up, PWM_Speed, PWM_Steering;
extern volatile long leftCount, rightCount;

int PID_PWM_L=0, PID_PWM_R=0;
float Speed_L, Speed_R;
int16_t Speed = 0;
float PWM_Deviation=0;



// Timer1 中斷函式（每秒觸發一次）
ISR(TIMER2_COMPA_vect) {
  float PWM_Upright=0, PWM_Speed=0, PWM_Steering=0;
  timer2IsrCounter++;
  if (timer2IsrCounter < (DT_INTR_CNT5)){
    return;
  }

  // 1. Get Motor Encoder L+R 
  Speed_L = Get_Speed_L();
  Speed_R = Get_Speed_R();
  //Speed = (Speed_L+Speed_R)/2;
  Speed = (Speed_L+Speed_R);
  //Serial.print("Speed_L="); Serial.print(Speed_L);
  //Serial.print(" | Speed_R="); Serial.print(Speed_R);
  //Serial.print(" | Speed="); Serial.println(Speed);

  // 2. PID_UP
  //PWM_Upright = PID_Upright(Mechanical_Median, pitch, gyro_y);
  PWM_Upright = -PID_Upright(Mechanical_Median, curAngle, gy);

  // 3. PID_Speed
  PWM_Speed = -PID_Speed(0, Speed);

  // 4. PID_Steering
  //PWM_Steering = -PID_Steering(10, yaw, gyro_z + Z_Static_Deviation);

  // Adding balance PWM_Deviation
  PWM_Deviation = -28;

  // 5. Sum & Filter
  PID_PWM_R = PWM_Upright + PWM_Speed - PWM_Steering + PWM_Deviation;
  PID_PWM_L = PWM_Upright + PWM_Speed + PWM_Steering + PWM_Deviation;

  if(PID_PWM_L > PWM_Lim){
    PID_PWM_L = PWM_Lim;
  }else if(PID_PWM_L < -PWM_Lim){
    PID_PWM_L < -PWM_Lim;
  }

  if(PID_PWM_R > PWM_Lim){
    PID_PWM_R = PWM_Lim;
  }else if(PID_PWM_R < -PWM_Lim){
    PID_PWM_R < -PWM_Lim;
  }
  
  show_angle();
  Serial.print("PWM_Upright="); Serial.print(PWM_Upright);
  Serial.print(" | Speed="); Serial.print(Speed);
  Serial.print(" | PWM_Speed="); Serial.println(PWM_Speed);

#if 1
  // Finally, Set PWM to Motor
  setMotorSpeed(PID_PWM_L,PID_PWM_R);
#endif
  timer2IsrCounter=0;
}


void setup() {
  int _dt_intr_ms;
  // Init Serial
  Serial.begin(115200);
  Wire.begin();

  // Init MPU6050
  setup_mpu6050();

  // Init Motors
  setup_t6612_motors();

  // Init Timer ISR
  timer_isr_setup();

  /* Register limitation => intr can only set to 1ms, but we need 5ms */
  _dt_intr_ms = calc_clk(16000000, 64, 249);
  dt_intr_ms = (_dt_intr_ms*DT_INTR_CNT5);
  //Serial.print("dt_intr_ms="); Serial.println(dt_intr_ms);
}

void loop() {
  
  // Read MPU6050 Data
  get_mpu6050_raw_data();
  //show_mpu6050_raw_data();
  angle_calculate();
  //show_angle();

  // Controll motors
  //setMotorSpeed(/*Left*/100,/*Right*/100);
  //delay(3000); 

  //setMotorSpeed(/*Left*/0,/*Right*/0);
  //delay(1000);

  //setMotorSpeed(/*Left*/-100,/*Right*/-100);
  //delay(3000);

  //setMotorSpeed(/*Left*/0,/*Right*/0);
  //delay(2000);

  delay(dt_intr_ms);
}