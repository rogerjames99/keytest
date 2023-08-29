/** @file keyboard.h
*/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>
#include <elapsedMillis.h>

/**
 * @brief Declaration of CKeyboard class.
*/
class CKeyboard
{
public:
    static const uint8_t KEY_MATRIX_ROWS = 6;
    static const uint8_t KEY_MATRIX_COLUMNS = 8;
    static const uint8_t SCAN_IN_PROGRESS_DIAGNOSTIC_LINE = 38;
    static CKeyboard *getInstance();
    void scan();
protected:
    const uint16_t KEY_SCAN_INTERVAL_MS = 5000;
    elapsedMillis key_scan_interval;
    /** @typedef
     * The keyboard matrix type is used the hold the state of the keyboards.
     * It is a two dimensional matrix indexed by the row and column results
     * returned from the keyboard scan.
    */
    typedef bool keyboard_matrix_t[KEY_MATRIX_ROWS][KEY_MATRIX_COLUMNS];
    keyboard_matrix_t current_keyboard_matrix;
    keyboard_matrix_t previous_keyboard_matrix = {
            {false, false, false, false, false, false, false, false},
            {false, false, false, false, false, false, false, false},
            {false, false, false, false, false, false, false, false},
            {false, false, false, false, false, false, false, false},
            {false, false, false, false, false, false, false, false},
            {false, false, false, false, false, false, false, false}
        };
    CKeyboard() {};
    static CKeyboard* singleton;


    CKeyboard(CKeyboard &other) = delete;
    void operator = (const CKeyboard &) = delete;

};
#endif // KEYBOARD_H
