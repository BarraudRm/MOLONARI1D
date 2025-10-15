#include "LoRaWan_Molonari.hpp"

#ifdef LORA_DEBUG
#define LORA_LOG(msg) Serial.print(msg);
#define LORA_LOG_HEX(msg) Serial.print(msg, HEX);
#define LORA_LOG_LN(msg) Serial.println(msg);
#else
#define LORA_LOG(msg)
#define LORA_LOG_HEX(msg)
#define LORA_LOG_LN(msg)
#endif
// Constructor: Set pin and frequency for the LoRa module
LoraCommunication::LoraCommunication(long frequency, uint8_t localAdd, uint8_t desti)
    : freq(frequency), localAddress(localAdd), destination(desti), active(false)
{
}

void LoraCommunication::startLoRa()
{
    if (!active)
    {
        int retries = 3;
        while (retries--)
        {
            if (LoRa.begin(freq))
            {
                active = true;
                LORA_LOG_LN("LoRa started.");
                return;
            }
            else
            {
                LORA_LOG_LN("LoRa failed to start, retrying...");
                delay(1000); // Delay before retrying
            }
        }
        LORA_LOG_LN("Starting LoRa failed after retries.");
    }
}

void LoraCommunication::stopLoRa()
{
    if (active)
    {
        LoRa.end(); // This internally calls LoRa.sleep() and stops SPI

        // Step 3: Pull the RESET pin low to cut power if connected directly
        pinMode(LORA_RESET, OUTPUT);
        digitalWrite(LORA_RESET, LOW);
        active = false;
        LORA_LOG_LN("LoRa stopped.");
    }
}

// LA FONCTION SUIVANTE N'EST PAS PRESENTE DANS L'AUTRE DOCUMENT
void LoraCommunication::setdesttodefault()
{
    if (destination != 0xff)
        destination = 0xff;
}

void LoraCommunication::sendPacket(uint8_t packetNumber, RequestType requestType, const String &payload)
{
    if (active)
    {
        uint8_t b = LoRa.random();
        delay(100 + b); // Small delay to ensure buffer is cleared // Small delay to ensure buffer is cleared

        bool success = (bool)LoRa.beginPacket();
        if (!success)
        {
            LORA_LOG_LN("Aborting transmission : LoRa module busy");
            return;
        }
        // Calculate and append a simple checksum
        uint8_t checksum = calculateChecksum(destination, localAddress, packetNumber, requestType, payload);
        LoRa.write(checksum);

        LoRa.write(destination);
        LoRa.write(localAddress);
        LoRa.write(packetNumber);
        LoRa.write(requestType);
        //LoRa.write(payload.length()); // Specify payload length
        LoRa.print(payload);

        LoRa.endPacket();

        LORA_LOG("Packet sent: ");
        LORA_LOG_LN(payload);
    }
    else
    {
        LORA_LOG_LN("LoRa is not active, cannot send packet.");
    }
}

bool LoraCommunication::receivePacket(uint8_t &packetNumber, RequestType &requestType, String &payload)
{
    if (!active)
#include "LoraWAN.hpp"

LoraWANCommunication::LoraWANCommunication() {}

bool LoraWANCommunication::begin(const String& appEui, const String& appKey) {
    if (!modem.begin(EU868)) {
        Serial.println("Échec initialisation module LoRaWAN");
        return false;
    }

    int connected = modem.joinOTAA(appEui, appKey);
    if (!connected) {
        Serial.println("Échec de la connexion LoRaWAN");
        return false;
    }

    modem.minPollInterval(60);
    modem.setADR(true);
    return true;
}

bool LoraWANCommunication::sendQueue(std::queue<String>& sendingQueue) {
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
            Serial.println("Donnée envoyée : " + payload);
            sendingQueue.pop();
        } else {
            Serial.println("Échec après plusieurs tentatives, abandon du paquet.");
            return false;
        }
        delay(5000);
    }
    return true;
}
