# Node-RED MQTT (ChirpStack)

Ce flow Node-RED écoute un topic MQTT (uplinks ChirpStack), **extrait le champ `data`**, le **décode du Base64** puis le **convertit en nombre base 10**. La valeur est affichée sur un **dashboard** (jauge + historique). Une alerte **“DANGER!”** (texte + audio) est déclenchée au-dessus d’un seuil. 

## Fonctionnalités

- **MQTT In**
  - Broker : `srv-chirpstack.insa-toulouse.fr:1883`
  - Topic : `application/2fc05488-1aaa-49cf-a35a-cf58fae84647/device/0004a30b0110c16d/event/#`
  - QoS : `2` 
- **Décodage**
  - Extraction de `payload.data`
  - `Base64 -> UTF-8 -> Number (base 10)` 
- **Dashboard**
  - Jauge `Actual_Sensor_Value` (0 → 1000, zones vert/jaune/rouge)
  - Courbe `Sensor_value_History` (historique ~1h)
- **Alerte**
  - Si `valeur >= 900` → `DANGER!` + audio + texte 

## Prérequis

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

