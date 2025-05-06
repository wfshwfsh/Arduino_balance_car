/*
  Timer interrupt:
  
	ATmega328P 時脈是 16 MHz
	若使用 1024 預除數：
	OCR1A = (16,000,000 Hz) / (1024 * 1 Hz) - 1
	  = 15625 - 1
	  = 15624
  
	表示每 1 秒會觸發一次中斷
*/

#define LED_BUILTIN 13

volatile bool ledState = false;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // 停用全域中斷
  cli();
  
  // 設定 Timer1
  TCCR1A = 0;                // 普通操作模式
  TCCR1B = 0;
  TCNT1  = 0;                // 初始值
  
  // 設定比較比對暫存器 (1 秒 = 1Hz，中斷頻率)
  OCR1A = 15624;             // 計算方式見下方註解
  
  // 啟用 CTC 模式
  TCCR1B |= (1 << WGM12);
  
  // 設定預除數為 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  // 啟用比較中斷
  TIMSK1 |= (1 << OCIE1A);
  
  // 啟用全域中斷
  sei();
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, ledState);
}

// Timer1 中斷函式（每秒觸發一次）
ISR(TIMER1_COMPA_vect) {
  ledState = !ledState; // 切換 LED 狀態
}