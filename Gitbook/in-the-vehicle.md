# In the vehicle

## Node

### Setup

#### Hardware

* Assembling the node case
* Attaching the node to the CAN bus

#### Software

* ESP board definitions
* Arduino IDE / Platform IO / ESP-IDF
* Installing the node library

### How to use

Find the quick guide in the [overview section](./#quick-installation-guide).

{% hint style="info" %}
You can add up to 70+ nodes onto the CAN bus, however limiting the number of sensors to <15 is recommended (i.e. limit the different messages competing on the CAN bus).
{% endhint %}

#### Steps

1. <mark style="color:orange;">Assemble the node</mark> (sensor, computer and case).
2. <mark style="color:orange;">Connect a CAN cable</mark> to one of the JST sockets to add the node on the CAN bus.
3. If the node is added at the end of the bus, <mark style="color:orange;">switch the termination ON</mark>.

{% hint style="info" %}
After the system is ON (receiver switch ON), <mark style="color:orange;">check that the node is powered</mark> if you see a <mark style="color:orange;">LED lighting up</mark>.
{% endhint %}

#### Checklist



* Connect the node to your laptop through USB.



1. Open Arduino IDE and check the&#x20;
2.

#### Troubleshooting













## Receiver

### Setup

#### Hardware

#### Software

Similar to in the [node software setup](in-the-vehicle.md#software).







## 2024 configuration





