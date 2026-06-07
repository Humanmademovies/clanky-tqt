#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Init ecran...");

    tft.init();
    tft.setRotation(0);

    tft.fillScreen(TFT_RED);
    delay(800);
    tft.fillScreen(TFT_GREEN);
    delay(800);
    tft.fillScreen(TFT_BLUE);
    delay(800);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);              // Middle-Center
    tft.drawString("CLANKY", 64, 64);        // centre exact d'une dalle 128x128

    Serial.println("Ecran OK");
}

void loop() {
    delay(5000);
}