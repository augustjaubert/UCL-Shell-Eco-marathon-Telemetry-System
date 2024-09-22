#include "Node.h"
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_v3.h>

// For ESP32-C3 or ESP32-S3, the pins will be auto-assigned based on the MCU type.
// Input pin types are GPIO_NUM_X, e.g. GPIO_NUM_1 for pin 1.

SFE_UBLOX_GNSS myGNSS;

Node node;

int32_t latitude;
int32_t longitude;
uint32_t unixTime;
uint32_t unixTimeMicro;
int32_t velocity;

int64_t start_time;
int64_t end_time;

void setup() {
  Serial.begin(115200);
  delay(1000);  // Keep this delay to not cause issues with serial printing.
  Wire.begin();
  delay(50);

  while (myGNSS.begin() == false) {
    printf("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing.");
    delay(1);
    //esp_restart();  // restart node because it gets stuck because connection with gps fails on power on
  }
  myGNSS.setI2COutput(COM_TYPE_UBX);  //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.setNavigationFrequency(5);  // Produce ten solutions per second

  //myGNSS.setAutoPVT(true);  // Breaks down with the GPS we have, do not turn ON!
  unixTime = myGNSS.getUnixEpoch(unixTimeMicro);
  printf("unix: %d us: %d\n", unixTime, unixTimeMicro);

  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);
  node.initializeMessage(1, 8);
  node.initializeMessage(22, 8);
  node.initializeMessage(23, 4);

  node.updateMessageData(1, unixTime, unixTimeMicro);
  node.transmitMessage(1, pdMS_TO_TICKS(3000));
  node.deleteMessage(1);
  // Add multiple expected messages
}

void loop() {
  latitude = myGNSS.getLatitude();
  longitude = myGNSS.getLongitude();
  velocity = myGNSS.getGroundSpeed();
  node.updateMessageData(22, latitude, longitude);
  node.updateMessageData(23, velocity);
  node.transmitAllMessages(false, 200);
  //delay(2000);
}