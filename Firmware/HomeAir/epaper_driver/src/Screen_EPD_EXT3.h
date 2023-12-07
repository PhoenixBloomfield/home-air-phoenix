
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

// Other libraries
#include "hV_Screen_Buffer.h"

// Board
#include "hV_Board.h"

// PDLS utilities
#include "hV_Utilities_PDLS.h"

// Checks
#if (hV_HAL_PERIPHERALS_RELEASE < 700)
#error Required hV_HAL_PERIPHERALS_RELEASE 700
#endif // hV_HAL_PERIPHERALS_RELEASE

#if (hV_CONFIGURATION_RELEASE < 702)
#error Required hV_CONFIGURATION_RELEASE 702
#endif // hV_CONFIGURATION_RELEASE

#if (hV_SCREEN_BUFFER_RELEASE < 700)
#error Required hV_SCREEN_BUFFER_RELEASE 700
#endif // hV_SCREEN_BUFFER_RELEASE

#if (hV_BOARD_RELEASE < 700)
#error Required hV_BOARD_RELEASE 700
#endif // hV_BOARD_RELEASE

#ifndef SCREEN_EPD_EXT3_RELEASE
///
/// @brief Library release number
///
#define SCREEN_EPD_EXT3_RELEASE 702

///
/// @brief Library variant
///
#define SCREEN_EPD_EXT3_VARIANT "Basic-Global"

// Other libraries
#include "hV_Screen_Buffer.h"

#if (hV_SCREEN_BUFFER_RELEASE < 700)
#error Required hV_SCREEN_BUFFER_RELEASE 700
#endif // hV_SCREEN_BUFFER_RELEASE

class Screen_EPD_EXT3 final : public hV_Screen_Buffer, public hV_Utilities_PDLS
{
  public:

    Screen_EPD_EXT3(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board = boardESP32DevKitC);

    void begin();

    String WhoAmI();

    void clear(uint16_t colour = myColours.white);

    void flush();

    void regenerate();

    uint8_t flushMode(uint8_t updateMode = UPDATE_GLOBAL);

  protected:

    void _setOrientation(uint8_t orientation); // compulsory

    bool _orientCoordinates(uint16_t & x, uint16_t & y); // compulsory

    void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour);

    uint16_t _getPoint(uint16_t x1, uint16_t y1);

    uint32_t _getZ(uint16_t x1, uint16_t y1);

    uint16_t _getB(uint16_t x1, uint16_t y1);

    void _flushGlobal();

};

#endif // SCREEN_EPD_EXT3_RELEASE

