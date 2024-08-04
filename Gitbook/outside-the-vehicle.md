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



#### Software - Live Race Engineer Interface

1. Ensure that the telemetry system is turned on. Can be verified by lights on the boards and the driver display should be displaying information if connected.
2. Connect the ground station to your laptop.
3. Open Teleplot and select the port and baudrate (115200 by default).
4. Use the Teleplot interface to verify that data is being received and monitor it.
5. If Teleplot worked, disconnect from it (only one application can access a given serial port at a time) open the MATLAB application and run it.
6. On the top left of the MATLAB application select the port and baudrate (same as Teleplot).
7. If this worked successfully  you should be seeing data being plotted on the plots. Use the lap and quantity toggle switched to adjust the plots.

{% hint style="warning" %}
* Always start with Teleplot.
* The data that is being sent from the ground station to the laptop follows the format specified by Teleplot A:B§C
  * **A** is the name of the telemetry variable (be kind and avoid `:|` special chars in it!).
  * **B** is either the integer or floating point value to be plotted or a text format value to be displayed.
  * **C** is **optional** and is the unit of the telemetry ( please avoid `,;:|.` special chars in it!).
* The MATLAB application will crash if you click on a toggle switch that has no datafeed assigned to it.
* The timer in the MATLAB application currently needs to be started manually.
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

<pre class="language-matlab" data-overflow="wrap"><code class="lang-matlab">        <a data-footnote-ref href="#user-content-fn-1">myTimer %Timer</a>
        Timer % Timer object
        TimerObject % Timer object
        StartTime % Start time
</code></pre>

Function itself

<pre class="language-matlab" data-overflow="wrap"><code class="lang-matlab">        function startupFcn(app)
            % Query available serial ports
            serialInfo = serialportlist("available");
            app.serialPort.Items = serialInfo;
            if isempty(serialInfo)
                app.serialPort.Items = {'No Ports Available'};
                app.serialPort.Enable = 'off';
            else
                app.serialPort.Enable = 'on';
            end
            <a data-footnote-ref href="#user-content-fn-2">app.myTimer = timer('ExecutionMode', 'fixedRate', 'Period', 1, 'TimerFcn', @(src, event)myTimerCallback(app, src, event)); % Specify the callback function</a>
            % Start the timer
            start(app.myTimer);
            app.TimerObject = timer(...
                'ExecutionMode', 'fixedRate', ...
                'Period', 1, ... % Update every second
                'TimerFcn', @(~,~)updateTimer(app));
        end
</code></pre>

</details>

As of now the way the code is setup you need to define a new add value function for each quantity that you want to be plotted and a function to update each of your plots.

<details>

<summary>Adding the new values</summary>

The function works by appending the value you give it to the end of the nth array in the cell array. Where n is the current lap number.

{% code overflow="wrap" %}
```matlab
        function addVelocityValue(app, value)
            lapIndex = app.LapField.Value; % Get the current lap index from the LapField
    
            if length(app.velocity) < lapIndex || isempty(app.velocity{lapIndex})
                % If the current lap's cell doesn't exist, initialize it with the value
                app.velocity{lapIndex} = value;
            else
                % Otherwise, append the value to the existing cell data
                app.velocity{lapIndex} = [app.velocity{lapIndex}, value];
            end
        end
```
{% endcode %}

</details>

<details>

<summary>Plotting the new values</summary>

_Single variable plots_ - For plots like velocity where you are only comparing its value through various laps the following function was created to plot it.

{% code overflow="wrap" %}
```matlab
        function updateVelocityPlot(app)
            cla(app.velUIAxes);  % Clear the current axes
            hold(app.velUIAxes, 'on');
            checkedNodes = app.LapTree.CheckedNodes;  % Get the list of checked nodes

            % Loop over each checked node
            for i = 1:numel(checkedNodes)
                node = checkedNodes(i);  % Get each checked node
                lapIndex = node.NodeData;  % Assuming NodeData stores the corresponding lap index

                % Ensure lapIndex is valid and within bounds
                if isnumeric(lapIndex) && lapIndex > 0 && lapIndex <= length(app.velocity)
                    % Check if the lap data exists and is not empty
                    if ~isempty(app.velocity{lapIndex})
                        plot(app.velUIAxes, app.velocity{lapIndex}, 'DisplayName', sprintf('Lap %d', lapIndex), 'LineWidth',4);
                    end
                end
            end
            hold(app.velUIAxes, 'off');
            legend(app.velUIAxes, 'Location', 'best');
            title(app.velUIAxes, 'Velocity vs Measurements');
            xlabel(app.velUIAxes, 'Measurements');
            ylabel(app.velUIAxes, 'Velocity (m/s)');
        end

```
{% endcode %}

