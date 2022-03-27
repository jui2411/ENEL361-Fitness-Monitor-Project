/*
 * accelerometer.c
 *
 * Initialize clock & accelerometer
 * Getting Data from Accelerometer
 * Calculation of Mean
 *
 * Created on: 9/03/2020
 *     Author: Jia Shen Ui (27674749)
 *             Xin Tong Ow Yong (41511780)
 *             Azmelia Raihana Mohd Halmi (73414184)
 *
 *     Group Name: FitnessMonGroup14
 */

#include "accelerometer.h"
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
#include "circBufT.h"
#include "OLED.h"
#include "buttons4.h"


/*********************************************************
 * Initializing Accelerometer
 *********************************************************/
void
initAccl (void)
{
    char    toAccl[] = {0, 0};  // parameter, value , acts as a buffer so that multiple data can be transmitted at the same time

    /*
     * Enable I2C Peripheral
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    /*
     * Set I2C GPIO pins
     */
    GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
    GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
    GPIOPinConfigure(I2CSCL);
    GPIOPinConfigure(I2CSDA);

    /*
     * Setup I2C
     */
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true); // Initialise the I2C Master block by configuring the bus speed for the master

    GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);

    //Initialize ADXL345 Accelerometer

    toAccl[0] = ACCL_DATA_FORMAT;
    toAccl[1] = (ACCL_RANGE_2G | ACCL_FULL_RES);  // set +-2g, 13 bit resolution, active low interrupts
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR); // communication between i2c to accelerometer

    toAccl[0] = ACCL_PWR_CTL; // power control
    toAccl[1] = ACCL_MEASURE;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);


    toAccl[0] = ACCL_BW_RATE; // band width
    toAccl[1] = ACCL_RATE_100HZ;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_INT;
    toAccl[1] = 0x00;       // Disable interrupts from accelerometer.
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_X;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Y;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Z;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);
}

/********************************************************
 * Function to read accelerometer with circular buffers
 ********************************************************/
void
getAcclData (void)
{

    char    fromAccl[] = {0, 0, 0, 0, 0, 0, 0}; // starting address, place holders for data to be read. 2 bytes of data per axis thus 6 ... each byte is 8 bits
    vector3_t acceleration;
    uint8_t bytesToRead = 6;
    sampleCount ++;

    fromAccl[0] = ACCL_DATA_X0; // x-axis data 0 , puts in a location to start with
    I2CGenTransmit(fromAccl, bytesToRead, READ, ACCL_ADDR); //receiving from the tap threshold and reads only 6 bytes .. if correct, the tap threshold collects acc data

    acceleration.x = (fromAccl[2] << 8) | fromAccl[1]; // Return 16-bit/2 bytes acceleration readings. get mean sample value
    acceleration.y = (fromAccl[4] << 8) | fromAccl[3]; //pushing it to read the first byte of the axis
    acceleration.z = (fromAccl[6] << 8) | fromAccl[5];

    writeCircBuf(&acc_buffers.x_circBuff, acceleration.x); //sending it to the circular buffer
    writeCircBuf(&acc_buffers.y_circBuff, acceleration.y);
    writeCircBuf(&acc_buffers.z_circBuff, acceleration.z);

}

/**********************************************************
 * Function to calculate the mean sample acceleration value
 *********************************************************/
int16_t
calcMean (int16_t acclSum)
{
    int16_t acclMean;
    //display the rounded mean of the buffer contents
    acclMean = ((2 * acclSum + BUF_SIZE) / 2 / BUF_SIZE);

    return acclMean;
}


