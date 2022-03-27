/*
 * FitnessMonitor2.c
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
#include "system.h"
#include "OLED.h"
#include "buttons4.h"
#include "accelerometer.h"
#include "circBufT.h"
#include "math.h"
#include "inc/tm4c123gh6pm.h"

#define THRESHOLD_C 15

/********************************************************
 * Global Variables
 ********************************************************/
vector3_t accMean;
static vector3_t accSum; //vector 3 for summation used for averagings
vector3_t acceleration_g;

//flags
bool testMode_Flag; //flag for test mode
bool stepFlag = true; // flag for threshold crossing, set true to make sure step isnt set to 1 at beginning
bool inputReady = false; //true when initialization is complete

/********************************************************
 *Background Processing
 ********************************************************/

//Counter used for detecting a long press of the down button
void counterHandler (void) {
    if(downHoldFlag) {
        downHoldCounter++;
    }

    if(downHoldCounter >= DOWN_HOLD_DELAY) {
        if(displaySet == showSteps) {
            stepCount = 0;
        } else if (displaySet == showDisT) {
            distancetravelled = 0;
            distStp = 0;
        }
    }
}

void initAll (void) {
    //initializing modules
    initClock ();
    initDisplayTimerInt(DisplayTimerIntHandler);
    initAccl ();
    initDisplay ();
    initButtons ();
    initSwitches();
    initCircBuf (&acc_buffers.x_circBuff, BUF_SIZE);
    initCircBuf (&acc_buffers.y_circBuff, BUF_SIZE);
    initCircBuf (&acc_buffers.z_circBuff, BUF_SIZE);

    //initialize variables
    displaySet = showSteps;//showAcc_raw;
    curDistUnit = kiloM;
    testMode_Flag = false;
    stepCount = 0;
    distancetravelled = 0;
    downHoldFlag = false;
    downHoldCounter = 0;
    distStp = 0;
    acc_vector = 0;
    inputReady = true;

}

//reset summation of acceleration data
void resetAccSum() {
    //Sample Values Sums
    accSum.x = 0;
    accSum.y = 0;
    accSum.z = 0;
}

//function for getting summation of acceleration data
void getAcclSum() {
    uint16_t i;
    for (i = 0; i < BUF_SIZE; i++) {
        //read sum of individual buffers
        accSum.x = accSum.x + readCircBuf(&acc_buffers.x_circBuff);
        accSum.y = accSum.y + readCircBuf(&acc_buffers.y_circBuff);
        accSum.z = accSum.z + readCircBuf(&acc_buffers.z_circBuff);
    }
}

//calculating Means of acceleration values
void getAcclMean() {
    accMean.x = calcMean(accSum.x);
    accMean.y = calcMean(accSum.y);
    accMean.z = calcMean(accSum.z);
}

//Detect inputs (Buttons, Switches) and execute their functionality
void inputHandler() {
    //switches
    if(checkSwitches(SW1_PIN) != SW1_PIN) { //SW1 is down
        testMode_Flag = false;
    } else if(checkSwitches(SW1_PIN) == SW1_PIN) {//SW1 is up
        testMode_Flag = true;
    }

    displayTestMode(testMode_Flag);

    //UP Button
    updateButtons();
        butState = checkButton (UP);
    switch (butState)
    {
        case PUSHED:
            if(!testMode_Flag) {
                if(displaySet == showDisT) {
                    if(curDistUnit == kiloM) {
                        curDistUnit = miles;
                    } else if ( curDistUnit == miles){
                        curDistUnit = kiloM;
                    }
                }
            } else if(testMode_Flag) {
                distancetravelled += (0.09 * 100000); //0.09km convert to cm
                stepCount += 100;
            }
            break;
    }

    //down button
    updateButtons();
    butState = checkButton (DOWN);
    //Down Button Change State
    switch (butState)
    {
        case PUSHED:

            if(!testMode_Flag) {
                //refOrientationIsSet  = false;
                downHoldFlag = true; // counter for checking down button hold delay
            } else {
                distancetravelled -= (0.045 * 100000); //0.45km, convert to cm
                stepCount -= 500;
            }
            break;
        case RELEASED:
            downHoldFlag = false;
            downHoldCounter = 0;
            break;
    }

    //left button
    updateButtons();

    butState = checkButton (LEFT);
    switch (butState)
    {
        case PUSHED:
            if(displaySet == showSteps) {
                displaySet = showDisT;
            } else if (displaySet == showDisT) {
                displaySet = showSteps;
            }
            break;
    }

    //right button
    updateButtons();
    butState = checkButton (RIGHT);
    switch (butState)
    {
        case PUSHED:
            if(displaySet == showSteps) {
                displaySet = showDisT;
            } else if (displaySet == showDisT) {
                displaySet = showSteps;
            }
            break;
    }
}

//Fitness Monitor main operation to calculate vector of given acceleration data and to record steps, estimate steps
//and display data on the Orbital LED
void pedometerHandler() {
    acceleration_g.x = (accMean.x * 10) / 225;
    acceleration_g.y = (accMean.y * 10) / 225;
    acceleration_g.z = (accMean.z * 10) / 225;
    acc_vector = sqrt(acceleration_g.x^2 + acceleration_g.y^2 + acceleration_g.z^2);
    //threshold crossing
    if(acc_vector > (THRESHOLD_C/10) && !stepFlag) {
        stepCount++;
        distStp++;
        distancetravelled += (0.0009 * 100000 * distStp); //displayed in cm, 0.9m 0.0009km
        stepFlag = 1;
    }

    if(acc_vector < (THRESHOLD_C/10) && stepFlag) {
        stepFlag = 0;
    }
}


/********************************************************
 * main
 ********************************************************/
int
main (void)
{

    //Program Initiation Manager
    initAll();

    while (1)
    {
        counterHandler();
        getAcclData();
        resetAccSum();
        getAcclSum();
        getAcclMean();

        if(inputReady){
            inputHandler();
            pedometerHandler();
        }

        //Refresh Screen
        SysCtlDelay (SysCtlClockGet () / 40);

    }
}
