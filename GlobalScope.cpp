#include "GlobalScope.h"
String buttonName = "NONE";
String prevBtn = "NONE";
int vrx = 0;
String page = "mainPage";
// R band 5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917,
int prevFreq[] = {5658, 5732, 5843, 5917};
int tunnedFreq[] = {5658, 5732, 5843, 5917};
int tunnedChannel[] = {1, 3, 6, 8};
String tunnedBand[] = {"R", "R", "R", "R"};
int enter = 0;
int VRX_DATA_PIN[] = {7, 8, 9, 10};
int RX5808_DATA_PIN = 7;             //DATA output line to RX5808 module
int RX5808_SEL_PIN = 11;              //CLK output line to RX5808 module
int RX5808_CLK_PIN = 12;
int RSSI_PRI_PIN = A0;
int tunnedRSSI[] = {0, 0, 0, 0};
int rssiReadInx = 0;

void selectVRX(int vrxIdx) {
  RX5808_DATA_PIN = VRX_DATA_PIN[vrxIdx];
};
