//
// hV_Screen_Buffer.cpp
// Class library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
//
// See hV_Screen_Buffer.h for references
//
// Release 520: Added use of hV_HAL_Peripherals
// Release 523: Fixed rounded rectangles
// Release 526: Improved touch management
// Release 700: Refactored screen and board functions
//

// Library header
#include "hV_Screen_Buffer.h"
//#include "QuickDebug.h"

// Code
hV_Screen_Buffer::hV_Screen_Buffer()
{
    f_fontSize = 0;
    f_fontNumber = 0;
    f_fontSolid = true;
    f_fontSpaceX = 1;
    _penSolid = false;
}

void hV_Screen_Buffer::begin()
{
    f_begin(); // hV_font_...
}

void hV_Screen_Buffer::clear(uint16_t colour)
{
    uint8_t oldOrientation = _orientation;
    bool oldPenSolid = _penSolid;
    setOrientation(0);
    setPenSolid();
    rectangle(0, 0, screenSizeX() - 1, screenSizeY() - 1, colour);
    setOrientation(oldOrientation);
    setPenSolid(oldPenSolid);
}

void hV_Screen_Buffer::flush()
{
    ;
}

void hV_Screen_Buffer::setOrientation(uint8_t orientation)
{
    switch (orientation)
    {
        case 6:

            _orientation = 0;
            _setOrientation(_orientation);
            if (screenSizeX() > screenSizeY())
            {
                _orientation = 1;
                _setOrientation(_orientation);
            }
            break;

        case 7:

            _orientation = 0;
            _setOrientation(_orientation);
            if (screenSizeX() < screenSizeY())
            {
                _orientation = 1;
                _setOrientation(_orientation);
            }
            break;

        default:

            _orientation = orientation % 4;
            _setOrientation(_orientation);
            break;
    }
}

uint8_t hV_Screen_Buffer::getOrientation()
{
    return _orientation;
}

uint16_t hV_Screen_Buffer::screenSizeX()
{
    switch (_orientation)
    {
        case 1:
        case 3:

            return _screenSizeV; // _maxX
            break;

        // case 0:
        // case 2:
        default:

            return _screenSizeH; // _maxX
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenSizeY()
{
    switch (_orientation)
    {
        case 1:
        case 3:

            return _screenSizeH; // _maxY
            break;

        // case 0:
        // case 2:
        default:

            return _screenSizeV; // _maxY
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenDiagonal()
{
    return _screenDiagonal;
}

uint8_t hV_Screen_Buffer::screenColourBits()
{
    return _screenColourBits;
}

void hV_Screen_Buffer::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    line(x0, y0, x0 + dx - 1, y0 + dy - 1, colour);
}

void hV_Screen_Buffer::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if ((x1 == x2) and (y1 == y2))
    {
        _setPoint(x1, y1, colour);
    }
    else if (x1 == x2)
    {
        if (y1 > y2)
        {
            swap(y1, y2);
        }
        for (uint16_t y = y1; y <= y2; y++)
        {
            _setPoint(x1, y, colour);
        }
    }
    else if (y1 == y2)
    {
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        for (uint16_t x = x1; x <= x2; x++)
        {
            _setPoint(x, y1, colour);
        }
    }
    else
    {
        int16_t wx1 = (int16_t)x1;
        int16_t wx2 = (int16_t)x2;
        int16_t wy1 = (int16_t)y1;
        int16_t wy2 = (int16_t)y2;

        bool flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if (flag)
        {
            swap(wx1, wy1);
            swap(wx2, wy2);
        }

        if (wx1 > wx2)
        {
            swap(wx1, wx2);
            swap(wy1, wy2);
        }

        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;

        if (wy1 < wy2)
        {
            ystep = 1;
        }
        else
        {
            ystep = -1;
        }

        for (; wx1 <= wx2; wx1++)
        {
            if (flag)
            {
                _setPoint(wy1, wx1, colour);
            }
            else
            {
                _setPoint(wx1, wy1, colour);
            }

            err -= dy;
            if (err < 0)
            {
                wy1 += ystep;
                err += dx;
            }
        }
    }
}

void hV_Screen_Buffer::setPenSolid(bool flag)
{
    _penSolid = flag;
}

void hV_Screen_Buffer::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}

void hV_Screen_Buffer::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (_penSolid == false)
    {
        line(x1, y1, x1, y2, colour);
        line(x1, y1, x2, y1, colour);
        line(x1, y2, x2, y2, colour);
        line(x2, y1, x2, y2, colour);
    }
    else
    {
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }
        for (uint16_t x = x1; x <= x2; x++)
        {
            for (uint16_t y = y1; y <= y2; y++)
            {
                _setPoint(x, y, colour);
            }
        }
    }
}

