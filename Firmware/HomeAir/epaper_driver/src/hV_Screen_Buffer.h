

#include "hV_HAL_Peripherals.h"

#include "hV_Configuration.h"

#ifndef hV_SCREEN_BUFFER_RELEASE

#define hV_SCREEN_BUFFER_RELEASE 700

#include "hV_Colours565.h"

#include "hV_Utilities_Common.h"

#if (FONT_MODE == USE_FONT_TERMINAL)
#include "hV_Font_Terminal.h"

#else
#error FONT_MODE not defined
#endif // FONT_MODE

#warning USE_hV_SCREEN == USE_hV_SCREEN_SELF
#warning FONT_MODE == USE_FONT_TERMINAL
class hV_Screen_Buffer : protected hV_Font_Terminal
{
  public:

    hV_Screen_Buffer();

    virtual void begin();

    virtual String WhoAmI() = 0; // compulsory

    virtual void clear(uint16_t colour = myColours.black);

    virtual void flush() = 0; // compulsory

    virtual void setOrientation(uint8_t orientation);

    uint8_t getOrientation();

    virtual uint16_t screenSizeX();

    virtual uint16_t screenSizeY();

    virtual uint16_t screenDiagonal();

    virtual uint8_t screenColourBits();

    virtual void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    virtual void dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);

    virtual void setPenSolid(bool flag = true);

    virtual void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    virtual void dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);

    virtual void point(uint16_t x1, uint16_t y1, uint16_t colour);

    virtual void selectFont(uint8_t font);

    virtual uint8_t getFont();

    virtual uint8_t addFont(font_s fontName);

    virtual void setFontSolid(bool flag = true);

    virtual void setFontSpaceX(uint8_t number = 1);

    virtual void setFontSpaceY(uint8_t number = 1);

    virtual uint16_t characterSizeX(uint8_t character = 0x00);

    virtual uint16_t characterSizeY();

    virtual uint16_t stringSizeX(String text);

    virtual uint8_t stringLengthToFitX(String text, uint16_t pixels);

    virtual uint8_t fontMax();

    virtual void gText(uint16_t x0, uint16_t y0,
                       String text,
                       uint16_t textColour = myColours.black,
                       uint16_t backColour = myColours.white);

    virtual void drawText(int x, int y, int fontSize, String msg, String color = "black");

    virtual void clearScreen();


  protected:

    virtual void _setOrientation(uint8_t orientation) = 0; // compulsory

    virtual bool _orientCoordinates(uint16_t & x1, uint16_t & y1) = 0; // compulsory

    virtual void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour) = 0; // compulsory

    void _triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);

    uint8_t _getCharacter(uint8_t character, uint8_t index);

    bool _penSolid, _flagRead, _flagStorage, _flagEnergy;
    uint16_t _screenSizeH, _screenSizeV, _screenDiagonal, _screenMarginH, _screenMarginV;
    uint8_t _orientation, _intensity;
    uint16_t _screenColourBits;
    /// @endcond
};

#endif // hV_SCREEN_BUFFER_RELEASE
