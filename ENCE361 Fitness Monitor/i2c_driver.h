/*
 * i2c_driver.h
 *
 * Header File for i2c driver
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
 * i2c_driver.h
 *
 * Support for I2C helper functions on Tiva board.
 *
 * C.P. Moore
 * Last modified:  24/02/2020
 *
*******************************************************/



#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

// I2C Control

#define I2CPort         GPIO_PORTB_BASE
#define I2CSDAPort      GPIO_PORTB_BASE
#define I2CSCLPort      GPIO_PORTB_BASE
#define I2CSDA_PIN      GPIO_PIN_3
#define I2CSCL_PIN      GPIO_PIN_2
#define I2CSCL          GPIO_PB2_I2C0SCL
#define I2CSDA          GPIO_PB3_I2C0SDA
#define READ            1
#define WRITE           0

void Delay_us(void);
char I2CGenTransmit(char * pbData, int32_t cSize, bool fRW, char bAddr);
bool I2CGenIsNotIdle();

#endif /* I2C_DRIVER_H_ */
