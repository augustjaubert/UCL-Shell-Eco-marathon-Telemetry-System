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

### Development

#### How to edit/update

The MATLAB application works by taking reading the next message from the serial port which contains a single measurement in the Teleplot format, identifying its quantity (eg. Velocity, Super-capacitor voltage), storing it to it’s corresponding cell array (an array of arrays more detail on it later), and updating the plots accordingly with the new data while also checking which checkbox is checked.

{% hint style="info" %}
**Why cell arrays?**

Cell arrays were used to allow for various measurements per lap to be taken and stored in a dynamic structure. Where the index of the cell array corresponds to the lap number. Allowing to compare various laps measurements with each other based on measurement number. Meaning that you can only compare for example the 10th velocity measurement of lap 3 with lap 5 and not the velocity at 100 m from the start line of these laps. This is a big negative of the current version of the interface, but assuming that the vehicle has consistent lap times then this approach is acceptable. How to improve this is detailed in the future work.
{% endhint %}

#### Main bits of the code

Whenever calling a function in the MATLAB app environmnent the first value you need to pass into it is always “app”.

<details>

<summary>Cell arrays</summary>

This is where the readings will be stored as discussed previously.

{% code overflow="wrap" %}
```matlab
velocity = {}; %Cell array to store velocity readings of each lap
current = {}; %Cell array to store current readings of each lap
voltage = {}; %Cell array to store voltage readings of each lap
%Voltage values
superCapsOverallVoltage = {}; %Cell array to store the supercaps overall voltage readings of each lap
beforeMotorControlVoltage = {}; %Cell array to store the before motor control voltage readings of each lap
DCDCVoltage = {}; %Cell array to store the DC DC voltage readings of each lap
```
{% endcode %}

</details>

<details>

<summary>Start-up function</summary>

This is the function that allows for the serial port readings to be continuously (= every 1 ms) monitored and updated (highlighted in yellow), ultimately enabling the plots to be updated. The other stuff named timer are used to update the stopwatch.

Define these in `properties (Access = private)`

{% code overflow="wrap" %}
```matlab
        myTimer %Timer
        Timer % Timer object
        TimerObject % Timer object
        StartTime % Start time
```
{% endcode %}

Function itself

{% code overflow="wrap" %}
```matlab
        function startupFcn(app)
            % Query available serial ports
            serialInfo = serialportlist("available");
            app.serialPort.Items = serialInfo;
            if isempty(serialInfo)
                app.serialPort.Items = {'No Ports Available'};
                app.serialPort.Enable = 'off';
            else
                app.serialPort.Enable = 'on';
            end
            app.myTimer = timer('ExecutionMode', 'fixedRate', 'Period', 1, 'TimerFcn', @(src, event)myTimerCallback(app, src, event)); % Specify the callback function
            % Start the timer
            start(app.myTimer);
            app.TimerObject = timer(...
                'ExecutionMode', 'fixedRate', ...
                'Period', 1, ... % Update every second
                'TimerFcn', @(~,~)updateTimer(app));
        end
```
{% endcode %}

</details>





## Off-track data analysis



