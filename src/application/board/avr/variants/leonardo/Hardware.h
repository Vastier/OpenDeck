/*
    OpenDeck MIDI platform firmware
    Copyright (C) 2015-2018 Igor Petrovic

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

namespace Board
{
    namespace detail
    {
        ///
        /// \brief Constant used to debounce button readings.
        ///
        #define BUTTON_DEBOUNCE_COMPARE         0b11110000
    }

    ///
    /// brief Total number of analog components.
    ///
    #ifdef BOARD_A_PRO_MICRO
    #define MAX_NUMBER_OF_ANALOG            4
    #else
    #define MAX_NUMBER_OF_ANALOG            6
    #endif

    ///
    /// \brief Maximum number of buttons.
    ///
    #ifdef BOARD_A_PRO_MICRO
    #define MAX_NUMBER_OF_BUTTONS           6
    #else
    #define MAX_NUMBER_OF_BUTTONS           8
    #endif

    ///
    /// \brief Maximum number of LEDs.
    ///
    #define MAX_NUMBER_OF_LEDS              6

    ///
    /// \brief Maximum number of RGB LEDs.
    /// One RGB LED requires three standard LED connections.
    ///
    #define MAX_NUMBER_OF_RGB_LEDS          (MAX_NUMBER_OF_LEDS/3)

    ///
    /// \brief Maximum number of encoders.
    /// Total number of encoders is total number of buttons divided by two.
    ///
    #define MAX_NUMBER_OF_ENCODERS          (MAX_NUMBER_OF_BUTTONS/2)

    ///
    /// \brief If touchscreen isn't supported, set MAX_TOUCHSCREEN_BUTTONS to zero.
    ///
    #define MAX_TOUCHSCREEN_BUTTONS         0
}