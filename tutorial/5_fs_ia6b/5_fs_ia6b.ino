#include <SoftwareSerial.h>

#define IBUS_FRAME_SIZE 32

SoftwareSerial ibusSerial(8, 7);  // RX, TX（TX不用接）

uint8_t ibus_buffer[IBUS_FRAME_SIZE];
uint16_t ibus_channels[10];
uint8_t ibus_idx = 0;
unsigned long lastFrameTime = 0;

void setup() {
  Serial.begin(115200);         // 用於 Serial Monitor
  ibusSerial.begin(115200);     // iBus 接收機的串列速率
}

void loop() {
  while (ibusSerial.available()) {
    uint8_t byteReceived = ibusSerial.read();

    // 幀同步策略：重新同步頭 0x20, 0x40
    if (ibus_idx == 0 && byteReceived != 0x20) continue;
    if (ibus_idx == 1 && byteReceived != 0x40) {
      ibus_idx = 0;
      continue;
    }

    ibus_buffer[ibus_idx++] = byteReceived;

    if (ibus_idx == IBUS_FRAME_SIZE) {
      IBUS_Parse();
      IBUS_get_ch();
      ibus_idx = 0;
      memset(ibus_buffer, 0, IBUS_FRAME_SIZE);
      lastFrameTime = millis();
    }
  }

  // 若超過 20ms 沒收到完整幀 → 清除接收
  if (millis() - lastFrameTime > 20) {
    ibus_idx = 0;
    memset(ibus_buffer, 0, IBUS_FRAME_SIZE);
  }
}

void IBUS_get_ch() {
  for (int i = 0; i < 6; i++) {
    Serial.print("CH"); Serial.print(i + 1); Serial.print(": ");
    Serial.print(ibus_channels[i]); Serial.print("  ");
  }
  Serial.println();
}

bool IBUS_ChecksumValid() {
  uint16_t sum = 0;
  for (int i = 0; i < 30; i++) { // byte[0] ~ byte[29]
    sum += ibus_buffer[i];
  }

  uint16_t receivedChecksum = ibus_buffer[30] | (ibus_buffer[31] << 8);
  uint16_t expectedChecksum = 0xFFFF - sum;

  return receivedChecksum == expectedChecksum;
} 

void IBUS_Parse() {
  if (ibus_buffer[0] != 0x20 || ibus_buffer[1] != 0x40) return;

  uint8_t checksum = 0;
  for (int i = 0; i < 30; i++) {
    checksum += ibus_buffer[i];
  }
  checksum = checksum & 0xff;

  // 可選：驗證校驗和
  if(!IBUS_ChecksumValid()) return;
  //if (checksum != ibus_buffer[30]) return;

  for (int i = 0; i < 10; i++) {
    uint8_t low  = ibus_buffer[2 + i * 2];
    uint8_t high = ibus_buffer[3 + i * 2];
    ibus_channels[i] = (high << 8) | low;
  }
}