/*
 * OLED.h
 *
 * OLED Header File
 *
 * Created on: 9/03/2020
 *     Author: Jia Shen Ui (27674749)
 *             Xin Tong Ow Yong (41511780)
 *             Azmelia Raihana Mohd Halmi (73414184)
 *
 *     Group Name: FitnessMonGroup14
 */


#include <stdint.h>
#ifndef DISPLAY_H_
#define DISPLAY_H_0

void initDisplay(void);

void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);

void displayUpdate2 (char *str1, int16_t num, int16_t mean, uint8_t charLine);

void displayVal(int16_t accMeanX, int16_t accMeanY, int16_t accMeanZ,  bool refOriIsSet);

void
displayInitializing(void);


enum DisplayStates {showAcc_raw = 0, showAcc_g = 1, showAcc_MSsquared = 2, showButtonClicks = 3};
enum DisplayStates displaySet;



#endif /* DISPLAY_H_ */
