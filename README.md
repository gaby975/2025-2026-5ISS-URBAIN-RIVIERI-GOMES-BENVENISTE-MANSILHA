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

- a passive filter F1 at the input to prevent HF noise from causing distortion in the input stages: R0, C1, and R1.
- an active filter F2 at the operational amplifier level : R3 and C4.
- a passive filter F3 at the output of the circuit to remove noise introduced during processing (power supply noise, clock noise) : C2 and R6.

![ltspice_filter_table](images/ltspice_fc_table.PNG)

Using an LTspice simulation, we were able to determine the frequency response (frequency gain) of each filter and also of the entire circuit. The Bode diagram of the active filter is as follows :

![ltspice_fc2](images/ltspice_trans_impedance_amplifier_fc2.png)

The Bode diagram of the passive filter at the output of the circuit is as follows :

![ltspice_fc3](images/ltspice_trans_impedance_amplifier_fc3.png)

Finally, we find the following frequency response for the complete setup (gain and phase):

![ltspice_fullbode](images/ltspice_trans_impedance_amplifier_bode.png)

## PCB design on KiCad

## Fabrication of the Shield

## Embedded code

This program (Arduino/ESP32) reads a **gas sensor** on an analog input, computes an estimated value in **PPM**, then sends it to **The Things Network (TTN)** via an **RN2xx3** module using **LoRaWAN OTAA**.
Transmission happens **every 5 seconds** or **when the button is pressed**. An **LED** indicates sending, and a **buzzer** beeps on each transmission.

---

### Required hardware

* ESP32
* RN2xx3 LoRa module (RN2483 / RN2903) connected via UART
* Gas sensor (analog output)
* Button (digital input)
* Buzzer (digital output)
* Built-in LED (or external LED)

---

### Pinout (ESP32 ↔ RN2xx3 / Sensors / I/O)

#### LoRa RN2xx3 (UART2 / Serial2)

| Function   | ESP32 GPIO | Direction      | RN2xx3         |
| ---------- | ---------: | -------------- | -------------- |
| UART2 RX   | **GPIO16** | ESP32 receives | RN2xx3 **TX**  |
| UART2 TX   | **GPIO17** | ESP32 sends    | RN2xx3 **RX**  |
| LoRa Reset |  **GPIO4** | output         | RN2xx3 **RST** |
| Power      |   **3.3V** | —              | VCC            |
| Ground     |    **GND** | —              | GND            |

---

#### Sensors / Button / Buzzer / LED

| Element                 | ESP32 GPIO | Type   | Note                                                   |
| ----------------------- | ---------: | ------ | ------------------------------------------------------ |
| Gas sensor (analog)     | **GPIO36** | ADC1   | `analogRead(GAS_PIN)`                                  |
| Button                  | **GPIO22** | input  | in your code: `INPUT` (often better as `INPUT_PULLUP`) |
| Buzzer                  | **GPIO23** | output | beep on each send                                      |
| LED (built-in)          |  **GPIO2** | output | LED on during TX                                       |
| “Temperature” (current) |  **GPIO2** | ❌      | **Conflict** with LED + **not ADC**                    |

---

### Dependencies

#### Arduino library

* `rn2xx3`

Include in the code:

```cpp
#include <rn2xx3.h>
```

## Sending data with LoRa

## Node-RED

The Node-RED flow listens to an MQTT topic (ChirpStack uplinks), **extracts the `data` field**, **decodes it from Base64**, then **converts it to a base-10 number**. The value is displayed on a **dashboard** (gauge + history). A **“DANGER!”** alert (text + audio) is triggered above a threshold.

### Features

* **MQTT In**

  * Broker: `srv-chirpstack.insa-toulouse.fr:1883`
  * Topic: `application/2fc05488-1aaa-49cf-a35a-cf58fae84647/device/0004a30b0110c16d/event/up`
  * QoS: `2`
* **Decoding**

  * Extract `payload.data`
  * `Base64 -> UTF-8 -> Number (base 10)`
* **Dashboard**

  * Gauge `Actual_Sensor_Value` (0 → 8000, green/yellow/red zones)
  * Chart `Sensor_value_History` (~1h history)
* **Alert**

  * If `value >= 6000` → `DANGER!` + audio + text

### Prerequisites

* Node-RED installed
* Network access to the MQTT broker
* Node-RED modules:

  * `node-red-dashboard` (the flow indicates config **3.6.6**)
  * The `ui_audio` node may require an additional module depending on your setup (often `node-red-contrib-ui-audio`)

Example installation:

```bash
cd ~/.node-red
npm install node-red-dashboard
# if needed (if ui_audio is "unknown node type")
npm install node-red-contrib-ui-audio
```


## Conclusion


