#ifndef VRXPage_H
#define VRXPage_H

class VRXPage {
  public:
    VRXPage();
    void drawBand();
    void drawFreq();
    boolean edit;
  private:
    Adafruit_SH1106 oled;
    void handleState();
    void reset();
    void saveVRX();
    
};

#endif
