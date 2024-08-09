# In the vehicle

## NODE

The sensors are configured as nodes on the CAN bus, meaning they each are paired with a small computer board. The latter acts as the middleman between the sensor’s interfacing protocol (i.e. the language the sensor talks in) and the CAN bus (i.e. what the telemetry system talks in).



The node computer hardware diagram is presented in the figure below.

<figure><picture><source srcset=".gitbook/assets/node dark 1@4x.png" media="(prefers-color-scheme: dark)"><img src=".gitbook/assets/node light 1@4x.png" alt=""></picture><figcaption></figcaption></figure>

<figure><picture><source srcset=".gitbook/assets/node dark 2@4x.png" media="(prefers-color-scheme: dark)"><img src=".gitbook/assets/node light 2@4x.png" alt=""></picture><figcaption><p>Technical diagrams of the node computer</p></figcaption></figure>

### Technical specs

{% tabs %}
{% tab title="Tech specs" %}
* The nodes use a ESP32 microcontroller (S3 or C3 models) from Espressif, manufactured by Seeed ([XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO\_ESP32C3\_Getting\_Started/) / [XIAO ESP32S3](https://wiki.seeedstudio.com/xiao\_esp32s3\_getting\_started/)) (top surface).
* It uses two rows of 6 and 5 female headers (top surface) to connect to the sensor shield.
* They are equipped with [TCAN1462V-Q1](https://www.ti.com/lit/ds/symlink/tcan1462-q1.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe\&ts=1698833597358) CAN transceivers (bottom surface) that make use of the MCU’s native [TWAI](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/twai.html#overview) controllers.
* They have two daisy-chained 4-pin JST XH connectors (top surface) for the data transmission CAN High / Low, and the power pins from the Receiver +5V and GND.
* The slide switch on the top surface is the CAN termination switch, connecting the CAN High and Low lines by a 120 Ohm resistor.
* The CAN lines also have footprint for capacitors in series and parallel (bottom surface) to help if filtering is needed on the bus.
{% endtab %}

{% tab title="Pinout" %}
<figure><img src=".gitbook/assets/node pinout light@4x.png" alt=""><figcaption></figcaption></figure>

{% hint style="info" %}
This pinout diagram is valid for the XIAO ESP32-C3 microcontrollers, but some pins also double as analog pins on the XIAO ESP32-S3 (see Table 1), marked as A\<Number> (e.g. A5).
{% endhint %}

<table><thead><tr><th width="154">pin #</th><th>C3 pinout</th><th>S3 pinout</th></tr></thead><tbody><tr><td>1</td><td>5V</td><td>5V</td></tr><tr><td>2</td><td>GND</td><td>GND</td></tr><tr><td>3</td><td>3.3V</td><td>3.3V</td></tr><tr><td>4</td><td>D7 / RX / GPIO20</td><td>D7 / RX / GPIO44</td></tr><tr><td>5</td><td>D8 / SCK / GPIO8</td><td>D8 / A8 / SCK / GPIO7</td></tr><tr><td>6</td><td>D9 / MISO / GPIO9</td><td>D9 / A9 / MISO / GPIO8</td></tr><tr><td>7</td><td>D6 / TX / GPIO21</td><td>D6 / TX / GPIO43</td></tr><tr><td>8</td><td>D5 / SCL / GPIO7</td><td>D5 / A5 / SCL / GPIO6</td></tr><tr><td>9</td><td>D4 / SDA / GPIO6</td><td>D4 / A4 / SDA / GPIO5</td></tr><tr><td>10</td><td>D3 / A3 / GPIO5</td><td>D3 / A3 / GPIO4</td></tr><tr><td>11</td><td>D10 / MOSI / GPIO10</td><td>D10 / A10 / MOSI / GPIO9</td></tr><tr><td>CAN_TX</td><td>GPIO_NUM_3</td><td>GPIO_NUM_2</td></tr><tr><td>CAN_RX</td><td>GPIO_NUM_4</td><td>GPIO_NUM_3</td></tr><tr><td>CAN_Standby</td><td>GPIO2</td><td>GPIO1</td></tr></tbody></table>
{% endtab %}
{% endtabs %}

### Setup

#### Hardware

1. Assembling the node case (in ascending order)

<figure><img src=".gitbook/assets/node assembly light@4x.png" alt=""><figcaption></figcaption></figure>

2. Attaching the node to the CAN/Power bus

Connecting to the CAN and Power bus is easy. There are two 4-pin JST XH connectors opposite to the USB-C connector, which are connected in parallel for you to daisy-chain the whole system. This means that there’s no wrong way to connect it, just attach one cable to the bus, and if you want to add a new sensor, just attach another cable in the other connector and into the new node.

<figure><picture><source srcset=".gitbook/assets/can assembly dark@4x.png" media="(prefers-color-scheme: dark)"><img src=".gitbook/assets/can assembly light@4x.png" alt=""></picture><figcaption></figcaption></figure>

3. Installing in the vehicle

Place the node wherever it is needed in the vehicle.

<figure><img src=".gitbook/assets/sensor placement light@4x.png" alt=""><figcaption></figcaption></figure>

{% hint style="info" %}
If you need to make a new cable, make sure to use the proper connections as seen in the figure below.
{% endhint %}

<figure><picture><source srcset=".gitbook/assets/cable wiring dark@4x.png" media="(prefers-color-scheme: dark)"><img src=".gitbook/assets/cable wiring light@4x.png" alt="" width="375"></picture><figcaption><p>CAN &#x26; Power cable wiring diagram</p></figcaption></figure>



#### Software

1. Installing the integrated development environment (IDE)

The system works similar to most Arduino projects, as it has been coded in C++ using the Arduino IDE.

Any IDE will do the job, however we recommend using the following:

[Arduino IDE](https://www.arduino.cc/en/software) <mark style="color:orange;">is the easiest to use, has plenty of documentation and forum help and is sufficient almost 99% of the time.</mark>

[Platform IO](https://platformio.org/) (through VSCode) is essentially a pro version, where you have more control but requires a manual setup.

[ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#introduction) is the MCU's native IDE, which gives more liberty in using all its functions.

2. ESP board definitions

The ESP boards (MCU) are not defined by default in the Arduino IDE. You will have to add them initially to be able to connect to the nodes and others.

To do this in the Arduino IDE, navigate to **File > Preferences**, and fill **"Additional Boards Manager URLs"** with the url below: [_https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package\_esp32\_index.json_](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package\_esp32\_index.json)

Navigate to **Tools > Board > Boards Manager...**, type the keyword "**esp32**" in the search box, select the latest version of **esp32**, and install it.

3. Installing the node library

Download the two files `Node.h` and `Node.cpp` from the [Github repository](https://github.com/augustjaubert/UCL-SEM-Telemetry-System) in the Node folder.

To add the node library to your arduino sketch[^1], simply add the two files in your sketch folder.

<details>

<summary>Example folder organisation</summary>

```
example_sketch (folder)
-> Node.cpp
-> Node.h
-> example_sketch.ino (arduino sketch)
```

</details>

### How to use

Find the quick guide in the [overview section](./#quick-installation-guide).

{% hint style="info" %}
You can add up to 70+ nodes onto the CAN bus, however limiting the number of sensors to <15 is recommended (i.e. limit the different messages competing on the CAN bus).
{% endhint %}

#### Steps

1. <mark style="color:orange;">Switch the node ON</mark>.

<figure><img src=".gitbook/assets/node switch light@4x (1).png" alt="" width="375"><figcaption></figcaption></figure>

2. <mark style="color:orange;">Assemble the node</mark> (sensor, computer and case).
3. <mark style="color:orange;">Connect a CAN cable</mark> to one of the JST sockets to add the node on the CAN bus.
4. If the node is added at the end of the bus, <mark style="color:orange;">switch the CAN termination ON.</mark>

{% hint style="info" %}
After the system is powered, <mark style="color:orange;">the node is powered</mark> <mark style="color:orange;">if you see a LED lighting up.</mark>
{% endhint %}

{% hint style="warning" %}
Later versions of the node computer boards have a <mark style="color:orange;">slide switch on the bottom</mark>, which need to be activated for the power rail to go through the board.&#x20;

Be careful because earlier versions have reversed ON/OFF writings by mistake, so if in doubt test it outside the vehicle, and if you see a blinking red/yellow light coming from the node, it is powered.
{% endhint %}

#### Troubleshooting

If it seems as though the Receiver is not seeing the sensor messages, or something else is not working as it should:

1. Disassemble the node.
2. Connect the node computer to your laptop through USB.
3. Open Arduino IDE, and check that you have the correct board definition in the top-left text box.

<figure><img src=".gitbook/assets/image.png" alt="" width="375"><figcaption></figcaption></figure>

<details>

<summary>No board appearing</summary>

If you don't see a board appearing, then the MCU is not powered, or the serial connection has been severed. This can be because,

* The USB cable is not properly connected either to the node or laptop
* The MCU is not receiving power, which can be checked with a multimeter on the MCU pins to see if it detects a voltage. This can be due to a severed power connection on the circuit.
* The serial connection is severed, which can be because&#x20;
  * the USB connector on the MCU has been damaged,&#x20;
  * or because something (e.g. static electricity) has caused your laptop to disconnect the USB port (safety mode). It happens that connection is re-established after some time.
    * You can first try to reset the board by clicking the **RESET BUTTON** once while the board is connected to your PC. If that does not work, hold the **BOOT BUTTON**, connect the board to your PC while holding the **BOOT** button, and then release it to enter **bootloader mode**.

</details>

<details>

<summary>The wrong board is appearing</summary>

If the wrong board is appearing, that is a good sign because it recognises something.

You can open the text box, click on select <mark style="color:orange;">other board and port...</mark> and search for the appropriate board.

The boards are,

For C3 models - XIAO\_ESP32C3

For S3 models - XIAO\_ESP32S3

S3 models are identifiable by the writing on the sticker and the presence of solderable pins on the bottom, as seen on the right side of the image below.

<img src=".gitbook/assets/image (1).png" alt="" data-size="original">

</details>

4. Open the serial monitor, in the top-right corner.

<figure><img src=".gitbook/assets/image (2).png" alt="" width="188"><figcaption></figcaption></figure>

This will present what is output on the serial port by the board, which you can use to deduce where the issues are coming from.

e.g. it can display "sensor not found on I2C bus" even when the sensor board is connected meaning that there is a connection issue between the node computer and sensor board).

{% hint style="info" %}
The serial output will only output things that it has been coded to output, so it is your responsibility to leave serial print commands in your code for debugging.
{% endhint %}

### Development

#### Node Library - to code a node

{% tabs %}
{% tab title="API reference" %}
The `Node` library provides a framework for interacting with a CAN bus network using the ESP-IDF framework on ESP32 devices. It includes functionalities for initializing the CAN bus, sending and receiving messages, and managing message expectations.

#### Classes and Structures

**`Node`**

**Constructor**

```cpp
Node();
```

Creates an instance of the `Node` class.

**Public Methods**

* `void begin(gpio_num_t canRxPin = GPIO_NUM_NC, gpio_num_t canTxPin = GPIO_NUM_NC, uint8_t canStdbyPin = 0, bool listenMode = false)`&#x20;
  * Initialises the CAN bus with the specified pins and mode.
* `void initializeMessage(uint32_t id, uint8_t dataLength)`&#x20;
  * Initialises a message container with the specified ID and data length.
* `void deleteMessage(uint32_t id)`&#x20;
  * Deletes a message from the `messages` container.
* `template <typename T1, typename T2 = std::nullptr_t> void updateMessageData(uint32_t id, T1 var1, T2 var2 = nullptr)`&#x20;
  * Updates the data of a message with the given ID in the Node's message map.
* `void transmitAllMessages(bool lowPowerMode = false, uint32_t sleepDurationMs = 1000)`&#x20;
  * Transmits all messages in the Node's message map to the CAN bus.
* `void transmitMessage(uint32_t id, TickType_t ticks_to_wait = pdMS_TO_TICKS(1000))`&#x20;
  * Transmits a specific message with the specified ID and waits for the specified number of ticks.
* `void addExpectedMessage(uint32_t id, VariableType var1Type)`&#x20;
  * Adds a single expected message with only one variable.
* `void addExpectedMessage(uint32_t id, VariableType var1Type, VariableType var2Type)`&#x20;
  * Adds an expected message with two variables.
* `void addExpectedMessages(const std::vector<ExpectedMessage> &messages)`&#x20;
  * Adds a collection of expected messages to the internal map.
* `std::pair<uint32_t, std::pair<String, String>> parseReceivedMessage()`&#x20;
  * Parses the received CAN message and returns the message ID and string representations of the first and second variables in the message payload.
* `void displayLatestMessageData()`&#x20;
  * Displays the data of the latest message in the Node's message map.

**Private Methods**

* `void initializeCANBus(gpio_num_t canRxPin, gpio_num_t canTxPin, uint8_t canStdbyPin, bool listenMode)`&#x20;
  * Initialises the CAN bus with the specified pins and mode.
* `template <typename T> void convertToBytes(T value, byte *buffer)`&#x20;
  * Converts a value of type `T` to a byte array.

**`VariableType`**

An enumeration defining the possible types of variables that can be included in a CAN message:

* `INT32`: 32-bit signed integer.
* `UINT32`: 32-bit unsigned integer.
* `FLOAT`: Floating-point number.
* `NONE`: No variable (used when there is only one variable).

**`ExpectedMessage`**

A structure representing an expected message with its identifier and variable types:

* `uint32_t id`: The identifier of the expected message.
* `VariableType var1Type`: The type of the first variable in the message payload.
* `VariableType var2Type`: The type of the second variable in the message payload.

#### Usage Notes

* The `Node` class methods provide a high-level interface for interacting with a CAN bus network, abstracting away the complexities of direct hardware manipulation.
* The `VariableType` enumeration and `ExpectedMessage` structure facilitate the definition and handling of expected messages within the CAN network.
{% endtab %}

{% tab title="Example sketch" %}
This example code initialises the CAN messages in the setup, and then in the loop updates them and sends them onto the bus.

```cpp
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
```
{% endtab %}

{% tab title="Example screen" %}
This example code receives CAN messages on the bus and hypothetically updates the screen accordingly.

```cpp
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
    
    // ADD THE CODE TO UPDATE THE SCREEN HERE
  }
}
```
{% endtab %}

{% tab title="Example sensor" %}
This code was made for the voltmeter node, to show the implementation of the node library with a specific sensor (i.e. TI ADS1115 ADC chip).

```cpp
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
 * - XIAO ESP32-C3: 2
 * - XIAO ESP32-S3: 1
 *
 * Note:
 * - Make sure to define the correct board type before setting these pins.
 * - These configurations are critical for proper communication over the CAN bus.
 */

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

Node node;
void setup()
{
  // Initialize serial communication
  Serial.begin(115200);

  // Delay to prevent issues with serial printing
  delay(1000);

  // Initialize the Node with specific GPIO pins and standby pin
  node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);

  // Initialize messages with specific IDs and data lengths
  node.initializeMessage(10, 8);
  node.initializeMessage(11, 4);

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.setGain(GAIN_TWO); // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);  // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  //  ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  //  ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  // Initialize the ADS1115 ADC
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }

  // Delay after initialization
  delay(500);

  // Set the data rate for the ADS1115 ADC
  ads.setDataRate(RATE_ADS1115_8SPS);
}

void loop()
{
  // Read ADC values for differential inputs 0-1 and 2-3
  int32_t results01 = ads.readADC_Differential_0_1();
  int32_t results23 = ads.readADC_Differential_2_3();

  // Map the ADC values to the desired range
  float mapped01 = map(results01, 5100, 32640, 500, 3200) / 1E2;
  // float mapped23 = map(results23, 5100, 32640, 500, 3200) / 1E2;
  // float mappedSOC = map((int32_t)(mapped23 * 10), 0, 327, 0, 100);

  // printf("0-1: %d  2-3: %d\n", results01, results23);
  printf("%d,%d,\n", results01, results23);
  printf("%f,\n", mapped01);

  // Update the message data
  // node.updateMessageData(10, mappedSOC, mapped23);
  // node.updateMessageData(11, mapped01);
  // node.transmitAllMessages();
  
  delay(500);
}
```
{% endtab %}
{% endtabs %}

#### Design a new sensor

The sensors come in all forms, but only one shape. They all have a standardised footprint that you can build on to make new sensors while still remaining compatible with the node computer.

<figure><img src=".gitbook/assets/image (3).png" alt="" width="375"><figcaption><p>Template sensor shield footprint (Unit: mm)</p></figcaption></figure>

The footprint has four M3 sized holes for support, and two rows of pin headers placed just like the [node computer pinout diagram](in-the-vehicle.md#technical-specs).

The fusion PCB file can be found on the Github repository under **Hardware**, and has the pin header sensor connections already pre-defined to simplify the design.

Some general help and thoughts on selecting a sensor:

* Think about what you want to know performance-wise (e.g. fuel consumption etc.), and search up what kind of sensor best suits your requirements.

You can have multiple sensors that can accomplish the same task, but have different pros and cons.&#x20;

e.g. to measure velocity, we could have had an RPM sensor or a GPS. GPS was a safer, more absolute solution, and in this case we already needed it for position.

* On big retailers websites (e.g. Mouser, Farnell, DigiKey, etc.), look for&#x20;
  * the price (i.e. unit or bulk?),&#x20;
  * availability (i.e. is it end of life?),&#x20;
  * size (i.e. does it fit onthe board?),&#x20;
  * functionalities (i.e. handy reliability mechanisms, etc.)



* What communcation protocol does it use?

Connecting to your sensor depends on the protocol the sensor uses to communicate. These can be Serial-Peripheral-Interface (SPI), Inter-Integrated Communication (I2C), UART or you have some general-purpose input/output (GPIO) pins that you can reprogram based on the ESP32-C3 or ESP32-S3 capabilities. The board also has an input for simple analog voltage reading up to 2.5V, however it is recommended to use the ADC shield instead of the ADC pins as they have lower resolution and impedance (essentially just worse, check out Espressif’s documentation).

* Does it have an ECAD footprint available?

Often you are able to find the footprint ready-made by a retailer on their page (Mouser, etc.) or by a hobbyist. This simplifies the design process, however it is recommended to always double check the pinout matches that of the datasheet, as well as the dimensions.

{% hint style="info" %}
**Tip:** Find sensors with extensive online coverage (arduino tutorials, pre-existing code, etc.) as well as from breakout board manufacturers (e.g. Adafruit, Sparkfun, Seed). They often open source their electrical diagrams and code.
{% endhint %}

When it comes to wiring the sensor on the board and adding the necessary components around the chip, it is best to follow the datasheet given by the manufacturer (e.g. Texas Instruments). They often present example schematics showcasing the most common setups.

Most of the sensors developed this year were chosen because they had been covered in online tutorials, and thus had already proven code and easy-to-use libraries for them. This makes it easy to pair with our node library, as you can see in the [sensor example](in-the-vehicle.md#example-sensor).

#### Build a computer

Tips on soldering

Parts list (BOM)

schematics







## Receiver

### setup

#### hardware

#### software

Similar to in the [node software setup](in-the-vehicle.md#software).







## 2024 configuration







[^1]: This is the name for an arduino project, essentially the code you upload to a microcontroller.
