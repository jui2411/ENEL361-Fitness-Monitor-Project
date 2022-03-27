/*
 * accelerometer.h
 *
 * Header File for accelerometer
 *
 * Created on: 9/03/2020
 *     Author: Jia Shen Ui (27674749)
 *             Xin Tong Ow Yong (41511780)
 *             Azmelia Raihana Mohd Halmi (73414184)
 *
 *     Group Name: FitnessMonGroup14
 */


/*******************************************************
 *
 * accelerometer.h
 *
 * Support for interfacing with the ADXL345 accelerometer
 * on the Orbit BoosterPack.
 *
 * C.P. Moore
 * Last modified:  24/02/2020
 *
*******************************************************/


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "circBufT.h"


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

//Accl Interrupt Pins

#define ACCL_INT1Port       GPIO_PORTB_BASE
#define ACCL_INT2Port       GPIO_PORTE_BASE
#define ACCL_INT1           GPIO_PIN_4
#define ACCL_INT2           GPIO_PIN_4

#define ACCL                2
#define ACCL_ADDR           0x1D    //tap threshold = when acceleration goes below threshold then activate interrupt

#define ACCL_INT            0x2E
#define ACCL_OFFSET_X       0x1E
#define ACCL_OFFSET_Y       0x1F
#define ACCL_OFFSET_Z       0x20
#define ACCL_DATA_X0        0x32    //X-axis Data 0
#define ACCL_DATA_X1        0x33

#define ACCL_PWR_CTL        0x2D    //power control = ???
// Parameters for ACCL_PWR_CTL:
#define ACCL_MEASURE        0x08

#define ACCL_DATA_FORMAT    0x31
// Parameters for ACCL_DATA_FORMAT:
#define ACCL_RANGE_2G       0x00
#define ACCL_RANGE_4G       0x01
#define ACCL_RANGE_8G       0x02
#define ACCL_RANGE_16G      0x03
#define ACCL_FULL_RES       0x08
#define ACCL_JUSTIFY        0x04

#define ACCL_BW_RATE        0x2C
// Parameters for ACCL_BW_RATE:
#define ACCL_RATE_3200HZ    0x0F
#define ACCL_RATE_1600HZ    0x0E
#define ACCL_RATE_800HZ     0x0D
#define ACCL_RATE_400HZ     0x0C
#define ACCL_RATE_200HZ     0x0B
#define ACCL_RATE_100HZ     0x0A
#define ACCL_RATE_50HZ      0x09
#define ACCL_RATE_25HZ      0x08
#define ACCL_RATE_12_5HZ    0x07
#define ACCL_RATE_6_25HZ    0x06
#define ACCL_RATE_3_13HZ    0x05
#define ACCL_RATE_1_56HZ    0x04
#define ACCL_RATE_0_78HZ    0x03
#define ACCL_RATE_0_39HZ    0x02
#define ACCL_RATE_0_20HZ    0x01
#define ACCL_RATE_0_10HZ    0x00

uint32_t sampleCount;    // Counter for the interrupts

typedef struct myVector{
    int16_t x;
    int16_t y;
    int16_t z;
}vector3_t;

//void initClock (void);
void initAccl (void);
void getAcclData (void);
int16_t calcMean (int16_t acclSum);

#endif /*ACCELEROMETER_H_*/
