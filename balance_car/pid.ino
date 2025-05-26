//直立环
float PID_Upright_Kp = 36.5*0.6;//50*0.6;
float PID_Upright_Kd = 1.412*0.6;

//速度环
float PID_Speed_Kp = 10;//3.23;
float PID_Speed_Ki = (PID_Speed_Kp/200);//(0.53/200);

//转向环
float PID_Steering_Kp = 0;
float PID_Steering_Kd = 1.8;

float PWM_Up=0, PWM_Speed=0, PWM_Steering=0;
int Speed_Integral=0, Speed_Err_Last=0;


int PID_Upright(float expect, float angle, float gyro)
{
  int PWM_out;
  PWM_out = PID_Upright_Kp * (angle - expect) + PID_Upright_Kd * gyro;
  return PWM_out;
}

int PID_Speed(int16_t expect, int16_t speed)
{
  int PWM_out, Speed_Err=0;
  //static int Speed_Err_Last=0;
  float a = 0.2;

  Speed_Err = speed - expect;                           //误差计算
  Speed_Err = (1 - a) * Speed_Err + a * Speed_Err_Last; //低通滤波
  Speed_Err_Last = Speed_Err;                           //更新上次误差

  Speed_Integral += Speed_Err; //积分计算

  if (Speed_Integral > 10000)
      Speed_Integral = 0;
  if (Speed_Integral < -10000)
      Speed_Integral = 0; //积分限幅

  PWM_out = PID_Speed_Kp * Speed_Err + PID_Speed_Ki * Speed_Integral; //计算输出
  //Serial.print("Speed_Err="); Serial.print(Speed_Err);
  //Serial.print(" | PWM_out="); Serial.println(PWM_out);
  return PWM_out;
}

int PID_Steering(int16_t expect, float angle, float gyro)
{
  int PWM_out;
  float a = 0.7;
  static float gyro_Last;
  gyro = (1 - a) * gyro + a * gyro_Last; //低通滤波
  PWM_out = PID_Steering_Kp * (angle - expect) + PID_Steering_Kd * gyro;
  return PWM_out;
}

