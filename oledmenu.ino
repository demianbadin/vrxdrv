/*********************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

  This example is for a 128x64 size display using SPI to communicate
  4 or 5 pins are required to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <SH1106.h>
#include <Arduino.h>
#include "GlobalScope.h"
#include "MainPage.h"
#include "VRXPage.h"
#include "Rx5808Fns.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"

#define OLED_MOSI   3
#define OLED_CLK   4
#define OLED_DC    2
#define OLED_CS    6
#define OLED_RESET 5
static Adafruit_SH1106 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
MainPage mainPage = MainPage();
VRXPage vrxPage = VRXPage();

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int ble = 0;
void setup()   {
  Serial.begin(115200);
  display.begin(SH1106_SWITCHCAPVCC);
  display.clearDisplay();
  display.drawBitmap(0, 0, rsz_argentos2, 127, 62, WHITE);
  display.display();

  //INIT VRXs PINS
  for (int i = 0; i < 4; i++) {
    pinMode(VRX_DATA_PIN[i], OUTPUT);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(RX5808_SEL_PIN, OUTPUT);
  pinMode(RX5808_CLK_PIN, OUTPUT);

  //END INIT VRXs PINs

  for (int i = 0; i < 4; i++) {
    EEPROM_readAnything(5 * i, tunnedChannel[i]);
    tunnedFreq[i] = freqStringToFreqInMhz(tunnedBand[i] + tunnedChannel[i]);
    prevFreq[i] = tunnedFreq[i];
    selectVRX(i);
    Serial.print("Setting VRX: ");
    Serial.print(i);
    Serial.print(" to: ");
    Serial.println(tunnedFreq[i]);
    setChannelByFreq(tunnedFreq[i]);
    delay(50);
  }
  delay(1000);
  display.clearDisplay();
}

long buttonPressed = 0;

void loop() {
  manageButtons();
  updateRssi();
}
long startTime = 0;
void manageButtons() {
  prevBtn = buttonName;
  int button = analogRead(A5);
  delay(25);
  //  if (button > 7 && button < 12) {
  //    buttonName = "UP";
  //  }
  if (button > 1000 && buttonName != "BACK") {
    if (startTime == 0) {
      startTime = millis();
    }
    if (startTime + 300 < millis()) {
      buttonName = "BACK";
      startTime = 0;
    }
    if (buttonName != "BACK") {
      if (enter == 0) {
        buttonName = "ENTER";
        enter++;
      } else {
        buttonName = "NONE";
      }
    }
  }
  if (button > 120 && button < 143) {
    buttonName = "RIGHT";
  }
  if (button > 28 && button < 45) {
    buttonName = "LEFT";
  }
  //  if (button == 1 || button ==2) {
  //    buttonName = "ENTER";
  //  }
  if (button == 0) {
    buttonName = "NONE";
    prevBtn = "NONE";
    startTime = 0;
    enter = 0;
  }
  UIRouter();
}

void updateRssi() {
  if (rssiReadInx == 4) {
    rssiReadInx = 0;
  }

  switch (rssiReadInx) {
    case 0:
      tunnedRSSI[0] = analogRead(A0);
      break;
    case 1:
      tunnedRSSI[1] = analogRead(A1);
      break;
    case 2:
      tunnedRSSI[2] = analogRead(A2);
      break;
    case 3:
      tunnedRSSI[3] = analogRead(A3);
      break;
  }

  rssiReadInx++;

}

void UIRouter() {
  display.clearDisplay();
  if (page == "mainPage") {
    mainPage.drawVRX();
  }
  if (page == "vrxPage") {
    vrxPage.drawFreq();
  }
  display.display();
  delay(5);
}
