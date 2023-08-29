/** @file keyboard.cpp
*/
#include "keyboard.h"
#include <ArduinoLog.h>

CKeyboard* CKeyboard::singleton = nullptr;

CKeyboard* CKeyboard::getInstance()
{
    if (singleton == nullptr)
        singleton = new CKeyboard();
    return singleton;
}

/** @page keyboardmatrices Keyboard matrices
 * The keyboard is arranged as an 8 columns by 6 rows switch matrix.
 * 
 * The columns are addressed by a 3 bit number sent to the GPIO output pins 26, 25 and 24 (MSB first).
 * The column values are read back as 6 bit number from the GPIO input pins 32, 31, 30, 29, 28 and 27 (MSB first).
 * The input pins are pulled high by default so a pressed key is indicated by a zero bit value.
 * 
 * The keyboard matrix columns are scanned sequentially. For each column, bits 0, 1 and 2 of the six bit value
 * returned give the state of the switches in the top, middle and bottom rows of the left hand keyboard, bits
 * 3, 4 an 5 give the state of the switches in the top, middle and bottom rows of the right hand keyboard.
 * 
 * When the intial scan has completed the new switch status matrix is compared with a saved copy. Two passes through
 * the matrix are performed. The first pass checks for switches have turned from on to off. These are processed before
 * next pass is made. The second pass checks for switches that have turned from off to on.
 * 
 * The two passes are made in this way in order to ensure maximum availability of instrument voices for playing
 * new notes.
 * 
*/
void CKeyboard::scan()
{
    const uint8_t output_lines[] = {24, 25, 26};
    const uint8_t input_lines[] = {32, 31, 30, 29, 28, 27};

    bool matrix_changed = false;

    if (key_scan_interval > KEY_SCAN_INTERVAL_MS)
    {
    //    digitalWrite(SCAN_IN_PROGRESS_DIAGNOSTIC_LINE, 1);
        for (uint8_t matrix_column = 0; matrix_column < 8; matrix_column++)
        {
            // Set up the keyboard scan address
            for (uint8_t line_index = 0; line_index < 3; line_index++)
            {
                uint8_t bit = (matrix_column >> line_index) & 1;
                //Log.verbose("Keyboard scan setup matrix_column %d line_index %d line %d bit %d\n", matrix_column, line_index, output_lines[line_index], bit);
                digitalWrite(output_lines[line_index], bit);
            }

            // Wait for the switch matrix to settle
            delayMicroseconds(10);
            
            // Read the input lines
            for (uint8_t matrix_row = 0; matrix_row < 6; matrix_row++)
            {
                current_keyboard_matrix[matrix_row][matrix_column] = !bool(digitalRead(input_lines[matrix_row]));
                /*Log.verbose("Keyboard scan read matrix_row %d  matrix_column %d result %s\n",
                            matrix_row,
                            matrix_column,
                            current_keyboard_matrix[matrix_row][matrix_column] ? "true" : "false");*/
            }
        }

        for (uint8_t matrix_column = 0; matrix_column < 8; matrix_column++)
        {
            for (uint8_t matrix_row = 0; matrix_row < 6; matrix_row++)
            {
                if (current_keyboard_matrix[matrix_row][matrix_column] !=
                        previous_keyboard_matrix[matrix_row][matrix_column])
                {
                    previous_keyboard_matrix[matrix_row][matrix_column] = current_keyboard_matrix[matrix_row][matrix_column];
                    matrix_changed = true;
                }
            }
        }

        if (matrix_changed)
        {
            Log.verbose("current_keyboard_matrix\n");
            for (uint8_t matrix_row = 0; matrix_row < 6; matrix_row++)
            {
                Log.verbose("%d %d %d %d %d %d %d %d\n",
                    current_keyboard_matrix[matrix_row][0],
                    current_keyboard_matrix[matrix_row][1],
                    current_keyboard_matrix[matrix_row][2],
                    current_keyboard_matrix[matrix_row][3],
                    current_keyboard_matrix[matrix_row][4],
                    current_keyboard_matrix[matrix_row][5],
                    current_keyboard_matrix[matrix_row][6],
                    current_keyboard_matrix[matrix_row][7]);
            }
        }

        key_scan_interval = key_scan_interval - KEY_SCAN_INTERVAL_MS;
    }
}
