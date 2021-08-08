/*
 * display.cpp
 *
 *  Created on: 28.05.2021
 *      Author: F.Driediger
 */

#include "display.h"
#include "gpio_msp432.h"
#include "msp.h"


display::display()
{
    //setup panel backlight
    gpio_msp432_pin background_light { PORT_PIN(2, 6) };
    background_light.setMode(GPIO::OUTPUT);
    background_light.setSEL(1);

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP;
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;

    //use pwm frequency of 300Hz
    TIMER_A0->CCR[0] = 10000;

    //set panel backlight to 100%
    TIMER_A0->CCR[3] = 10000;
}

display::~display() {}

void display::draw(uGUI gui)
{
    gui.PutString(5, 5, "MENU", false);

    gui.DrawLine(5, 15, 5, 24, C_WHITE);    // |
    gui.DrawLine(5, 25, 15, 25, C_WHITE);   // ---
    gui.PutString(20, 20, "MODE:", true);
    gui.DrawLine(20, 30, 20, 39, C_WHITE);
    gui.DrawLine(20, 40, 30, 40, C_WHITE);
    //the current mode is being drawn within the led function calls

    gui.DrawLine(5, 45, 5, 54, C_WHITE);
    gui.DrawLine(5, 55, 15, 55, C_WHITE);
    gui.PutString(20, 50, "COLOR:", true);
    gui.DrawLine(20, 60, 20, 69, C_WHITE);
    gui.DrawLine(20, 70, 30, 70, C_WHITE);
    gui.PutString(35, 75, "               ", true); // 2nd row for gradient color, to prevent leftover letters not being cleaned

    //led colors analog
}




