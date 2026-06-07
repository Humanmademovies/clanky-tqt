#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <TFT_eSPI.h>

// --- CONFIGURATION WIFI ---
const char* ssid     = "Bbox-94C2D5D3";
const char* password = "gwCcgaysA4rJkaGrW2";

// --- CONFIGURATION SERVEUR ---
const char* WS_HOST = "192.168.1.103";
const uint16_t WS_PORT = 7860;
const char* WS_PATH = "/ws";

// --- PROTOCOLE ---
const uint8_t TYPE_AUDIO = 0x01;
const uint8_t TYPE_IMAGE = 0x02;

// --- IMAGE EMOTION ---
const int EMOJI_SIZE = 96;
const int EMOJI_X = (128 - EMOJI_SIZE) / 2;   // centrage horizontal
const int EMOJI_Y = (128 - EMOJI_SIZE) / 2;   // centrage vertical
const size_t EMOJI_BYTES = EMOJI_SIZE * EMOJI_SIZE * 2;  // 18432

TFT_eSPI tft = TFT_eSPI();
WebSocketsClient webSocket;

void onWsEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            Serial.println(">>> WebSocket CONNECTE");
            tft.fillScreen(TFT_BLACK);
            break;
        case WStype_DISCONNECTED:
            Serial.println(">>> WebSocket DECONNECTE");
            break;
        case WStype_BIN:
            if (length < 1) return;
            if (payload[0] == TYPE_IMAGE) {
                // Verifier la taille attendue
                if (length - 1 == EMOJI_BYTES) {
                    tft.setSwapBytes(true);
                    tft.pushImage(
                        EMOJI_X, EMOJI_Y, EMOJI_SIZE, EMOJI_SIZE,
                        (uint16_t*)(payload + 1)
                    );
                    Serial.println(">>> Image emotion affichee");
                } else {
                    Serial.print(">>> Image taille inattendue: ");
                    Serial.println(length - 1);
                }
            } else if (payload[0] == TYPE_AUDIO) {
                // Audio : pas encore gere (brique suivante)
            }
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Ecran
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("WiFi...", 64, 64);

    // WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("WiFi OK. IP = ");
    Serial.println(WiFi.localIP());

    tft.fillScreen(TFT_BLACK);
    tft.drawString("WS...", 64, 64);

    // WebSocket
    webSocket.begin(WS_HOST, WS_PORT, WS_PATH);
    webSocket.onEvent(onWsEvent);
    webSocket.setReconnectInterval(3000);
}

void loop() {
    webSocket.loop();
}