#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

// --- CONFIGURATION WIFI ---
const char* ssid     = "Bbox-94C2D5D3";
const char* password = "gwCcgaysA4rJkaGrW2";

// --- CONFIGURATION SERVEUR ---
const char* WS_HOST = "192.168.1.103";
const uint16_t WS_PORT = 7860;
const char* WS_PATH = "/ws-client";

WebSocketsClient webSocket;

void onWsEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            Serial.println(">>> WebSocket CONNECTE");
            break;
        case WStype_DISCONNECTED:
            Serial.println(">>> WebSocket DECONNECTE");
            break;
        case WStype_TEXT:
            Serial.print(">>> Texte recu : ");
            Serial.write(payload, length);
            Serial.println();
            break;
        case WStype_BIN:
            Serial.print(">>> Binaire recu, ");
            Serial.print(length);
            Serial.println(" octets");
            break;
        case WStype_ERROR:
            Serial.println(">>> ERREUR WebSocket");
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.print("Connexion WiFi a ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("WiFi OK. IP = ");
    Serial.println(WiFi.localIP());

    Serial.print("Connexion WebSocket a ws://");
    Serial.print(WS_HOST);
    Serial.print(":");
    Serial.print(WS_PORT);
    Serial.println(WS_PATH);

    webSocket.begin(WS_HOST, WS_PORT, WS_PATH);
    webSocket.onEvent(onWsEvent);
    webSocket.setReconnectInterval(3000);
}

void loop() {
    webSocket.loop();
}