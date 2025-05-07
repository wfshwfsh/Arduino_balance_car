/*
 * OCR2A = (0.01 * 16000000)/64 - 1 = 249 
 * => 1 intr per 10ms(0.01sec)
 */

// the setup function runs once when you press reset or power the board
void timer_isr_setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // 停用全域中斷
  cli();
  
  // 設定 Timer2
  TCCR2A = 0;                // 普通操作模式
  TCCR2B = 0;
  TCNT2  = 0;                // 初始值
  
  // 設定比較比對暫存器 (1 秒 = 1Hz，中斷頻率)
  OCR2A = 249;             // 計算方式見下方註解
  
  // 啟用 CTC 模式
  TCCR2A |= (1 << WGM21);
  
  // 設定預除數(prescaler)為 64
  TCCR2B |= (1 << CS22);
  
  // 啟用比較中斷
  TIMSK2 |= (1 << OCIE2A);
  
  // 啟用全域中斷
  sei();
}
