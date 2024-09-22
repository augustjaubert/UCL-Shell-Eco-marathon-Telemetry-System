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

Node node;
void setup()
{
  // Begin serial communication at a baud rate of 115200.
  // This is necessary to print messages to the serial monitor.
  Serial.begin(115200);

  // Delay for 1 second to allow the serial monitor to initialize properly.
  // This is necessary to avoid interference with serial printing.
  delay(1000);

  // Initialize the node with the specified RX and TX pins, a standby pin, and a listen mode flag.
  // The node will use the specified pins for CAN communication.
  // The standby pin is used to control the CAN transceiver.
  // The listen mode flag determines whether the node will listen for messages on the CAN bus.
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);

  // Initialize a message container with the specified ID and data length.
  // This function is called once per message ID that is used (i.e. to be sent or received).
  // The message container is used to store the message ID and the data associated with the message.
  node.initializeMessage(10, 8);

  // Initialize another message container with another ID and data length.
  node.initializeMessage(14, 4);

  // Display the data of the latest message in the Node's message map.
  // This function iterates through the messages in the Node's message map to find
  // the message with the highest ID, which is assumed to be the latest message.
  // It then prints the ID and data of the latest message.
  node.displayLatestMessageData();

  // Add multiple expected messages.
  // This vector contains ExpectedMessage objects that represent the expected messages received by the node.
  // Each ExpectedMessage object has an ID and two variable types.
  std::vector<ExpectedMessage> messages = {
      {20, INT32, FLOAT}, // Example of a message with two variables
      {24, UINT32, NONE}  // Example of a message with only one variable
  };

  // Add the expected messages to the Node's internal map of expected messages.
  node.addExpectedMessages(messages);
}

void loop()
{
  /*
   * In the example loop, we update and transmit messages.
   * We update message 10 with sine values of i and -i.
   * We update message 14 with the value -14.0.
   * We transmit all messages.
   * We delay for 1 second before the next iteration.
   */
  for (int i = 0; i < 314; i++)
  {
    // Update message 10 with sine values of i and -i.
    node.updateMessageData(10, (sin((float)i / 100)), (sin((float)-i / 100)));

    // Update message 14 with the value -14.0.
    node.updateMessageData(14, (float)-14);

    // Transmit all messages.
    node.transmitAllMessages(false);

    // Delay for 1 second before the next iteration.
    delay(1000);
  }
}
