/*
 * fitnessMonitor.c
 *
 * Top level file
 * Contains main function
 * Connects all files together to operate as a fitness monitor
 *
 *  Created on: 9/03/2020
 *      Author: Jia Shen Ui (27674749)
 *              Xin Tong Ow Yong (41511780)
 *              Azmelia Raihana Mohd Halmi (73414184)
 *
 *     Group Name: FitnessMonGroup14
 */

#include <accelerometer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "i2c_driver.h"
#include "OLED.h"
#include "buttons4.h"
#include "accelerometer.h"
#include "circBufT.h"




/********************************************************
 * main
 ********************************************************/
int
main (void)
{

    //initializing
    initClock ();
    initAccl ();
    initDisplay ();
    initButtons ();
    initCircBuf (&acc_buffers.x_circBuff, BUF_SIZE);
    initCircBuf (&acc_buffers.y_circBuff, BUF_SIZE);
    initCircBuf (&acc_buffers.z_circBuff, BUF_SIZE);

    // variables
    vector3_t accMean;
    vector3_t refOrientation;
    bool refOrientationIsSet = false; // if a reference orientation is set
    bool ref_Flag = false;

    // set to default
    refOrientation.x = 0;
    refOrientation.y = 0;
    refOrientation.z = 0;
    int16_t counter = 0;
    int16_t refCounter = 0;
    displaySet = showAcc_raw;

    while (1)
    {
        // Counters
        counter++; // used for the buffer delay ("Initializing" display)
        refCounter++;
        getAcclData();

        //Sample Values Sums
        vector3_t accSum;
        accSum.x = 0;
        accSum.y = 0;
        accSum.z = 0;
        uint16_t i;
        for (i = 0; i < BUF_SIZE; i++) {
            //read sum of individual buffers
            accSum.x = accSum.x + readCircBuf(&acc_buffers.x_circBuff);
            accSum.y = accSum.y + readCircBuf(&acc_buffers.y_circBuff);
            accSum.z = accSum.z + readCircBuf(&acc_buffers.z_circBuff);
        }


        //calculating Means of acceleration values
        accMean.x = calcMean(accSum.x);
        accMean.y = calcMean(accSum.y);
        accMean.z = calcMean(accSum.z);

        //Program Initiation Manager
        if (!refOrientationIsSet && counter > BUFFER_DELAY) {
            refOrientation.x = accMean.x;
            refOrientation.y = accMean.y;
            refOrientation.z = accMean.z;
            refOrientationIsSet  = true;
            refCounter = 0;
            ref_Flag = true;
        }

        //Reference Orientation Display Manager
        if(ref_Flag && refCounter > REF_DELAY) {
            ref_Flag = false;
        }

        //Display and Button Handler
        if(!refOrientationIsSet && !ref_Flag) {
            displayInitializing();
        } else {

            updateButtons();
            butState = checkButton (UP);

            switch (butState)
            {
                case PUSHED:
                    if(displaySet == showAcc_raw) {
                        displaySet = showAcc_g;
                    } else if (displaySet == showAcc_g){
                        displaySet = showAcc_MSsquared;
                    } else if (displaySet == showAcc_MSsquared) {
                        displaySet = showAcc_raw;
                    }
                    displayVal(accMean.x, accMean.y, accMean.z, ref_Flag);
                    break;
                case RELEASED:
                    displayVal(accMean.x, accMean.y, accMean.z, ref_Flag);
                    break;
                case NO_CHANGE:
                    displayVal(accMean.x, accMean.y, accMean.z, ref_Flag);
                    break;
            }

            updateButtons();
            butState = checkButton (DOWN);

            switch (butState)
            {
                case PUSHED:
                    refOrientationIsSet  = false;
                    break;
            }

        }

        //Refresh Screen
        SysCtlDelay (SysCtlClockGet () / 40);

    }
}

