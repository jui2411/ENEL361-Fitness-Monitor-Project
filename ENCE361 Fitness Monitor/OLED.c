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


void displayPedometer(int16_t stepCount,int16_t acc_vector, int16_t distancetravelled) {
    OLEDStringDraw ("                ", 0, 1);
    OLEDStringDraw ("                ", 0, 2);

    if(displaySet == showSteps) {
        displayUpdate ("Step", ":", stepCount, 1);

    } else if (displaySet == showDisT) {

        if(curDistUnit == kiloM) {
            OLEDStringDraw ("Meas in km", 0, 2);
        } else if (curDistUnit == miles) {
            distancetravelled /= 1.609; //for m to miles
            OLEDStringDraw ("Meas in miles", 0, 2);
        }

        displayUpdate ("Dist", ":", distancetravelled, 1);
    }



}


void displayTestMode(bool testMode_Flag) {
    if(testMode_Flag) {
        OLEDStringDraw ("                ", 0, 0);
        OLEDStringDraw ("TEST MODE", 0, 0);
    } else {
        OLEDStringDraw ("                ", 0, 0);
    }
}