***

_Multivariable plots_ - For plots where multiple quantities are being compared within a lap and over various laps the following function can be used. With the difference compared to the previous function being the switch which checks for which quantities should be plotted for the given lap.

{% code overflow="wrap" %}
```matlab
        function updateCurrentPlot(app)
            cla(app.currentUIAxes);  % Clear the current axes
            hold(app.currentUIAxes, 'on');
            checkedLapNodes = app.LapTree.CheckedNodes;  % Get the list of checked nodes from the LapTree
            checkedCurrentParameters = app.CurrentTree.CheckedNodes;  % Get the list of checked voltage parameters


            % Loop over each checked lap node
            for i = 1:numel(checkedLapNodes)
                lapNode = checkedLapNodes(i);  % Get each checked lap node
                lapIndex = lapNode.NodeData;  % Assuming NodeData stores the corresponding lap index

                % Ensure lapIndex is valid and within bounds
                if isnumeric(lapIndex) && lapIndex > 0 && lapIndex <= length(app.velocity)
                    % Check if the lap data exists and is not empty

                    % Loop over each checked parameter node
                    for j = 1:numel(checkedCurrentParameters)
                        parameterNode = checkedCurrentParameters(j);
                        parameterIndex = parameterNode.NodeData;  % NodeData here should correspond to the parameter index

                        switch parameterIndex
                            case 1  % Supercaps Overall Current
                                if ~isempty(app.superCapsOverallCurrent{lapIndex})
                                    plot(app.currentUIAxes, str2double(app.superCapsOverallCurrent{lapIndex}), 'DisplayName', sprintf('SCO Lap %d', lapIndex), 'LineWidth',4);
                                end
                            case 2  % Supercaps Charge Current
                                if ~isempty(app.superCapsChargeCurrent{lapIndex})
                                    plot(app.currentUIAxes, str2double(app.superCapsChargeCurrent{lapIndex}), 'DisplayName', sprintf('SCC Lap %d', lapIndex), 'LineWidth',4);
                                end
                            case 3  % Before Motor Control Current
                                if ~isempty(app.beforeMotorControlCurrent{lapIndex})
                                    plot(app.currentUIAxes, str2double(app.beforeMotorControlCurrent{lapIndex}), 'DisplayName', sprintf('BMC Lap %d', lapIndex), 'LineWidth',4);
                                end
                            % case 4  % DC DC Current
                            %     if ~isempty(app.DCDCCurrent{lapIndex})
                            %         plot(app.currentUIAxes, str2double(app.DCDCCurrent{lapIndex}), 'DisplayName', sprintf('DCDC Lap %d', lapIndex), 'LineWidth',4);
                            %     end
                        end
                    end
                end
            end
            hold(app.currentUIAxes, 'off');
            legend(app.currentUIAxes, 'Location', 'best');
            title(app.currentUIAxes, 'Current vs Measurements');
            xlabel(app.currentUIAxes, 'Measurements');
            ylabel(app.currentUIAxes, 'Current (A)');
        end
```
{% endcode %}

</details>

#### Putting it all together

{% hint style="info" %}
You can click on the underlined code to reveal information about its use.
{% endhint %}

