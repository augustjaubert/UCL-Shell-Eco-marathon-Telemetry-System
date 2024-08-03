# Outside the vehicle

## Radio Station

### Setup

#### Hardware

assembling the ground station

#### Software

Before using the interface two applications need to be installed:

1\.        MATLAB for using the custom app.

2\.        VSCode (or any IDE) and the [Teleplot](https://github.com/nesnes/teleplot) extension installed for troubleshooting and redundancy.

### How to use

#### Hardware



#### Software

1. Ensure that the telemetry system is turned on. Can be verified by lights on the boards and the driver display should be displaying information if connected.
2. Connect the ground station to your laptop.
3. Open Teleplot and select the port and baudrate (115200 by default).
4. Use the Teleplot interface to verify that data is being received and monitor it.
5. If Teleplot worked, disconnect from it (only one application can access a given serial port at a time) open the MATLAB application and run it.
6. On the top left of the MATLAB application select the port and baudrate (same as Teleplot).
7. If this worked successfully  you should be seeing data being plotted on the plots. Use the lap and quantity toggle switched to adjust the plots.

{% hint style="info" %}
* Always start with Teleplot.
* The data that is being sent from the ground station to the laptop follows the format specified by Teleplot A:B§C
  * **A** is the name of the telemetry variable (be kind and avoid `:|` special chars in it!).
  * **B** is either the integer or floating point value to be plotted or a text format value to be displayed.
  * **C** is **optional** and is the unit of the telemetry ( please avoid `,;:|.` special chars in it!).
* The MATLAB application will crash if you click on a toggle switch that has no datafeed assigned to it.
* The timer in the MATLAB application needs to be started manually.
{% endhint %}









## Off-track data analysis



