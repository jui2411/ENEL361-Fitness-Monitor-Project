/*
 * OLED.c
 *
 * Main file for Display Functions
 * Display "Initializing"
 * Display Mean Value
 * Initializing display
 * Update Display
 *
 * Created on: 9/03/2020
 *     Author: Jia Shen Ui (27674749)
 *             Xin Tong Ow Yong (41511780)
 *             Azmelia Raihana Mohd Halmi (73414184)
 *
 *     Group Name: FitnessMonGroup14
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h" // Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/debug.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "OLED.h"
#include "accelerometer.h"
#include "buttons4.h"

/*********************************************************
 * Initializing Display
 *********************************************************/
void
initDisplay (void)
{
    // Initialise the Orbit OLED display
    OLEDInitialise ();
}

//*****************************************************************************
// Function to display a changing message on the display.
// The display has 4 rows of 16 characters, with 0, 0 at top left.
//*****************************************************************************
void
displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine)
{
    char text_buffer[17]; //Display fits 16 characters wide.

    // "Undraw" the previous contents of the line to be updated.
    OLEDStringDraw ("                ", 0, charLine);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf(text_buffer, sizeof(text_buffer), "%s %s %3d", str1, str2, num);
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, charLine);
}

void
displayVal(int16_t accMeanX, int16_t accMeanY, int16_t accMeanZ, bool refOriIsSet)
{
    // variables
    vector3_t acceleration_g;
    vector3_t acceleration_MSsquared;

    // Row 0 Display
    OLEDStringDraw ("                ", 0, 0);
    if (refOriIsSet) {
        OLEDStringDraw ("Ref Ori is Set", 0, 0);
    } else {
        if(displaySet == showAcc_raw) {
            OLEDStringDraw ("Acc in raw", 0, 0);
        } else if (displaySet == showAcc_g) {
            OLEDStringDraw ("Acc in g", 0, 0);
        } else if (displaySet == showAcc_MSsquared) {
            OLEDStringDraw ("Acc in ms^-2 ", 0, 0);
        }
    }

    // Acceleration Display

    if(displaySet == showAcc_raw) {

        // in raw
        displayUpdate ("Accl", "X", accMeanX, 1);
        displayUpdate ("Accl", "Y", accMeanY, 2);
        displayUpdate ("Accl", "Z", accMeanZ, 3);

    } else if (displaySet == showAcc_g) {

        // in g
        acceleration_g.x = (accMeanX) / 249;
        acceleration_g.y = (accMeanY) / 249;
        acceleration_g.z = (accMeanZ) / 249;
        displayUpdate ("Accl", "X", acceleration_g.x, 1);
        displayUpdate ("Accl", "Y", acceleration_g.y, 2);
        displayUpdate ("Accl", "Z", acceleration_g.z, 3);

    } else if (displaySet == showAcc_MSsquared) {

        // in ms^-2
        acceleration_MSsquared.x = (accMeanX * 981) / 249;
        acceleration_MSsquared.y = (accMeanY * 981) / 249;
        acceleration_MSsquared.z = (accMeanZ * 981) / 249;
        displayUpdate ("Accl", "X", acceleration_MSsquared.x, 1);
        displayUpdate ("Accl", "Y", acceleration_MSsquared.y, 2);
        displayUpdate ("Accl", "Z", acceleration_MSsquared.z, 3);
    }



}

//display "Initializing"
void
displayInitializing(void) {
    OLEDStringDraw ("                ", 0, 0);
    OLEDStringDraw ("INITIALIZING", 0, 0);
}


