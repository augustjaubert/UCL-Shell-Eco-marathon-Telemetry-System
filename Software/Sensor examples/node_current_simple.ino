#include "Node.h"
#include <Adafruit_ADS1X15.h>

// For ESP32-C3 or ESP32-S3, the pins will be auto-assigned based on the MCU type.
// Input pin types are GPIO_NUM_X, e.g. GPIO_NUM_1 for pin 1.

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */
Node node;

const int NUM_MEASUREMENTS = 10;  // Number of measurements to average
int calibrationValue;             // Variable to store the calibration value
bool isCalibrated = false;        // Flag to indicate if calibration is done

void setup() {
  Serial.begin(115200);
  delay(1000);  // Keep this delay to not cause issues with serial printing.
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);
  node.initializeMessage(14, 4);

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);  // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);  // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  ads.setGain(GAIN_FOUR);  // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV (USE THIS GAIN IN DIFFERENTIAL MODE)
  // ads.setGain(GAIN_EIGHT);  // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  //ads.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
  delay(500);
  ads.setDataRate(RATE_ADS1115_8SPS);

  // Calibration process
  int sum = 0;
  for (int i = 0; i < NUM_MEASUREMENTS; i++) {
    sum += ads.readADC_Differential_2_3();  // Read ADC value
    delay(130);                             // Delay to ensure stability between measurements
  }
  calibrationValue = sum / NUM_MEASUREMENTS;  // Calculate average
  isCalibrated = true;
  printf("Sensor calibrated to %d as 0.00A\n", calibrationValue);
}

void loop() {
  //uint16_t adc2 = ads.readADC_SingleEnded(2);
  int32_t adc23 = ads.readADC_Differential_2_3();

  // Map the current ADC value using the average of the first NUM_MEASUREMENTS measurements
  float current;
  if (isCalibrated) {
    current = map(adc23, calibrationValue, 15430, 0, 3000) / 1E2;
    printf("%d, %f A\n", adc23, current);
    //node.updateMessageData(14, current);
    //node.transmitAllMessages();
  } else {
    // If measurements array is not filled yet, use a default value for mapping
    current = map(adc23, 5, 3866, 0, 3000) / 1E2;
    printf("%d, %f A\n", adc23, current);
  }
  //float current = map(adc23, 5, 6472, 0, 5000) / 1E2;
  //printf("0-1: %d  2-3: %d\n", results01, results23);

  delay(500);
}