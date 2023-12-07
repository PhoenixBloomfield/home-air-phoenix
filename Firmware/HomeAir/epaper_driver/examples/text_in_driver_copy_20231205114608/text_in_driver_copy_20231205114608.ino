#include "epaper_driver.h"
#include "hV_HAL_Peripherals.h"
#include "hV_Configuration.h"


//DEFAULT BOARD: boardESP32DevKitC
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_213); //smaller 2.13" screen
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_266); //larger 2.66" screen
/*
  Pins:
    "BUSY", Red devkit jumper = 16
    "D/C", Orange devkit jumper = 18 
    "RST", Yellow devkit jumper = 17
    "MISO", Green devkit jumper = 12
    "MOSI", Blue devkit jumper = 13
    "SCK", Brown devkit jumper = 14
    "FCSM", Purple devkit jumper = 8
    "ECSM", Grey devkit jumper = 3
*/

//BREADBOARD: breadBoardBreakout
Screen_EPD_EXT3 screen(eScreen_EPD_EXT3_213, breadBoardBreakout); //smaller 2.13" screen
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_266, breadBoardBreakout); //larger 2.66" screen
/*
  Pins:
    "BUSY", Red devkit jumper = 45
    "D/C", Orange devkit jumper = 48 
    "RST", Yellow devkit jumper = 47
    "MISO", Green devkit jumper = 37
    "MOSI", Blue devkit jumper = 36
    "SCK", Brown devkit jumper = 35
    "FCSM", Purple devkit jumper = 21
    "ECSM", Grey devkit jumper = 11
*/

//CUSTOM PINS
//for redefinition of pins if necessary
const pins_t customPins =
{
    .panelBusy = 45, //"BUSY", Red breadboard jumper
    .panelDC = 48, //"D/C", Orange breadboard jumper
    .panelReset = 47, //"RST", Yellow breadboard jumper
    .panelMISO = 37, //"MISO", Green breadboard jumper
    .panelMOSI = 36, //"MOSI", Blue breadboard jumper
    .panelSCK = 35, //"SCK", Brown breadboard jumper
    .flashCS = 21, //"FCSM", Purple breadboard jumper
    .panelCS = 11, //"ECSM", Grey breadboard jumper
};
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_213, customPins); //smaller 2.13" screen
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_266, customPins); //larger 2.66" screen


void setup()
{
  Serial.begin(115200);
  delay(500);
  screen.begin();
  screen.clearScreen(); //resets image

  /*drawText function:
    drawText(int x, int y, int fontSize, String msg, String color = "black");
    x = x pixel location of text
    y = y pixel location of text
    fontSize = size of text, 1-4 where 1 is smallest and 4 is biggest
    msg = text to display
    color = color of text, black by default. Specify "red" for red text
  */

//   enum sensorMap {
//   CO2_PPM = 0,
//   PPM_1_0 = 1,
//   PPM_2_5 = 2,
//   PPM_4_0 = 3,
//   PPM_10 = 4,
//   HUMIDITY = 5,
//   TEMP = 6,
//   VOC = 7,
//   CO = 8,
//   NG = 9,
//   AQI = 10
// };

  // int num = 600;
  // String msg = "CO2 (PPM): " + String(num);
  // myScreen.drawText(5, 5, 2, "CO2 (PPM): " + String(num)); //adds text to buffer
  // msg = "Particles 1.0/2.5/4.0/10: " + "100/100/100/100";
  // myScreen.drawText(10, 30, 2, "Particles 1.0: "); //adds red text to buffer
  // myScreen.drawText(10, 50, 3, "Large text"); //adds text to buffer
  // myScreen.drawText(10, 80, 4, "Huge text", "red"); //adds red text to buffer
  screen.drawText(5, 5, 2, "CO2 PPM: ");
  // myScreen.drawText(5, 20, 2, "Temperature: " );
  // myScreen.drawText(5, 35, 2, "Humidity: ");
  // myScreen.drawText(5, 50, 2, "CO (PPM): ");
  // myScreen.drawText(5, 65, 2, "CH4 (PPM): ");
  // // myScreen.drawText(5, 50, 3, "Large text"); //adds text to buffer
  // myScreen.drawText(10, 80, 4, "AQI: ", "red"); //adds red text to buffer
  // myScreen.flush(); //refreshes screen
}

void loop()
{

}
