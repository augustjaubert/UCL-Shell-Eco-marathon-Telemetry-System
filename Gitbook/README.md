# Overview

This user manual provides the information necessary for students to use the telemetry system.

<figure><img src=".gitbook/assets/Asset 1@4x (1).png" alt="" width="563"><figcaption></figcaption></figure>

### Features

This system was developed for usage on a race vehicle, to collect key performance indices. These vary depending on the vehicle competition, and this guide is directed towards the use on a vehicle participating in the Shell Eco Marathon competition.

It has,

* Expandable plug-in sensor capabilities
* CAN bus communication
* Data logging
* Live radio telemetry
* Driver & Race engineer graphical user interface

The system comprises of sensors, a receiver, and a radio station.

The sensors are configured as nodes on the CAN bus, meaning they each are paired with a small computer board. The latter acts as the middleman between the sensor’s interfacing protocol (i.e. the language the sensor talks in) and the CAN bus (i.e. what the telemetry system talks in).

The receiver is a computer board, who’s only function is to receive those sensor messages, store them onto a microSD card and handle the radio.

Notice that the receiver does not take care of the driver display. This is because depending on the setups, they may be at the complete opposite ends of a vehicle, so running a long cable from one to the other is not ideal. Instead, the system was designed so that any interfacing element (e.g. display, servo motor, etc.) can be attached to the CAN bus via a node and from there communicate with the system however it was programmed to.

The radio station is connected by USB to a computer, and all it does is intercept data over radio from the vehicle’s receiver, parse and translate that information, and then the computer program takes care of nicely displaying it all.

### Live race engineer interface

The live race engineer interface allows for the race engineer to assess the vehicle’s and driver’s performance during testing and at the race. This analysis is done via the use of plots. Currently there are three plots showing velocity, voltage, and current (these plots were meant to be used with a hydrogen powertrain). Besides the plots, there are toggle switches allowing for the comparison of various measured data (eg. Super-capacitor voltage and DC-DC voltage) and various laps.

The how to use section describes how the already existing application can be used for data analysis. While the how to edit/update section details how the application was setup and how to modify it or update it for future applications. Finally, the future work section mentions improvements that can be made.
