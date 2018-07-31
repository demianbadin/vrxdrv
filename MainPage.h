#ifndef MainPage_H
#define MainPage_H
#include <SH1106.h>
#include <Arduino.h>

class MainPage {
  public:
    MainPage();
    void drawVRX();
    void drawVRXInfo(int& boxX, int& vrxIdx);
    boolean edit;
  private:
    int boxY;
    int vrxSelected;
    Adafruit_SH1106 oled;
    void handleState();
    void reset();
    int scaleRssi(int rawRssi);
    int trimRssi(int rawRssi);
    
};

#endif

