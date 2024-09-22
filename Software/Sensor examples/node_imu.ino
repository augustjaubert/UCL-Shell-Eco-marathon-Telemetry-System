#include "Node.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// For ESP32-C3 or ESP32-S3, the pins will be auto-assigned based on the MCU type.
// Input pin types are GPIO_NUM_X, e.g. GPIO_NUM_1 for pin 1.

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

Node node;

void setup() {
  Serial.begin(115200);
  delay(1000);  // Keep this delay to not cause issues with serial printing.
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);
  //printf("got here\n");
  node.initializeMessage(24, 8);
  node.initializeMessage(25, 4);
  node.displayLatestMessageData();

  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!\n");
    delay(2000);
    esp_restart();
  }

  bno.setExtCrystalUse(true);
  bno.setMode(OPERATION_MODE_ACCONLY);
  delay(100);
}

void loop() {

  // Get the acceleration
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  float x = accel.x();  // Acceleration in X
  float y = accel.y();  // Acceleration in Y
  float z = accel.z();  // Acceleration in Z

  node.updateMessageData(24, x, y);
  node.updateMessageData(25, z);
  node.displayLatestMessageData();
  // Example: node.updateMessageData(10, var1, var2);
  // Example: node.updateMessageData(4, var3);
  node.transmitAllMessages(false, 1000);
  //delay(1000);
  //delay(500);
  // Example: node.transmitAllMessages();
}