void hV_Screen_Buffer::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    rectangle(x0, y0, x0 + dx - 1, y0 + dy - 1, colour);
}

void hV_Screen_Buffer::_triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    int16_t wx1 = (int16_t)x1;
    int16_t wy1 = (int16_t)y1;
    int16_t wx2 = (int16_t)x2;
    int16_t wy2 = (int16_t)y2;
    int16_t wx3 = (int16_t)x3;
    int16_t wy3 = (int16_t)y3;
    int16_t wx4 = wx1;
    int16_t wy4 = wy1;
    int16_t wx5 = wx1;
    int16_t wy5 = wy1;

    bool changed1 = false;
    bool changed2 = false;

    int16_t dx1 = abs(wx2 - wx1);
    int16_t dy1 = abs(wy2 - wy1);

    int16_t dx2 = abs(wx3 - wx1);
    int16_t dy2 = abs(wy3 - wy1);

    int16_t signx1 = (wx2 >= wx1) ? +1 : -1;
    int16_t signx2 = (wx3 >= wx1) ? +1 : -1;

    int16_t signy1 = (wy2 >= wy1) ? +1 : -1;
    int16_t signy2 = (wy3 >= wy1) ? +1 : -1;

    if (dy1 > dx1)
    {
        swap(dx1, dy1); // swap values
        changed1 = true;
    }

    if (dy2 > dx2)
    {
        swap(dx2, dy2); // swap values
        changed2 = true;
    }

    int16_t e1 = 2 * dy1 - dx1;
    int16_t e2 = 2 * dy2 - dx2;

    for (int i = 0; i <= dx1; i++)
    {
        line(wx4, wy4, wx5, wy5, colour);

        while (e1 >= 0)
        {
            if (changed1)
            {
                wx4 += signx1;
            }
            else
            {
                wy4 += signy1;
            }
            e1 = e1 - 2 * dx1;
        }

        if (changed1)
        {
            wy4 += signy1;
        }
        else
        {
            wx4 += signx1;
        }

        e1 = e1 + 2 * dy1;

        while (wy5 != wy4)
        {
            while (e2 >= 0)
            {
                if (changed2)
                {
                    wx5 += signx2;
                }
                else
                {
                    wy5 += signy2;
                }
                e2 = e2 - 2 * dx2;
            }

            if (changed2)
            {
                wy5 += signy2;
            }
            else
            {
                wx5 += signx2;
            }

            e2 = e2 + 2 * dy2;
        }
    }
}

// Font functions
void hV_Screen_Buffer::setFontSolid(bool flag)
{
    f_setFontSolid(flag);
}

uint8_t hV_Screen_Buffer::addFont(font_s fontName)
{
    return f_addFont(fontName);
}

void hV_Screen_Buffer::selectFont(uint8_t font)
{
    f_selectFont(font);
}

uint8_t hV_Screen_Buffer::getFont()
{
    return f_fontSize;
}

uint8_t hV_Screen_Buffer::fontMax()
{
    return f_fontMax();
}

uint16_t hV_Screen_Buffer::characterSizeX(uint8_t character)
{
    uint16_t result = 0;
    if ((f_font.kind & 0x40) == 0x40) // Monospaced font
    {
        result = f_font.maxWidth + f_fontSpaceX;
    }
    else
    {
        result = f_characterSizeX(character);
    }

    return result;
}

