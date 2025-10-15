#include <Arduino.h>
#include <SD.h>
#include <MKRWAN.h>
#include "Lora.hpp"
#include "Waiter.hpp"
#include <queue>
#include <vector>

// ----- Structures -----
struct ConfigRelais {
    //String appEui;
    //String appKey;
    int CSPin;
    int lora_freq;
    int lora_intervalle_secondes;
};

// ----- Variables globales -----
ConfigRelais config = {5, 868E6, 10800}; // valeurs par défaut
LoRaModem modem;
LoraCommunication lora(868E6, 0xAA, 0xFF);
std::queue<String> sendingQueue;

unsigned long lastLoraSend = 0;

// ----- Lecture CSV -----
void lireConfigCSV(const char* NomFichier) {
    if (!SD.begin(config.CSPin)) {
        Serial.println("Impossible de monter SD");
        return;
    }

    File f = SD.open(NomFichier);
    if (!f) {
        Serial.println("Fichier CSV non trouvé, utilisation des valeurs par défaut");
        return;
    }

    while (f.available()) {
        String line = f.readStringUntil('\n');
        line.trim();
        if (line.length() == 0 || line.startsWith("#")) continue;

        int idx = line.indexOf(',');
        if (idx < 0) continue;
        String key = line.substring(0, idx);
        String val = line.substring(idx + 1);

        //if (key == "appEui") config.appEui = val;
        //else if (key == "appKey") config.appKey = val;
        if (key == "CSPin") config.CSPin = val.toInt();
        else if (key == "lora_freq") config.lora_freq = val.toInt();
        else if (key == "lora_intervalle_secondes") config.lora_intervalle_secondes = val.toInt();
    }
    f.close();
}

// ----- Connexion LoRaWAN -----
bool SetUpLoRaWAN() {
    if (!modem.begin(EU868)) {
        Serial.println("Échec initialisation module LoRaWAN");
        return false;
    }

    int connected = modem.joinOTAA(config.appEui, config.appKey);
    if (!connected) {
        Serial.println("Échec de la connexion LoRaWAN. Réessayer...");
        return false;
    }

    modem.minPollInterval(60);
    modem.setADR(true);
    return true;
}

// ----- Envoi vers le serveur -----
bool SendQueue() {
    while (!sendingQueue.empty()) {
        String payload = sendingQueue.front();
        int err = 0, retries = 0;
        do {
            modem.beginPacket();
            modem.print(payload);
            err = modem.endPacket(true);
            if (err <= 0) {
                Serial.println("Erreur d’envoi, nouvelle tentative...");
                delay(10000);
            }
        } while (err <= 0 && ++retries < 6);

        if (err > 0) {
            Serial.println("Donnée envoyée avec succès : " + payload);
            sendingQueue.pop(); // suppression seulement après succès
        } else {
            Serial.println("Échec après plusieurs tentatives, abandon du paquet.");
            return false;
        }

        delay(5000);
    }
    return true;
}

// ----- Setup -----
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(115200);
    unsigned long end_date = millis() + 5000;
    while (!Serial && millis() < end_date) {}

    Serial.println("\n=== Initialisation du Relais Molonari ===");

    lireConfigCSV("relay_config.csv");
    Serial.println("Configuration chargée.");

    lora = LoraCommunication(config.lora_freq, 0xAA, 0xFF);

    if (!SD.begin(config.CSPin)) {
        Serial.println("Erreur SD - arrêt système.");
        while (true) {}
    }

    Serial.println("Initialisation terminée !");
    pinMode(LED_BUILTIN, INPUT_PULLDOWN);
}

// ----- Loop -----
void loop() {
    Waiter waiter;
    waiter.startTimer();

    std::queue<String> receiveQueue;
    lora.startLoRa();

    if (lora.handshake(0)) {
        Serial.println("Handshake réussi. Réception des paquets...");
        int last = lora.receivePackets(receiveQueue);
        lora.closeSession(last);
        lora.stopLoRa();

        while (!receiveQueue.empty()) {
            sendingQueue.push(receiveQueue.front());
            receiveQueue.pop();
        }

        // Si le temps d'envoi est atteint
        unsigned long currentTime = millis() / 1000;
        if (currentTime - lastLoraSend >= (unsigned long)config.lora_intervalle_secondes) {
            if (SetUpLoRaWAN()) {
                if (SendQueue()) {
                    Serial.println("Tous les paquets ont été envoyés !");
                } else {
                    Serial.println("Certains paquets n’ont pas pu être envoyés, ils seront réessayés.");
                }
            } else {
                Serial.println("Connexion LoRaWAN impossible, report de l’envoi.");
            }
            lastLoraSend = currentTime;
        }
    } else {
        Serial.println("Handshake échoué, aucune donnée reçue.");
        lora.stopLoRa();
    }

    Serial.println("Relais en veille jusqu’à la prochaine fenêtre de communication...");
    waiter.sleepUntil(60000); // dort 1 min avant nouvelle tentative
}
