
// SDK
#include "hV_HAL_Peripherals.h"

// List of options
#include "hV_List_Options.h"

// List of constants
#include "hV_List_Constants.h"

// List of boards
#include "hV_List_Boards.h"

// List of screens
#include "hV_List_Screens.h"

// Checks
#if (hV_LIST_CONSTANTS_RELEASE < 700)
#error Required hV_LIST_CONSTANTS_RELEASE 700
#endif // hV_LIST_CONSTANTS_RELEASE

#if (hV_LIST_BOARDS_RELEASE < 700)
#error Required hV_LIST_BOARDS_RELEASE 700
#endif // hV_LIST_BOARDS_RELEASE

#if (hV_LIST_SCREENS_RELEASE < 702)
#error Required hV_LIST_SCREENS_RELEASE 702
#endif // hV_LIST_SCREENS_RELEASE

#ifndef hV_CONFIGURATION_RELEASE
///
/// @brief Release
///
#define hV_CONFIGURATION_RELEASE 702

///
/// @name 1- List of supported Pervasive Displays screens
/// @see hV_List_Screens.h
///

///
/// @name 2- List of pre-configured boards
/// @see hV_List_Boards.h
///

///
/// @name 3- Font mode, internal MCU or external SPI Flash
/// @name 4- Maximum number of fonts
/// @name 5- SRAM memory, internal MCU or external SPI
/// @name 6- Use self or virtual object
/// @name 7- Touch mode, activated or not
/// @name 8- Haptic feedback mode, activated or not
/// @name 9. Set GPIO expander mode, not implemented
/// @name 10. String object of char array options for string.
/// @name 11. Set storage mode, serial console by default
///
/// @see hV_List_Options.h
///

#endif // hV_CONFIGURATION_RELEASE

