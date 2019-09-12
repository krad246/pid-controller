pid-controller
==============

This project was designed by **Powell Rangers** for their Capstone project for 2018. This is a PID controller meant for local temperature regulation applications. 

## Purpose

Traditional heating applications use high wattage incandescent bulbs, heat lamps, etc. These lamps tend to die out very quickly, and the power consumption is rather wasteful because of large inrush currents affecting the bulb and continuous operation at maximum power even when it is not necessary. This project aims to solve that problem by creating an affordable PID mechanism for the lamp, thus saving farmers (or other users) money.

Furthermore, this project aims to bring the IoT spin to the project for full automation. The controller will have internet connectivity and will interface with an external app. The motivation behind this is to make the controller a hands-free solution, where a user simply sets their desired temperature and can simply adjust it wherever and whenever they please.

## Algorithm

The project will be doing a number of things:

1. Reading data from a temperature probe via the SPI protocol.
2. Continuously updating the state of a PID algorithm at a fixed interval.
3. Pulse width modulating a triac carrying 120 VAC at some fraction of 60 Hz.
4. Using the PID data to adjust the pulse width.
5. Continuously updating an LCD screen via I<sup>2</sup>C to display the set point, etc.
6. Accepting input from a pushbutton rotary encoder to adjust the set point.
7. Transmitting data to the phone app over WiFi, via some form of socket.

## Technology

The project uses the TI CC3220SF LaunchPad to facilitate internet connectivity, provide the processing power for the PID control algorithm, and provide the hardware for the PWM switching technique we are using for PID regulation.