<pre class="language-matlab" data-overflow="wrap" data-line-numbers data-full-width="true"><code class="lang-matlab"><a data-footnote-ref href="#user-content-fn-3">function myTimerCallback(app, src, event)</a>
            <a data-footnote-ref href="#user-content-fn-4">if (src.NumBytesAvailable > 0)</a>
                        <a data-footnote-ref href="#user-content-fn-5">dataStr = readline(src);  % Reads data from the serial port until a newline is received</a>
                        % Remove non-ASCII characters except '§'
                        <a data-footnote-ref href="#user-content-fn-6">dataStr = regexprep(dataStr, '[\s>]|[^\x20-\x7E§]', '');</a>
                        <a data-footnote-ref href="#user-content-fn-7">try</a>
                            % Split the data into its components
                            <a data-footnote-ref href="#user-content-fn-8">splitData = strsplit(dataStr, '§');  % Split based on the '§' delimiter</a>
                            <a data-footnote-ref href="#user-content-fn-9">if numel(splitData) == 2</a>
                                <a data-footnote-ref href="#user-content-fn-10">varNameValue = splitData{1};</a>
                                %varUnit = splitData{2};

                                % Further split to separate variable name and value
                                <a data-footnote-ref href="#user-content-fn-11">nameValueSplit = strsplit(varNameValue, ':');</a>
                                    <a data-footnote-ref href="#user-content-fn-12">if numel(nameValueSplit) == 2</a>
                                        <a data-footnote-ref href="#user-content-fn-13">varName = nameValueSplit{1};</a>
                                        <a data-footnote-ref href="#user-content-fn-14">varValue = str2double(nameValueSplit{2});  % Convert value to double</a>

                                        <a data-footnote-ref href="#user-content-fn-15">if strcmp(varName, 'Lap#')</a>
                                            <a data-footnote-ref href="#user-content-fn-16">app.LapField.Value = varValue;</a>
                                            <a data-footnote-ref href="#user-content-fn-17">handleNewLap(app);</a>

                                        elseif strcmp(varName, 'Velocity')
                                            %disp('This is a velocity reading');
                                            <a data-footnote-ref href="#user-content-fn-18">addVelocityValue(app, varValue);</a>
                                            <a data-footnote-ref href="#user-content-fn-19">updateVelocityPlot(app);</a>

                                        %Supercaps Overall voltage update
                                        elseif strcmp(varName, 'SCOV')
                                            <a data-footnote-ref href="#user-content-fn-20">addSCOVoltageValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-21">updateVoltagePlot(app);</a>

                                        %before motor control voltage update
                                        elseif strcmp(varName, 'BMCV')
                                            <a data-footnote-ref href="#user-content-fn-22">addBMCVoltageValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-23">updateVoltagePlot(app);</a>

                                        %DC DC voltage update
                                        elseif strcmp(varName, 'DCDCV')
                                            <a data-footnote-ref href="#user-content-fn-24">addDCDCVoltageValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-25">updateVoltagePlot(app);</a>

                                        %Supercaps Overall current update
                                        elseif strcmp(varName, 'Curr1')
                                            <a data-footnote-ref href="#user-content-fn-26">addSCOCurrentValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-27">updateCurrentPlot(app);</a>

                                        %Supercaps Charge current update
                                        elseif strcmp(varName, 'Curr2')
                                            <a data-footnote-ref href="#user-content-fn-28">addSCCCurrentValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-29">updateCurrentPlot(app);</a>

                                        %Before Motor Control Current
                                        %update
                                        elseif strcmp(varName, 'Curr3')
                                            <a data-footnote-ref href="#user-content-fn-30">addBMCCurrentValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-31">updateCurrentPlot(app);</a>

                                        %DC DC Current update
                                        elseif strcmp(varName, 'DCDCC')
                                            <a data-footnote-ref href="#user-content-fn-32">addDCDCCurrentValue(app, string(varValue));</a>
                                            <a data-footnote-ref href="#user-content-fn-33">updateCurrentPlot(app);</a>
                                        end
                                    else
                                        disp('Error: Variable name and value not in expected format.');
                                    end
                            else
                                    disp('Error: Data not in expected format.');
                            end
                        catch
                            disp(['Error processing data: ', dataStr]);
                        end
            end
        end
</code></pre>

### Development



#### Future Work - Live Race Engineer Interface

The interface can be improved upon by creating a sophisticated database structure and querying the data from it. This would allow for any kind of comparison plot to be created. Furthermore, instead of sending the measurements from the vehicle to the ground station they could be uploaded to an online database from which the readings can be accessed by any number of race engineers and from anywhere, allowing more people to do various kinds of data analysis simultaneously.





## Off-track data analysis





[^1]: A

[^2]: A

[^3]: Timer function callback, allows the plots to be continuously updated.

[^4]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^5]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^6]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^7]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^8]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^9]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^10]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^11]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^12]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^13]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^14]: Converts the Teleplot formatted measurement into VarName and VarValue.

[^15]: Checks whether VarName is “Lap#” and if so increments to lap number.

[^16]: Checks whether VarName is “Lap#” and if so increments to lap number.

[^17]: Checks whether VarName is “Lap#” and if so increments to lap number.

[^18]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^19]: Updates the plots.

[^20]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^21]: Updates the plots.

[^22]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^23]: Updates the plots.

[^24]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^25]: Updates the plots.

[^26]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^27]: Updates the plots.

[^28]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^29]: Updates the plots.

[^30]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^31]: Updates the plots.

[^32]: Compares the VarName with the a predefined variable name and stores it VarValue to the corresponding cell array.

[^33]: Updates the plots.
