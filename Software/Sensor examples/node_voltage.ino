#include "Node.h"
#include <Adafruit_ADS1X15.h>

/**
 * Pin Configuration for XIAO ESP32-C3 and XIAO ESP32-S3
 * 
 * This section defines the GPIO pins used for TX, RX, and controlling
 * the sleep mode of the CAN transceiver chip based on the specific 
 * board variant being used. Ensure that you select the correct pin 
 * configuration for your hardware setup.
 * 
 * TX_GPIO_NUM: Transmit Pin
 * - XIAO ESP32-C3: GPIO_NUM_3
 * - XIAO ESP32-S3 (e.g. Screen Node): GPIO_NUM_2
 * 
 * RX_GPIO_NUM: Receive Pin
 * - XIAO ESP32-C3: GPIO_NUM_4
 * - XIAO ESP32-S3 (e.g. Screen Node): GPIO_NUM_3
 * 
 * sleepPin: Sleep Control Pin for the CAN Transceiver
 * - XIAO ESP32-C3: GPIO_NUM_2
 * - XIAO ESP32-S3: GPIO_NUM_1
 * 
 * Note:
 * - Make sure to define the correct board type before setting these pins.
 * - These configurations are critical for proper communication over the CAN bus.
 */

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

Node node;

void setup() {
  Serial.begin(115200);
  delay(1000);  // Keep this delay to not cause issues with serial printing.
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);
  node.initializeMessage(10, 8);  // Voltage meter 1
  node.initializeMessage(11, 4);  // Voltage meter 1
  // node.initializeMessage(12, 8); // Voltage meter 2

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.setGain(GAIN_TWO);  // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  //ads.setGain(GAIN_FOUR);  // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
  delay(500);
  ads.setDataRate(RATE_ADS1115_8SPS);
}

void loop() {

  int32_t results01 = ads.readADC_Differential_0_1();
  int32_t results23 = ads.readADC_Differential_2_3();
  float mapped01 = map(results01, 0, 16593, 0, 3240) / 1E2;
  float mapped23 = map(results23, 0, 16535, 0, 3240) / 1E2;
  float mappedSOC = mapped23 * 3.08642;
  printf("%d,%f,%d,%f\n", results01, mapped01, results23, mapped23);
  printf("SOC:%f%\n", mappedSOC);
  node.updateMessageData(10, mappedSOC, mapped23);
  node.updateMessageData(11, mapped01);
  //node.updateMessageData(12, mapped01, mapped23);
  node.transmitAllMessages(false, 500);
  //delay(500);
}