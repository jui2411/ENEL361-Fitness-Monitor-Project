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
#include <stdbool.h>
#ifndef DISPLAY_H_
#define DISPLAY_H_0

void initDisplay(void);
void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);
void displayTestMode(bool testMode_Flag);
void displayPedometer(int16_t stepCount,int16_t acc_vector, int16_t distancetravelled);

enum DisplayStates {showSteps, showDisT};
enum DisplayStates displaySet;

enum DistUnit {kiloM = 0, miles = 1};
enum DistUnit curDistUnit;

#endif /* DISPLAY_H_ */
