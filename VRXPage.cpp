#include <SH1106.h>
#include <Arduino.h>
#include "VRXPage.h"
#include "GlobalScope.h"
#include "Rx5808Fns.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"

VRXPage::VRXPage() {
  edit = false;
}

void VRXPage::drawFreq() {
  handleState();

  int width = oled.width() - 8 ;
  int height = oled.height() - 4;
  int margin = 4;
  oled.drawRoundRect(4, 2, width, height, 8, WHITE);
  oled.setTextColor(WHITE);
  oled.setTextSize(3);
  oled.setCursor(ceil(oled.width() / 4), 15);
  oled.println(tunnedBand[vrx]);
  oled.setCursor(ceil(oled.width() / 2), 15);
  if (true == edit) {
    oled.setTextColor(BLACK, WHITE);
  }
  oled.println(tunnedChannel[vrx]);
  oled.setTextSize(2);
  oled.setCursor(ceil(oled.width() / 2), 45);
  oled.setTextColor(WHITE);
  oled.println(freqStringToFreqInMhz(tunnedBand[vrx] + tunnedChannel[vrx]));

}

void VRXPage::handleState() {
  if (prevBtn != buttonName && buttonName == "LEFT" && edit == true) {
    tunnedChannel[vrx] = (tunnedChannel[vrx] == 1) ? 8 : tunnedChannel[vrx] - 1 ;
  }
  if (prevBtn != buttonName && buttonName == "RIGHT" && edit == true) {
    tunnedChannel[vrx] = (tunnedChannel[vrx] == 8) ? 1 : tunnedChannel[vrx] + 1;
  }
  if (prevBtn != buttonName && buttonName == "ENTER") {
    if (edit == true) {
      saveVRX();
      reset();
    } else {
      if (enter > 0) {
        edit = true;
      }
    }
  }
  if (prevBtn != buttonName && buttonName == "BACK") {
    reset();
  }
}

void VRXPage::reset() {
  edit = false;
  vrx = 0;
  page = "mainPage";
}

void VRXPage::saveVRX() {
  tunnedFreq[vrx] = freqStringToFreqInMhz(tunnedBand[vrx] + tunnedChannel[vrx]);
  if (prevFreq[vrx] != tunnedFreq[vrx]) {
    selectVRX(vrx);
    setChannelByFreq(tunnedFreq[vrx]);
    prevFreq[vrx] = tunnedFreq[vrx];
    EEPROM_writeAnything(5*vrx, tunnedChannel[vrx]);
  }
}

