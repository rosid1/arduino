// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>
#include <string.h>
#define ROW 5
#define COLUMN 8

char CANBuffer[8];

struct CANPacket{
  uint8_t id;
  uint8_t dlc;
  uint8_t data[8];
}CANPacket;

enum{
  ACC_ID = (uint32_t) 0x44C,
  RPM_ID = (uint32_t) 0x47,
  WHEELPULSE_ID = (uint32_t) 0x0261,
};

uint8_t data_packet[ROW][COLUMN] = {
  {0x40, 0x02, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00},
  {0x40, 0x12, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00},
  {0x10, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x61},
  {0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0xAF, 0x61},
  {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80},
};
String hexString;
uint32_t wp = 0x00;

void CANsend(long id, long dlc, char *message, char msgLength) {
  CAN.beginPacket(id, dlc);
  for (char i = 0; i < msgLength; i ++) {
    CAN.write(message[i]);
  }
  CAN.endPacket();
}

void setBuffer(char *inBuffer, uint8_t data[8]) {
  for (int i = 0; i < 8; i++) {
    inBuffer[i] = data[i];
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  wp++;
  data_packet[4][2] = wp & 0x3F;
  hexString = String(data_packet[4][2]);
  Serial.println(data_packet[4][2], HEX);
  
  struct CANPacket wheelpulse;
  wheelpulse.id = WHEELPULSE_ID;
  wheelpulse.dlc = 0x008;
  
  memcpy(wheelpulse.data, data_packet[4], 8);
  
  setBuffer(CANBuffer, wheelpulse.data);
  
  CANsend(0x0261, wheelpulse.dlc, CANBuffer, 8);
  
  CAN.endPacket();

  delay(500);
}
