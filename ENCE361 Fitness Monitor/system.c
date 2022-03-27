/*
 * system.c
 *
 * * Clock and Timer file
 * Contains Clock and Timer Interrupts
 * Foreground Tasks for the program
 * Display Timer is used to update display regularly using timer interrupt
 *
 * NOTE: for it to work, it requires user to open tm4c123gh6pm_startup_ccs_gcc.c
 * and changing the line with comment "Timer 1 subtimer A" to "DisplayTimerIntHandler,",
 * and also declaring "extern void DisplayTimerIntHandler(void); at the top of the file,
 * necessary for the compiler to resolve this symbol
 *
 *  Created on: 22/05/2020
 *      Author: Jason Ui
 */
#include "system.h"
#include "OLED.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "inc/tm4c123gh6pm.h"

#define DISPLAY_TIMER_PERIPH SYSCTL_PERIPH_TIMER1
#define DISPLAY_TIMER_BASE TIMER1_BASE
#define DISPLAY_TIMER TIMER_A
#define PRESCALER 10

/**********************************************************
 * Variables
 **********************************************************/

uint32_t ui32SysClock;
uint16_t DisplayTimerIntFlag;

/***********************************************************
* Initialization functions: clock, Timer
***********************************************************
* Clock
***********************************************************/
void
initClock (void)
{
// Dividing the clock rate by 10 to set the clock rate to 20 MHz | Use of PLL, phase locked-loop | Main Oscillator |
// USB interface of 16MHz, as the communication with the Tiva board need to be in the same frequency as the computer, to make sure data is sent correctly.
// PLL is used to allow one circuit board to synchronize the phase of its on board clock with an external timing signal.
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}
//*****************************************************************************
//
// The interrupt handler for the Timer1A interrupt.
//
//*****************************************************************************

// Configure Timer1A as a 16-bit periodic counter
void initDisplayTimerInt(void (*pfnHandler)(void)) {

    //Enable Timer1 peripheral
    SysCtlPeripheralEnable(DISPLAY_TIMER_PERIPH);

    //Wait for Timer1 module to be ready
    while(!SysCtlPeripheralReady(DISPLAY_TIMER_PERIPH)) {
    }

    //Configure Timer 1 as a periodic timer.
    TimerConfigure(DISPLAY_TIMER_BASE, TIMER_CFG_A_PERIODIC);
    //Disable Timer 1
    TimerIntDisable(DISPLAY_TIMER_BASE, TIMER_CLOCK_PIOSC);

    //To toggle a GPIO at 5hz and a 50% duty cycle by dividing by 2 of the desired period.
    //Calling SysCtlClockGet() to get number of clock cycles required for a 5hz period and then divide by 5hz
    //Load calculated period into the Timer's interval load register using TimerLoadSet function
    TimerLoadSet(DISPLAY_TIMER_BASE, DISPLAY_TIMER, (SysCtlClockGet ()/5 / 2) -1);
    //Set Prescaler
    TimerPrescaleSet(DISPLAY_TIMER_BASE, DISPLAY_TIMER, PRESCALER);

    // Enable the Timer1A interrupt on the processor (NVIC).
    IntEnable(INT_TIMER1A);
    //register interrupt handler (ISR)
    TimerIntRegister(DISPLAY_TIMER_BASE, DISPLAY_TIMER, pfnHandler);
    //Enable Timer Interrupt, Timer 1A Interrupt when Timeout
    TimerIntEnable(DISPLAY_TIMER_BASE, TIMER_TIMA_TIMEOUT);
    //master interrupt enable API for all interrupts
    IntMasterEnable();
    //Enable Timer 1A
    TimerEnable(DISPLAY_TIMER_BASE, DISPLAY_TIMER);

}

//Display Timer Interrupt Handler (ISR)
void DisplayTimerIntHandler(void){
    DisplayTimerIntClear();
    displayPedometer(stepCount,acc_vector, distancetravelled);
}

//Clear the timer interrupt flag. Required to launch next interrupt
void DisplayTimerIntClear(void) {
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}



