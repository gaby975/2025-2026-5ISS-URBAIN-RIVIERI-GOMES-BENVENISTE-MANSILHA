#include <rn2xx3.h>

// --- Choix des pins ESP32 ---
static const int LORA_RX_PIN = 16;   // ESP32 reçoit (RX2)  <- TX du module Lora
static const int LORA_TX_PIN = 17;   // ESP32 émet   (TX2)  -> RX du module Lora
static const int LORA_RST_PIN = 4;   // Reset module Lora
static const int LED_PIN = 2;        // LED intégrée
static const int BUZ_PIN = 23;       // Buzzer du shield
static const int BUT_PIN = 22;       // Bouton du shield
static const int GAS_PIN = 36;       // Capteur AIME
static const int TEMP_PIN = 2;       // Capteur AIME

rn2xx3 myLora(Serial2);
int lastSend = 0;
const float R1 = 100000;
const float R3 = 1000;
const float R2 = 100000;
const float R5 = 10000;
const float GAIN = 1+(R2/R3);
const float VCC = 3.3; 

void led_on()  { digitalWrite(LED_PIN, HIGH); }
void led_off() { digitalWrite(LED_PIN, LOW);  }
void buz_on()  { digitalWrite(BUZ_PIN, HIGH);  }
void buz_off() { digitalWrite(BUZ_PIN, LOW);  }

void initialize_radio()
{
  pinMode(LORA_RST_PIN, OUTPUT);
  digitalWrite(LORA_RST_PIN, LOW);
  delay(500);
  digitalWrite(LORA_RST_PIN, HIGH);

  delay(100);
  while (Serial2.available()) Serial2.read();

  // Comme code d'origine (module souvent en 57600 par défaut)
  myLora.autobaud();

  String hweui = myLora.hweui();
  while (hweui.length() != 16) {
    Serial.println("Communication RN2xx3 KO. Verifie cablage/alimentation.");
    Serial.println(hweui);
    delay(5000);
    hweui = myLora.hweui();
  }

  Serial.print("DevEUI (OTAA) :");
  Serial.println(myLora.hweui());
  Serial.print("Firmware :");
  Serial.println(myLora.sysver());

  Serial.println("Trying to join TTN (OTAA) ...");

  // --- OTAA (ATTENTION clés en clair ici) ---
  const char *appEui = "70B3D57ED00001A6";
  const char *appKey = "e31cda50a5c44300005ab3b765549a99";

  bool join_result = myLora.initOTAA(appEui, appKey);

  while (!join_result) {
    Serial.println("Join OTAA impossible. Verifie clés / couverture.");
    delay(60000);
    join_result = myLora.initOTAA(appEui, appKey);
  }
  Serial.println("Successfully joined TTN");
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZ_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT);
  led_on();

  Serial.begin(115200); // Console PC

  // UART2: (baud, config, RX, TX)
  Serial2.begin(9600, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);

  Serial.println("Startup ESP32 + RN2xx3");
  initialize_radio();

  myLora.tx("TTN Mapper ESP32 node");
  led_off();
  delay(2000);
}

void loop()
{
  // MESURES BOUTON + GAS + TEMPERATURE
  int buttun = digitalRead(BUT_PIN);
  float gas_value = analogRead(GAS_PIN);
  Serial.println(gas_value);
  int temp_value = analogRead(TEMP_PIN);
  gas_value = gas_value*3.3/4095;
  Serial.println(gas_value);

  // CALCULS DES VALEURS
  float resistance = 0;
  if (gas_value != 0){ resistance = R1*GAIN*(VCC/gas_value)-R5-R1; }
  
  Serial.println(resistance);
  float ppm_value = resistance/1396;
  Serial.println(ppm_value);

  Serial.println(buttun);

  // ENVOIE TOUT LES 5 SECONDES
  if (buttun || (millis()-lastSend)>5000){
    lastSend = millis();

    led_on();

    Serial.print("## Envoi en cours : ");
    Serial.println(ppm_value);
    TX_RETURN_TYPE r = myLora.tx(String(ppm_value));
    Serial.print("TX result: ");
    if ((int)r == 1){
      Serial.println("SUCCESS");
    }else{
      Serial.println("FAIL");
    }
    
    led_off();

    buz_on();
    delay(200);
    buz_off();
    delay(1000);

  }else{
    buz_off();
  }
  delay(10);
}
