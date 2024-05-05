#include <Arduino.h>
#include <Arduino_CAN.h>
#include <string.h>
#include <stdint.h>

#define CAN_SEND_ID 0x22


uint8_t message_data[11][8]; //defined can msgs and the 


void setup() {
  
  Serial.begin(9600);
  while (!Serial) { }
  if (!CAN.begin(CanBitRate::BR_250k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
}

void loop() {
  uint8_t msg_data1[] = {0x6B,0xFD}; // disable write lock
  memcpy(message_data[0], &msg_data1, 2);
  uint8_t msg_data2[] = {0x31,0xFC}; // set the alarm to auto reset
  memcpy(message_data[1], &msg_data2, 2);
  uint8_t msg_data3[] = {0x39,0x2}; // set the current mode to standard
  memcpy(message_data[2], &msg_data3, 2);
  uint8_t msg_data4[] = {0x3B,0x2}; // set the startup mode to standard
  memcpy(message_data[3], &msg_data4, 2);
  uint8_t msg_data5[] = {0x47,0xE1, 0x0}; // Set the trigger resistance to 225 kOhm
  memcpy(message_data[4], &msg_data5, 3);
  uint8_t msg_data6[] = {0x49,0x0A, 0x0};  // 10s trigger timeout alarm
  memcpy(message_data[5], &msg_data6, 3);
  uint8_t msg_data7[] = {0x4B,0xFF, 0x0}; //set the warning to 255 bc I didnt want to do the 16bit hex conversion
  memcpy(message_data[6], &msg_data7, 3);
  uint8_t msg_data8[] = {0x59,0x00, 0x0}; // disabled self-test
  memcpy(message_data[7], &msg_data8, 3);
  uint8_t msg_data9[] = {0x65,0xFE}; // DC voltage mode
  memcpy(message_data[8], &msg_data9, 2);
  uint8_t msg_data10[] = {0x67,0x0, 0x0}; //undervoltage detection disabled
  memcpy(message_data[9], &msg_data10, 3);
  uint8_t msg_data11[] = {0x6B,0xFC};
  memcpy(message_data[10], &msg_data11, 2);

  for(uint32_t addr = 0; addr < 11; addr++){
        // memcpy((void *)(msg_data), &message_data[addr], sizeof(message_data[addr]));
        // Serial.println(addr);
        // *msg = CanMsg(0x400, sizeof(message_data[addr - START_ITEM]), message_data[addr - START_ITEM]);

        int ret = CAN.write(CanMsg(CanStandardId(CAN_SEND_ID), sizeof(message_data[addr]), message_data[addr]));
        if(!(ret == 0 || ret == 1)){
            Serial.print("CAN Error: ");
            Serial.println(ret);
            CAN.clearError();
            // break;
        }
        Serial.println("Programming ...");
        delay(50);
        Serial.println("...");
        // Serial.println(ret);
    }



  Serial.println("Finished programming IMD. Waiting for 10 seconds before restarting");
  delay(10000);
  Serial.println("Restarting now...");
  delay(1000);
}
