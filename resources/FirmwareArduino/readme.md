# ESP32 + RN2xx3 (LoRaWAN OTAA) — Capteur Gaz, Calcul PPM, Envoi TTN

Ce programme (Arduino/ESP32) lit un **capteur de gaz** sur une entrée analogique, calcule une estimation en **PPM**, puis l’envoie sur **The Things Network (TTN)** via un module **RN2xx3** en **LoRaWAN OTAA**.  
L’envoi se fait **toutes les 5 secondes** ou **sur appui du bouton**. Une **LED** indique l’envoi et un **buzzer** bippe à chaque transmission.

---

## Matériel requis

- ESP32
- Module LoRa RN2xx3 (RN2483 / RN2903) connecté en UART
- Capteur de gaz (sortie analogique)
- Bouton (entrée digitale)
- Buzzer (sortie digitale)
- LED intégrée (ou LED externe)

---
#Bien sûr — voici un bloc **Pinout** prêt à coller dans ton README (Markdown), basé sur **TON code actuel** (avec la remarque sur le conflit `GPIO2`).

---

## Pinout (ESP32 ↔ RN2xx3 / Capteurs / I/O)

### LoRa RN2xx3 (UART2 / Serial2)

| Fonction     | ESP32 GPIO | Sens         | RN2xx3            |
| ------------ | ---------: | ------------ | ----------------- |
| UART2 RX     | **GPIO16** | ESP32 reçoit | **TX** du RN2xx3  |
| UART2 TX     | **GPIO17** | ESP32 émet   | **RX** du RN2xx3  |
| Reset LoRa   |  **GPIO4** | sortie       | **RST** du RN2xx3 |
| Alimentation |   **3.3V** | —            | VCC               |
| Masse        |    **GND** | —            | GND               |

---

### Capteurs / Bouton / Buzzer / LED

| Élément                | ESP32 GPIO | Type   | Remarque                                                  |
| ---------------------- | ---------: | ------ | --------------------------------------------------------- |
| Capteur Gaz (analog)   | **GPIO36** | ADC1   | `analogRead(GAS_PIN)`                                     |
| Bouton                 | **GPIO22** | entrée | dans ton code : `INPUT` (souvent mieux en `INPUT_PULLUP`) |
| Buzzer                 | **GPIO23** | sortie | bip à chaque envoi                                        |
| LED (intégrée)         |  **GPIO2** | sortie | LED pendant TX                                            |
| “Température” (actuel) |  **GPIO2** | ❌      | **Conflit** avec LED + **pas ADC**                        |

---

## Dépendances

### Librairie Arduino

- `rn2xx3`

Import dans le code :

```cpp
#include <rn2xx3.h>
