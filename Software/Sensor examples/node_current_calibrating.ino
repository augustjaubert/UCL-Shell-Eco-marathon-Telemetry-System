#include "Node.h"
#include <Adafruit_ADS1X15.h>
#include <EEPROM.h>

Adafruit_ADS1115 ads;
Node node;

const int NUM_MEASUREMENTS = 10;
const int CALIBRATION_THRESHOLD = 300;
const int STABLE_READINGS = 20;
const int MIN_CALIBRATION_DIFFERENCE = 10;
int lowerCalibrationValue;       // Lower calibration value for 0 Amps
int upperCalibrationValue;       // Upper calibration value for expected Amps
bool isLowerCalibrated = false;  // Flag to indicate upper calibration
bool isUpperCalibrated = false;  // Flag to indicate upper calibration
int stableCount = 0;
int32_t previousReading;
float expectedAmps;

void setup() {
  Serial.begin(115200);
  delay(1000);
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);
  node.initializeMessage(16, 8);  // Curr1 id = 14, Curr2 id = 15, Curr3 id = 16

  ads.setGain(GAIN_FOUR);
  if (!ads.begin()) {
    printf("Failed to initialize ADS.\n");
    while (1)
      ;
  }

  delay(500);
  ads.setDataRate(RATE_ADS1115_8SPS);

  // Read upper calibration value and expected Amps from EEPROM
  EEPROM.begin(sizeof(upperCalibrationValue) + sizeof(float));
  EEPROM.get(0, upperCalibrationValue);
  EEPROM.get(sizeof(upperCalibrationValue), expectedAmps);

  if (upperCalibrationValue != 0) {
    isUpperCalibrated = true;
    printf("Upper calibration value loaded from EEPROM: %d\n", upperCalibrationValue);
  } else {
    printf("No upper calibration value found in EEPROM.\n");
  }

  if (expectedAmps != 0.0) {
    printf("Expected Amps loaded from EEPROM: %.3f\n", expectedAmps);
  } else {
    printf("No expected Amps found in EEPROM.\n");
  }

  // Lower calibration process
  int sum = 0;
  for (int i = 0; i < NUM_MEASUREMENTS; i++) {
    sum += ads.readADC_Differential_2_3();  // Read ADC value
    delay(130);                             // Delay to ensure stability between measurements
  }
  lowerCalibrationValue = sum / NUM_MEASUREMENTS;  // Calculate average
  isLowerCalibrated = true;
  printf("Sensor calibrated to %d as 0.00A\n", lowerCalibrationValue);

  // Initialize previousReading with the initial calibration value
  previousReading = lowerCalibrationValue;
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'c') {
      calibrateSensor();
    }
  }

  int32_t adc23 = ads.readADC_Differential_2_3();

  if (abs(adc23 - lowerCalibrationValue) <= CALIBRATION_THRESHOLD && abs(adc23 - previousReading) <= CALIBRATION_THRESHOLD) {
    stableCount++;
  } else {
    stableCount = 0;
  }

  if (stableCount >= STABLE_READINGS) {
    if (abs(adc23 - lowerCalibrationValue) >= MIN_CALIBRATION_DIFFERENCE) {
      lowerCalibrationValue = adc23;
      printf("Recalibrated to %d as 0.00A\n", lowerCalibrationValue);
      isLowerCalibrated = true;
    }
    stableCount = 0;
  }

  float current;
  if (isUpperCalibrated) {
    current = map(adc23, lowerCalibrationValue, upperCalibrationValue, 0, expectedAmps * 1E2) / 1E2;
  } else {
    current = map(adc23, lowerCalibrationValue, 15430, 0, 3000) / 1E2;
  }

  printf("%d, %.3f\n", adc23, current);
  node.updateMessageData(16, current, adc23);  // Curr1 id = 14, Curr2 id = 15, Curr3 id = 16
  previousReading = adc23;

  node.transmitAllMessages(false, 500);
}

void calibrateSensor() {
  int sum = 0;
  printf("Calibrating sensor. Please keep the sensor stable.\n");
  for (int i = 0; i < NUM_MEASUREMENTS; i++) {
    sum += ads.readADC_Differential_2_3();
    delay(130);
  }

  upperCalibrationValue = sum / NUM_MEASUREMENTS;
  printf("Upper calibration completed. New calibration value: %d\n", upperCalibrationValue);
  isUpperCalibrated = true;

  // Prompt user for expected Amps
  printf("Enter the expected current in Amps for upper calibration:\n");
  while (!Serial.available()) {
    delay(5000);  // Add a small delay to prevent the loop from hogging resources
  }
  expectedAmps = Serial.parseFloat();
  printf("Expected current set to: %.3f\n", expectedAmps);

  // Store calibration value and expected Amps in EEPROM
  EEPROM.put(0, upperCalibrationValue);
  EEPROM.put(sizeof(upperCalibrationValue), expectedAmps);
  EEPROM.commit();  // Commit the changes to EEPROM
}