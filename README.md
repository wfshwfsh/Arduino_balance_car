# Arduino_balance_car

## Basic Arduino Tutorial
* [Done] Blink Led
* [Done] Timer Interrupt
* [Done] Get MPU6050 Stream Data
* [Done] T6612 & JGB37-520 motor

## PID Balance Car with MPU6050 and L298n motor
* [Done] Using Timer Interrupt
* [Done] Get MPU6050 Data
* [Done] Control Two motors by T6612
* Get Two motors' encoder
* Angle Calculate
* PID Balance
  * UP
  * Speed
  * Steering



## Meet Issues

* Pin Conflict: Timer Interrupt timer1 & PWMB => Both using D9
  * Fixed by Change Timer Interrupt to timer2 (D3,D11)
    * Timer Pin CAN NOT using as interrupt & PWM output (analogWrite()) at same time