# In the vehicle

## Node

The node computer hardware diagram is presented in the figure below.

<figure><img src=".gitbook/assets/node 1_1@4x.png" alt=""><figcaption></figcaption></figure>



### node setup

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





