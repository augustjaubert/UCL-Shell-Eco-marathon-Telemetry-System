# In the vehicle

## Node

The node computer hardware diagram is presented in the figure below.

<figure><picture><source srcset=".gitbook/assets/node dark 1@4x.png" media="(prefers-color-scheme: dark)"><img src=".gitbook/assets/node light 1@4x.png" alt=""></picture><figcaption></figcaption></figure>

<figure><picture><source srcset=".gitbook/assets/node dark 2@4x.png" media="(prefers-color-scheme: dark)"><img src=".gitbook/assets/node light 2@4x.png" alt=""></picture><figcaption><p>Technical diagrams of the node computer</p></figcaption></figure>

### technical specs

* The nodes use a ESP32 microcontroller (S3 or C3 models) from Espressif, manufactured by Seeed ([XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO\_ESP32C3\_Getting\_Started/) / [XIAO ESP32S3](https://wiki.seeedstudio.com/xiao\_esp32s3\_getting\_started/)) (top surface).
* It uses two rows of 6 and 5 female headers (top surface) to connect to the sensor shield.
* They are equipped with [TCAN1462V-Q1](https://www.ti.com/lit/ds/symlink/tcan1462-q1.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe\&ts=1698833597358) CAN transceivers (bottom surface) that make use of the MCU’s native [TWAI](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/twai.html#overview) controllers.
* They have two daisy-chained 4-pin JST XH connectors (top surface) for the data transmission CAN High / Low, and the power pins from the Receiver +5V and GND.
* The slide switch on the top surface is the CAN termination switch, connecting the CAN High and Low lines by a 120 Ohm resistor.
* The CAN lines also have footprint for capacitors in series and parallel (bottom surface) to help if filtering is needed on the bus.

<details>

<summary>technical specs</summary>

* The nodes use a ESP32 microcontroller (S3 or C3 models) from Espressif, manufactured by Seeed ([XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO\_ESP32C3\_Getting\_Started/) / [XIAO ESP32S3](https://wiki.seeedstudio.com/xiao\_esp32s3\_getting\_started/)) (top surface).
* It uses two rows of 6 and 5 female headers (top surface) to connect to the sensor shield.
* They are equipped with [TCAN1462V-Q1](https://www.ti.com/lit/ds/symlink/tcan1462-q1.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe\&ts=1698833597358) CAN transceivers (bottom surface) that make use of the MCU’s native [TWAI](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/twai.html#overview) controllers.
* They have two daisy-chained 4-pin JST XH connectors (top surface) for the data transmission CAN High / Low, and the power pins from the Receiver +5V and GND.
* The slide switch on the top surface is the CAN termination switch, connecting the CAN High and Low lines by a 120 Ohm resistor.
* The CAN lines also have footprint for capacitors in series and parallel (bottom surface) to help if filtering is needed on the bus.

</details>

{% tabs %}
{% tab title="tech specs" %}
* The nodes use a ESP32 microcontroller (S3 or C3 models) from Espressif, manufactured by Seeed ([XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO\_ESP32C3\_Getting\_Started/) / [XIAO ESP32S3](https://wiki.seeedstudio.com/xiao\_esp32s3\_getting\_started/)) (top surface).
* It uses two rows of 6 and 5 female headers (top surface) to connect to the sensor shield.
* They are equipped with [TCAN1462V-Q1](https://www.ti.com/lit/ds/symlink/tcan1462-q1.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe\&ts=1698833597358) CAN transceivers (bottom surface) that make use of the MCU’s native [TWAI](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/twai.html#overview) controllers.
* They have two daisy-chained 4-pin JST XH connectors (top surface) for the data transmission CAN High / Low, and the power pins from the Receiver +5V and GND.
* The slide switch on the top surface is the CAN termination switch, connecting the CAN High and Low lines by a 120 Ohm resistor.
* The CAN lines also have footprint for capacitors in series and parallel (bottom surface) to help if filtering is needed on the bus.
{% endtab %}

{% tab title="pinout" %}

{% endtab %}

{% tab title="pinout table" %}
<figure><img src=".gitbook/assets/node pinout light@4x.png" alt=""><figcaption></figcaption></figure>

{% hint style="info" %}
This pinout diagram is valid for the XIAO ESP32-C3 microcontrollers, but some pins also double as analog pins on the XIAO ESP32-S3 (see Table 1), marked as A\<Number> (e.g. A5).
{% endhint %}

<table><thead><tr><th width="126">pin #</th><th>C3 pinout</th><th>S3 pinout</th></tr></thead><tbody><tr><td>1</td><td>5V</td><td>5V</td></tr><tr><td>2</td><td>GND</td><td>GND</td></tr><tr><td>3</td><td>3.3V</td><td>3.3V</td></tr><tr><td>4</td><td>D7 / RX / GPIO20</td><td>D7 / RX / GPIO44</td></tr><tr><td>5</td><td>D8 / SCK / GPIO8</td><td>D8 / A8 / SCK / GPIO7</td></tr><tr><td>6</td><td>D9 / MISO / GPIO9</td><td>D9 / A9 / MISO / GPIO8</td></tr><tr><td>7</td><td>D6 / TX / GPIO21</td><td>D6 / TX / GPIO43</td></tr><tr><td>8</td><td>D5 / SCL / GPIO7</td><td>D5 / A5 / SCL / GPIO6</td></tr><tr><td>9</td><td>D4 / SDA / GPIO6</td><td>D4 / A4 / SDA / GPIO5</td></tr><tr><td>10</td><td>D3 / A3 / GPIO5</td><td>D3 / A3 / GPIO4</td></tr><tr><td>11</td><td>D10 / MOSI / GPIO10</td><td>D10 / A10 / MOSI / GPIO9</td></tr></tbody></table>
{% endtab %}
{% endtabs %}



<table><thead><tr><th width="126">Pin #</th><th>C3 Pinout</th><th>S3 Pinout</th></tr></thead><tbody><tr><td>1</td><td>5V</td><td>5V</td></tr><tr><td>2</td><td>GND</td><td>GND</td></tr><tr><td>3</td><td>3.3V</td><td>3.3V</td></tr><tr><td>4</td><td>D7 / RX / GPIO20</td><td>D7 / RX / GPIO44</td></tr><tr><td>5</td><td>D8 / SCK / GPIO8</td><td>D8 / A8 / SCK / GPIO7</td></tr><tr><td>6</td><td>D9 / MISO / GPIO9</td><td>D9 / A9 / MISO / GPIO8</td></tr><tr><td>7</td><td>D6 / TX / GPIO21</td><td>D6 / TX / GPIO43</td></tr><tr><td>8</td><td>D5 / SCL / GPIO7</td><td>D5 / A5 / SCL / GPIO6</td></tr><tr><td>9</td><td>D4 / SDA / GPIO6</td><td>D4 / A4 / SDA / GPIO5</td></tr><tr><td>10</td><td>D3 / A3 / GPIO5</td><td>D3 / A3 / GPIO4</td></tr><tr><td>11</td><td>D10 / MOSI / GPIO10</td><td>D10 / A10 / MOSI / GPIO9</td></tr></tbody></table>

### setup

#### hardware

1. Assembling the node case



2. Attaching the node to the CAN bus



#### software

* ESP board definitions
* Arduino IDE / Platform IO / ESP-IDF
* Installing the node library

### how to use

Find the quick guide in the [overview section](./#quick-installation-guide).

{% hint style="info" %}
You can add up to 70+ nodes onto the CAN bus, however limiting the number of sensors to <15 is recommended (i.e. limit the different messages competing on the CAN bus).
{% endhint %}

#### steps

1. <mark style="color:orange;">Assemble the node</mark> (sensor, computer and case).
2. <mark style="color:orange;">Connect a CAN cable</mark> to one of the JST sockets to add the node on the CAN bus.
3. If the node is added at the end of the bus, <mark style="color:orange;">switch the termination ON.</mark>

{% hint style="info" %}
After the system is ON, <mark style="color:orange;">the node is powered</mark> <mark style="color:orange;">if you see a LED lighting up.</mark>
{% endhint %}

#### checklist



* Connect the node to your laptop through USB.



1. Open Arduino IDE and check the&#x20;
2.

#### troubleshooting



### development

#### node library - to code a node

API reference



examples

* screen node
* voltmeter node

See overview for [pin definition](./#hardware-and-pinouts).

```arduino
void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    delay(1000);  // Wait for serial monitor to initialize

    // Set up the node for CAN communication with specified pins
    // RX pin: GPIO_NUM_4, TX pin: GPIO_NUM_3, Standby pin: 2
    node.begin(GPIO_NUM_4, GPIO_NUM_3, 2);

    // Initialize message containers with specific IDs and data lengths
    node.initializeMessage(10, 8);  // Message ID: 10, Data Length: 8 bytes
    node.initializeMessage(14, 4);  // Message ID: 14, Data Length: 4 bytes

    // Display the data from the message with the highest ID
    node.displayLatestMessageData();

    // Define expected messages with their IDs and variable types
    std::vector<ExpectedMessage> messages = {
        {20, INT32, FLOAT}, // ID: 20, Variables: INT32 and FLOAT
        {24, UINT32, NONE}  // ID: 24, Variable: UINT32
    };

    // Add expected messages to the node's map
    node.addExpectedMessages(messages);
}
```

#### design a new sensor

Electrical design

Code integration

Tips

#### build a computer

Tips on soldering

Parts list (BOM)

schematics







## Receiver

### setup

#### hardware

#### software

Similar to in the [node software setup](in-the-vehicle.md#software).







## 2024 configuration





