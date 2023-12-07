
// Library header
#include "Screen_EPD_EXT3.h"

#if defined(ENERGIA)
///
/// @brief Proxy for SPISettings
/// @details Not implemented in Energia
/// @see https://www.arduino.cc/en/Reference/SPISettings
///
struct _SPISettings_s
{
    uint32_t clock; ///< in Hz, checked against SPI_CLOCK_MAX = 16000000
    uint8_t bitOrder; ///< LSBFIRST, MSBFIRST
    uint8_t dataMode; ///< SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3
};
///
/// @brief SPI settings for screen
///
_SPISettings_s _settingScreen;
#else
///
/// @brief SPI settings for screen
///
SPISettings _settingScreen;
#endif // ENERGIA

#ifndef SPI_CLOCK_MAX
#define SPI_CLOCK_MAX 16000000
#endif


//
// === COG section
//

//
// === End of COG section
//

//
// === Class section
//
Screen_EPD_EXT3::Screen_EPD_EXT3(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board)
{
    u_eScreen_EPD_EXT3 = eScreen_EPD_EXT3;
    b_pin = board;
    u_newImage = 0; // nullptr
}

void Screen_EPD_EXT3::begin()
{
    u_codeExtra = (u_eScreen_EPD_EXT3 >> 16) & 0xff;
    u_codeSize = (u_eScreen_EPD_EXT3 >> 8) & 0xff;
    u_codeType = u_eScreen_EPD_EXT3 & 0xff;
    _screenColourBits = 2; // BWR and BWRY

    u_begin(b_pin, FAMILY_SMALL, 50);

    switch (u_codeSize)
    {

        case 0x21: // 2.13"

            _screenSizeV = 212; // vertical = wide size
            _screenSizeH = 104; // horizontal = small size
            _screenDiagonal = 213;
            break;

        case 0x26: // 2.66"

            _screenSizeV = 296; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            _screenDiagonal = 266;
            break;

        default:

            break;
    } // u_codeSize

    u_bufferDepth = _screenColourBits; // 2 colours
    u_bufferSizeV = _screenSizeV; // vertical = wide size
    u_bufferSizeH = _screenSizeH / 8; // horizontal = small size 112 / 8; 1 bit per pixel

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    u_pageColourSize = (uint32_t)u_bufferSizeV * (uint32_t)u_bufferSizeH;

    u_frameSize = u_pageColourSize;

#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (u_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(u_pageColourSize * u_bufferDepth);
        u_newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (u_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[u_pageColourSize * u_bufferDepth];
        u_newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM

    memset(u_newImage, 0x00, u_pageColourSize * u_bufferDepth);

    // Initialise the /CS pins
    pinMode(b_pin.panelCS, OUTPUT);
    digitalWrite(b_pin.panelCS, HIGH); // CS# = 1

    // New generic solution
    pinMode(b_pin.panelDC, OUTPUT);
    pinMode(b_pin.panelReset, OUTPUT);
    pinMode(b_pin.panelBusy, INPUT); // All Pins 0

    // Initialise Flash /CS as HIGH
    if (b_pin.flashCS != NOT_CONNECTED)
    {
        pinMode(b_pin.flashCS, OUTPUT);
        digitalWrite(b_pin.flashCS, HIGH);
    }

    // Initialise slave panel /CS as HIGH
    if (b_pin.panelCSS != NOT_CONNECTED)
    {
        pinMode(b_pin.panelCSS, OUTPUT);
        digitalWrite(b_pin.panelCSS, HIGH);
    }

    // Initialise slave Flash /CS as HIGH
    if (b_pin.flashCSS != NOT_CONNECTED)
    {
        pinMode(b_pin.flashCSS, OUTPUT);
        digitalWrite(b_pin.flashCSS, HIGH);
    }

    // Initialise SD-card /CS as HIGH
    if (b_pin.cardCS != NOT_CONNECTED)
    {
        pinMode(b_pin.cardCS, OUTPUT);
        digitalWrite(b_pin.cardCS, HIGH);
    }

    // Initialise SPI
    _settingScreen = {4000000, MSBFIRST, SPI_MODE0};

// #if defined(ARDUINO_XIAO_ESP32C3)

//     // Board Xiao ESP32-C3 crashes if pins are specified.
//     SPI.begin(8, 9, 10); // SCK MISO MOSI

// #elif defined(ARDUINO_NANO_ESP32)

//     // Board Arduino Nano ESP32 arduino_nano_nora v2.0.11
//     SPI.begin();

// #elif defined(ARDUINO_ARCH_ESP32)

//     // Board ESP32-Pico-DevKitM-2 crashes if pins are not specified.
//     SPI.begin(14, 12, 13); // SCK MISO MOSI

// #else

    // SPI.begin(14, 12, 13);
    SPI.begin(b_pin.panelSCK, b_pin.panelMISO, b_pin.panelMOSI);

// #endif // ARDUINO_ARCH_ESP32

    SPI.beginTransaction(_settingScreen);

// #endif // ENERGIA

    // Reset
            b_reset(5, 5, 10, 5, 5); // small


    // Standard
    hV_Screen_Buffer::begin();

    setOrientation(0);
    if (f_fontMax() > 0)
    {
        f_selectFont(0);
    }
    f_fontSolid = false;

    _penSolid = false;
    u_invert = false;

    // Report
    Serial.println(formatString("= Screen %s %ix%i", WhoAmI().c_str(), screenSizeX(), screenSizeY()));
    Serial.println(formatString("= PDLS %s v%i.%i.%i", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));

    clear();
}

String Screen_EPD_EXT3::WhoAmI()
{
    char work[64] = {0};
    u_WhoAmI(work);

    return formatString("iTC %i.%02i\"%s", _screenDiagonal / 100, _screenDiagonal % 100, work);
}

uint8_t Screen_EPD_EXT3::flushMode(uint8_t updateMode)
{
    updateMode = checkTemperatureMode(updateMode);

    switch (updateMode)
    {
        case UPDATE_FAST:
        case UPDATE_GLOBAL:

            _flushGlobal();
            break;

        default:

            Serial.println("* PDLS - UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}

void Screen_EPD_EXT3::flush()
{
    flushMode(UPDATE_GLOBAL);
}

void Screen_EPD_EXT3::_flushGlobal()
{
    uint8_t * blackBuffer = u_newImage;
    uint8_t * redBuffer = u_newImage + u_pageColourSize;

     b_reset(5, 5, 10, 5, 5);

        uint8_t data9[] = {0x0e};
        b_sendIndexData(0x00, data9, 1); // Soft-reset
        delay_ms(5);

        uint8_t data7[] = {0x19};
        data7[0] = u_temperature; // u_temperature
        b_sendIndexData(0xe5, data7, 1); // Input Temperature 0°C = 0x00, 22°C = 0x16, 25°C = 0x19
        uint8_t data6[] = {0x02};
        b_sendIndexData(0xe0, data6, 1); // Active Temperature


        uint8_t index00_work[2] = {0xcf, 0x8d}; // PSR, all except 4.2"

        b_sendIndexData(0x00, index00_work, 2); // PSR

        // Send image data
        b_sendIndexData(0x10, blackBuffer, u_frameSize); // First frame
        b_sendIndexData(0x13, redBuffer, u_frameSize); // Second frame

        delay_ms(50);
        uint8_t data8[] = {0x00};
        b_sendIndexData(0x04, data8, 1); // Power on
        delay_ms(5);
        while (digitalRead(b_pin.panelBusy) != HIGH)
        {
            delay(100);
        };

        while (digitalRead(b_pin.panelBusy) != HIGH);
        b_sendIndexData(0x12, data8, 1); // Display Refresh
        delay_ms(5);
        while (digitalRead(b_pin.panelBusy) != HIGH)
        {
            delay(100);
        };

        b_sendIndexData(0x02, data8, 1); // Turn off DC/DC
        delay_ms(5);
        while (digitalRead(b_pin.panelBusy) != HIGH)
        {
            delay(100);
        };
        digitalWrite(b_pin.panelDC, LOW);
        digitalWrite(b_pin.panelCS, LOW);

        digitalWrite(b_pin.panelReset, LOW);
        // digitalWrite(PNLON_PIN, LOW);
    digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
}

void Screen_EPD_EXT3::clear(uint16_t colour)
{
    if (colour == myColours.red)
    {
        // physical red 01
        memset(u_newImage, 0x00, u_pageColourSize);
        memset(u_newImage + u_pageColourSize, 0xff, u_pageColourSize);
    }

    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black 00
        memset(u_newImage, 0x00, u_pageColourSize);
        memset(u_newImage + u_pageColourSize, 0x00, u_pageColourSize);
    }
    else
    {
        // physical white 10
        memset(u_newImage, 0xff, u_pageColourSize);
        memset(u_newImage + u_pageColourSize, 0x00, u_pageColourSize);
    }
}

void Screen_EPD_EXT3::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    // _orientCoordinates() returns false = success, true = error
    if (_orientCoordinates(x1, y1) == RESULT_ERROR)
    {
        return;
    }

    // Coordinates
    uint32_t z1 = _getZ(x1, y1);
    uint16_t b1 = _getB(x1, y1);

    // Basic colours
    if (colour == myColours.red)
    {
        // physical red 01
        bitClear(u_newImage[z1], b1);
        bitSet(u_newImage[u_pageColourSize + z1], b1);
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black 00
        bitClear(u_newImage[z1], b1);
        bitClear(u_newImage[u_pageColourSize + z1], b1);
    }
    else if ((colour == myColours.black) xor u_invert)
    {
        // physical white 10
        bitSet(u_newImage[z1], b1);
        bitClear(u_newImage[u_pageColourSize + z1], b1);
    }
}

void Screen_EPD_EXT3::_setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
}

bool Screen_EPD_EXT3::_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool _flagResult = RESULT_ERROR; // false = success, true = error
    switch (_orientation)
    {
        case 3: // checked, previously 1

            if ((x < _screenSizeV) and (y < _screenSizeH))
            {
                x = _screenSizeV - 1 - x;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 2: // checked

            if ((x < _screenSizeH) and (y < _screenSizeV))
            {
                x = _screenSizeH - 1 - x;
                y = _screenSizeV - 1 - y;
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 1: // checked, previously 3

            if ((x < _screenSizeV) and (y < _screenSizeH))
            {
                y = _screenSizeH - 1 - y;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        default: // checked

            if ((x < _screenSizeH) and (y < _screenSizeV))
            {
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;
    }

    return _flagResult;
}

uint16_t Screen_EPD_EXT3::_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
}

uint32_t Screen_EPD_EXT3::_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1;
    return  z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 3);
}

uint16_t Screen_EPD_EXT3::_getB(uint16_t x1, uint16_t y1)
{
    uint16_t b1 = 0;

    b1 = 7 - (y1 % 8);

    return b1;
}

void Screen_EPD_EXT3::regenerate()
{
    clear(myColours.black);
    flush();

    delay(100);

    clear(myColours.white);
    flush();
}