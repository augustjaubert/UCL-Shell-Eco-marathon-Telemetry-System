#include "Node.h"

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
 * - XIAO ESP32-C3: 2
 * - XIAO ESP32-S3: 1
 *
 * Note:
 * - Make sure to define the correct board type before setting these pins.
 * - These configurations are critical for proper communication over the CAN bus.
 */

Node node; // Create an instance of the Node class

// The setup function is called once at startup
void setup() {
  // Initialize serial communication at a baud rate of 115200.
  Serial.begin(115200);

  // Introduce a delay of 1 second to allow for serial printing.
  // This delay is necessary to avoid issues with the serial output.
  delay(1000);

  // Initialize the CAN node with the specified pins and configuration.
  // The CAN_RX pin is GPIO_NUM_4, CAN_TX pin is GPIO_NUM_3,
  // CAN_STBY pin is 2, and CAN_LISTEN_MODE is set to false.
  // The CAN_RX and CAN_TX pins are specific to the XIAO ESP32-C3 and XIAO ESP32-S3.
  // Ensure to configure the correct pin configuration for your hardware setup.
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2, false);

  // Define multiple expected messages.
  // Each message is represented by an instance of the ExpectedMessage struct.
  // The struct contains the message ID and the variable types of the expected message payload.
  // In this example, we have two messages:
  // - Message ID: 10, Variable Type: FLOAT, FLOAT
  // - Message ID: 14, Variable Type: FLOAT, NONE (no second variable)
  std::vector<ExpectedMessage> messages = {
    { 10, FLOAT, FLOAT },
    { 14, FLOAT, NONE }
  };

  // Add the expected messages to the node.
  // This informs the node about the expected messages to receive.
  node.addExpectedMessages(messages);
}

// Main loop
void loop() {
  // Parse the received message and get the message ID, variable 1, and variable 2
  // The first element of the pair is the message ID, and the second element is a pair of strings for variable 1 and variable 2
  auto parsedMessage = node.parseReceivedMessage();

  // Check if a valid message ID was returned
  if (parsedMessage.first != 0) {
    // Get the message ID, variable 1, and variable 2
    uint32_t messageId = parsedMessage.first;  // The ID of the received message
    String var1 = parsedMessage.second.first;  // The value of the first variable in the message payload
    String var2 = parsedMessage.second.second;  // The value of the second variable in the message payload

    // Print the message ID and the values of the variables on the serial monitor
    printf("Message ID: %u\n", messageId);
    printf("Var1: %s\n", var1.c_str());
    printf("Var2: %s\n", var2.c_str());
  }
}
