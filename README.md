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



## PCB design on KiCad

## Fabrication of the Shield

## Embedded code

Ce programme (Arduino/ESP32) lit un **capteur de gaz** sur une entrée analogique, calcule une estimation en **PPM**, puis l’envoie sur **The Things Network (TTN)** via un module **RN2xx3** en **LoRaWAN OTAA**.  
L’envoi se fait **toutes les 5 secondes** ou **sur appui du bouton**. Une **LED** indique l’envoi et un **buzzer** bippe à chaque transmission.

---

### Matériel requis

- ESP32
- Module LoRa RN2xx3 (RN2483 / RN2903) connecté en UART
- Capteur de gaz (sortie analogique)
- Bouton (entrée digitale)
- Buzzer (sortie digitale)
- LED intégrée (ou LED externe)

---

### Pinout (ESP32 ↔ RN2xx3 / Capteurs / I/O)

#### LoRa RN2xx3 (UART2 / Serial2)

| Fonction     | ESP32 GPIO | Sens         | RN2xx3            |
| ------------ | ---------: | ------------ | ----------------- |
| UART2 RX     | **GPIO16** | ESP32 reçoit | **TX** du RN2xx3  |
| UART2 TX     | **GPIO17** | ESP32 émet   | **RX** du RN2xx3  |
| Reset LoRa   |  **GPIO4** | sortie       | **RST** du RN2xx3 |
| Alimentation |   **3.3V** | —            | VCC               |
| Masse        |    **GND** | —            | GND               |

---

#### Capteurs / Bouton / Buzzer / LED

| Élément                | ESP32 GPIO | Type   | Remarque                                                  |
| ---------------------- | ---------: | ------ | --------------------------------------------------------- |
| Capteur Gaz (analog)   | **GPIO36** | ADC1   | `analogRead(GAS_PIN)`                                     |
| Bouton                 | **GPIO22** | entrée | dans ton code : `INPUT` (souvent mieux en `INPUT_PULLUP`) |
| Buzzer                 | **GPIO23** | sortie | bip à chaque envoi                                        |
| LED (intégrée)         |  **GPIO2** | sortie | LED pendant TX                                            |
| “Température” (actuel) |  **GPIO2** | ❌      | **Conflit** avec LED + **pas ADC**                        |

---

### Dépendances

#### Librairie Arduino

- `rn2xx3`

Import dans le code :

```cpp
#include <rn2xx3.h>
```

## Sending data with LoRa

## NodeRed

Le flow Node-RED écoute un topic MQTT (uplinks ChirpStack), **extrait le champ `data`**, le **décode du Base64** puis le **convertit en nombre base 10**. La valeur est affichée sur un **dashboard** (jauge + historique). Une alerte **“DANGER!”** (texte + audio) est déclenchée au-dessus d’un seuil. 

### Fonctionnalités

- **MQTT In**
  - Broker : `srv-chirpstack.insa-toulouse.fr:1883`
  - Topic : `application/2fc05488-1aaa-49cf-a35a-cf58fae84647/device/0004a30b0110c16d/event/up`
  - QoS : `2` 
- **Décodage**
  - Extraction de `payload.data`
  - `Base64 -> UTF-8 -> Number (base 10)` 
- **Dashboard**
  - Jauge `Actual_Sensor_Value` (0 → 8000, zones vert/jaune/rouge)
  - Courbe `Sensor_value_History` (historique ~1h)
- **Alerte**
  - Si `valeur >= 6000` → `DANGER!` + audio + texte 

### Prérequis

- Node-RED installé
- Accès réseau au broker MQTT
- Modules Node-RED :
  - `node-red-dashboard` (le flow indique une config en **3.6.6**) 
  - Le node `ui_audio` peut nécessiter un module additionnel selon ton install (souvent `node-red-contrib-ui-audio`). 

Installation (exemple) :

```bash
cd ~/.node-red
npm install node-red-dashboard
# si besoin (si ui_audio est "unknown node type")
npm install node-red-contrib-ui-audio
```

## Conclusion


