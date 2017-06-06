# ArduinoSensorKit
Collection of code for Arduino sensors that can be used to measure indoor climate.

Mainly uses the following sensors:
DHT_11 (Temperature and Humidity)
T18B20 (Temperature)
Small/Big Sound
SW-420 (Shock)
KY-002 (Shock)

The code is neither giving precise nor complete results, due to the nature of the sensors.
This is just an example to show how multiple sensors can be combined in a single program to measures indoor climate values like temperature, humidity, light, noise and vibrations.

To achieve more reliable measurements, sensors with a higher quality should be used.

The src folder contains code for running each sensor on its own for testing and also the SensorKit that combines these sensors in a single program.

This repository contains many more sensor libraries that might be useful when implementing a new project.
