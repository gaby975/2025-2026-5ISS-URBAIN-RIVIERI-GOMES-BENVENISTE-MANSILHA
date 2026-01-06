# 2025-2026-5ISS-URBAIN-RIVIERI-GOMES-BENVENISTE-MANSILHA
## Introduction
The objective of this project is to develop an electronic card on which we have integrated a gas sensor manufactured in the clean room at AIME (Inter-University Micro-Nano Electronics Laboratory). The electronic board includes an analog circuit for conditioning the sensor signal, a microcontroller (ESP32) for receiving the conditioned sensor signal and communicating with the other components on the board, a LoRa module for sending sensor data over a LoRa network using Chirpstack, and a buzzer to alert when a gas concentration threshold has been exceeded.

## Gas sensor fabrication
The first step in this project is to manufacture a gas sensor. This sensor was manufactured in a clean room at the AIME (Interuniversity Micro-Nano Electronics Workshop) located on the INSA Toulouse campus. It is a semiconductor-type metal oxide gas sensor based on WO₃ nanoparticles and capable of detecting reducing gases such as ammonia (NH₃) and ethanol vapors.

The sensor is composed of several elements that are worth detailing. First, there are interdigitated aluminum combs on which WO₃ nanoparticles have been deposited and organized by dielectrophoresis. Next, there is an n-doped polysilicon heating resistor, which raises the temperature inside the sensor, improving the surface reaction between the WO₃ nanoparticles and the gas. Finally, there is an aluminum thermistor that measures the temperature inside the sensor.

![gas sensor](images/sensor-schematic.png)

The manufacturing process for this sensor involves a large number of steps, but the most important ones are the photolithography steps. Photolithography enables the formation of the different layers required for the sensor to function. We start with a Si/SiO₂ substrate onto which a layer of polysilicon is deposited using the Low Pressure Chemical Vapor Deposition (LPCVD) technique. The polysilicon is n-doped by diffusing phosphorus (P) into it. An initial photolithography step is used to obtain the correct pattern for the polysilicon layer. A second photolithography step is then used to create the SiO₂ insulation layer with openings. Finally, a last photolithography step is used to create the electrodes from a deposited aluminum layer.

![gas sensor process](images/sensor-process.png)

## Simulation on LTspice
As part of this project, one part was dedicated to designing a stage for conditioning/amplifying the signal delivered by the gas sensor. This signal, which is processed by the ESP32-WROOM microcontroller ADC, must be readable and processable by the latter. To do this, the conditioning stage was designed around the component LTC1050, which was used because It has a really low input voltage offset as well as a good common-mode tolerance and relatively low voltage drift maintain the integrity and accuracy of the 10mV voltage measured at the terminals of R1. This is the schematic we get on LTSpice:

![ltspice_trans_impedance_amplifier](images/ltspice_trans_impedance_amplifier.png)

To ensure good signal quality, several filters have been installed. A bandwidth of 1 Hz is sufficient to extract useful information from the sensor. Three filters have been used: 

- passive filter at the input to prevent HF noise from causing distortion in the input stages: R0, C1, and R1.



![ltspice_filter_table](images/ltspice_fc_table.png)




## PCB design on KiCad

## Fabrication of the Shield

## Embedded code

## Sending data with LoRa

## NodeRed

## Conclusion


