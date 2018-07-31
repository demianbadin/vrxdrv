#include <SH1106.h>
#include <Arduino.h>
#include "MainPage.h"
#include "GlobalScope.h"
#include "Rx5808Fns.h"

MainPage::MainPage() {
  boxY = 10;
  edit = false;
}

void MainPage::drawVRX() {

  handleState();

  int width = (oled.width() / 4);
  int height = oled.height() - 15;
  int margin = 4;
  for (int i = 0; i < 4; i++) {
    int boxX = (width * i) + (margin / 2);
    if (true == edit && vrx == i) {
      oled.fillRoundRect(boxX, boxY, width - margin, height, 8, WHITE);
    } else {
      oled.drawRoundRect(boxX, boxY, width - margin, height, 8, WHITE);
    }
    drawVRXInfo(boxX, i);
  }
}

void MainPage::drawVRXInfo(int& boxX, int& vrxIdx) {

  if (true == edit && vrx == vrxIdx) {
    oled.setTextColor(BLACK, WHITE);
  } else {
    oled.setTextColor(WHITE);
  }
  oled.setTextSize(2);
  oled.setCursor(boxX + 3, boxY + 6);
  oled.println(tunnedBand[vrxIdx] + tunnedChannel[vrxIdx]);
  oled.setTextSize(1);
  oled.setCursor(boxX + 3, boxY + 25);
  oled.println(scaleRssi(tunnedRSSI[vrxIdx]));
  oled.setCursor(boxX + 3, boxY + 35);
  oled.println(freqStringToFreqInMhz(tunnedBand[vrxIdx] + tunnedChannel[vrxIdx]));
}

void MainPage::handleState() {
  if (prevBtn != buttonName && buttonName == "LEFT" && edit == true) {
    vrx = (vrx == 0) ? 3 : vrx - 1 ;
  }
  if (prevBtn != buttonName && buttonName == "RIGHT" && edit == true) {
    vrx = (vrx == 3) ? 0 : vrx + 1;
  }

  if (prevBtn != buttonName && buttonName == "ENTER") {
    if (edit == true) {
      reset();
      page = "vrxPage";
    } else {
      edit = true;
    }
  }
  if (prevBtn != buttonName && buttonName == "BACK") {
    reset();
  }

}

void MainPage::reset() {
  edit = false;
  enter = 0;
}

int MainPage::trimRssi(int rawRssi) {
  return rawRssi < 0 ? 0 : (rawRssi > 100 ? 100 : rawRssi);
}

int MainPage::scaleRssi(int rawRssi) {
  return trimRssi(map(rawRssi, 140, 250, 0, 100 ));
}