uint16_t hV_Screen_Buffer::characterSizeY()
{
    return f_characterSizeY();
}

uint16_t hV_Screen_Buffer::stringSizeX(String text)
{
    return f_stringSizeX(text);
}

uint8_t hV_Screen_Buffer::stringLengthToFitX(String text, uint16_t pixels)
{
    return f_stringLengthToFitX(text, pixels);
}

void hV_Screen_Buffer::setFontSpaceX(uint8_t number)
{
    f_setFontSpaceX(number);
}

void hV_Screen_Buffer::setFontSpaceY(uint8_t number)
{
    f_setFontSpaceY(number);
}

uint8_t hV_Screen_Buffer::_getCharacter(uint8_t character, uint8_t index)
{
    return f_getCharacter(character, index);
}

void hV_Screen_Buffer::gText(uint16_t x0, uint16_t y0,
                             String text,
                             uint16_t textColour,
                             uint16_t backColour)
#if (FONT_MODE == USE_FONT_TERMINAL)
{
    uint8_t c;
    uint8_t line, line1, line2, line3;
    uint16_t x, y;
    uint8_t i, j, k;
#if (MAX_FONT_SIZE > 0)
    if (f_fontSize == 0)
    {
        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';

            for (i = 0; i < 6; i++)
            {
                line = f_getCharacter(c, i);

                for (j = 0; j < 8; j++)
                    if (bitRead(line, j))
                    {
                        point(x0 + 6 * k + i, y0 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 6 * k + i, y0 + j, backColour);
                    }
            }
        }
    }
#if (MAX_FONT_SIZE > 1)
    else if (f_fontSize == 1)
    {
        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';

            for (i = 0; i < 8; i++)
            {
                line = f_getCharacter(c, 2 * i);
                line1 = f_getCharacter(c, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 8 * k + i, y0 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 8 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 8 * k + i, y0 + 8 + j, textColour);
                    }
                    else if ((f_fontSolid) and (j < 4))
                    {
                        point(x0 + 8 * k + i, y0 + 8 + j, backColour);
                    }
                }
            }
        }
    }
#if (MAX_FONT_SIZE > 2)
    else if (f_fontSize == 2)
    {

        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';

            for (i = 0; i < 12; i++)
            {
                line = f_getCharacter(c, 2 * i);
                line1 = f_getCharacter(c, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 12 * k + i, y0 + j,    textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 12 * k + i, y0 + j,    backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 12 * k + i, y0 + 8 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 12 * k + i, y0 + 8 + j, backColour);
                    }
                }
            }
        }
    }
#if (MAX_FONT_SIZE > 3)
    else if (f_fontSize == 3)
    {
        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';
            for (i = 0; i < 16; i++)
            {
                line  = f_getCharacter(c, 3 * i);
                line1 = f_getCharacter(c, 3 * i + 1);
                line2 = f_getCharacter(c, 3 * i + 2);
                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 16 * k + i, y0 + j,     textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + j,     backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 16 * k + i, y0 + 8 + j,  textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + 8 + j,  backColour);
                    }
                    if (bitRead(line2, j))
                    {
                        point(x0 + 16 * k + i, y0 + 16 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + 16 + j, backColour);
                    }
                }
            }
        }
    }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
}
#endif // FONT_MODE

void hV_Screen_Buffer::drawText(int x, int y, int fontSize, String msg, String color) {
  setOrientation(7);
  if(fontSize == 1) selectFont(Font_Terminal6x8);
  else if(fontSize == 2) selectFont(Font_Terminal8x12);
  else if(fontSize == 3) selectFont(Font_Terminal12x16);
  else selectFont(Font_Terminal16x24);
  if(color == "red") gText(x, y, msg, myColours.red);
  else gText(x, y, msg, myColours.black);
}

void hV_Screen_Buffer::clearScreen() {
    clear(myColours.white);
    flush();
}

