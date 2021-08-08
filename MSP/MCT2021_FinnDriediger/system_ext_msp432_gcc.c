///////////////////////////////////////////////////////////////////////////
//
// system_ext_msp432.c
//
// A better system startup file for MSP432 with detailed clock selections
//
// Author: Andreas Terstegge April 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "msp.h"

////////////////////////////
// Hardware configuration //
////////////////////////////

// Defines the HFXT frequency in Hz (e.g. 48000000)
#define TARGET_HFXT_HZ 48000000

// Defines the LFXT frequency in Hz (e.g. 32768)
#define TARGET_LFXT_HZ 32768

///////////////////////////
// Configuration section //
///////////////////////////

// Controls the watchdog timer
#define TARGET_WDT_DISABLE 1

// Defines the DCO center frequency (3/6/12/24/48 MHz)
// DCO_1500kHz|DCO_3MHz|DCO_6MHz|DCO_12MHz|DCO_24MHz|DCO_48MHz
#define TARGET_DCO_RSEL DCO_3MHz

// Defines the DCO tuning value (-512...511)
// Use with caution! Too high values might brick the board...
#define TARGET_DCO_TUNE 0

// Selects the MCLK source: LFXT, VLO, REFO, DCO, MOD, HFXT
#define TARGET_MCLK_SELECT HFXT

// Selects the MCLK divider (1/2/4/8/16/32/64/128)
// DIV1, DIV2, DIV4, DIV8, DIV16, DIV32, DIV64, DIV128
#define TARGET_MCLK_DIV DIV1

// Selects the SMCLK source:  LFXT, VLO, REFO, DCO, MOD, HFXT
#define TARGET_SMCLK_SELECT HFXT

// Selects the SMCLK divider (1/2/4/8/16/32/64/128)
// DIV1, DIV2, DIV4, DIV8, DIV16, DIV32, DIV64, DIV128
#define TARGET_SMCLK_DIV DIV2

//////////////////////////////////////
// Defines for configuration values //
//////////////////////////////////////

#define DCO_1500kHz 0
#define DCO_3MHz    1
#define DCO_6MHz    2
#define DCO_12MHz   3
#define DCO_24MHz   4
#define DCO_48MHz   5

#define LFXT    0
#define VLO     1
#define REFO    2
#define DCO     3
#define MOD     4
#define HFXT    5

#define DIV1    0
#define DIV2    1
#define DIV4    2
#define DIV8    3
#define DIV16   4
#define DIV32   5
#define DIV64   6
#define DIV128  7

// Internal clock frequencies, used for SystemCoreClockUpdate()
#define __VLOCLK    9400
#define __MODCLK    25000000
#define __REFOCLK_L 32768
#define __REFOCLK_H 128000
#define __SYSCLK    5000000

// Sanity check for HFXT and LFXT
#if ((TARGET_MCLK_SELECT == HFXT) || (TARGET_SMCLK_SELECT == HFXT))
#ifndef TARGET_HFXT_HZ
#error No HFXT frequency specified (TARGET_HFXT_HZ)
#endif
#endif

#if ((TARGET_MCLK_SELECT == LFXT) || (TARGET_SMCLK_SELECT == LFXT))
#ifndef TARGET_LFXT_HZ
#error No LFXT frequency specified (TARGET_LFXT_HZ)
#endif
#endif

// Calculate HFXTFREQ bits (for CSCTL2)
#if (TARGET_HFXT_HZ > 40000000)
#define HFXT_FREQ CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE
#elif (TARGET_HFXT_HZ > 32000000)
#define HFXT_FREQ CS_CTL2_HFXTFREQ_5 | CS_CTL2_HFXTDRIVE
#elif (TARGET_HFXT_HZ > 2400000)
#define HFXT_FREQ CS_CTL2_HFXTFREQ_4 | CS_CTL2_HFXTDRIVE
#elif (TARGET_HFXT_HZ > 16000000)
#define HFXT_FREQ CS_CTL2_HFXTFREQ_3 | CS_CTL2_HFXTDRIVE
#elif (TARGET_HFXT_HZ > 8000000)
#define HFXT_FREQ CS_CTL2_HFXTFREQ_2 | CS_CTL2_HFXTDRIVE
#elif (TARGET_HFXT_HZ > 4000000)
#define HFXT_FREQ CS_CTL2_HFXTFREQ_1 | CS_CTL2_HFXTDRIVE
#else
#define HFXT_FREQ CS_CTL2_HFXTFREQ_0
#endif

#define MCLK_DIVIDER  (1 << TARGET_MCLK_DIV)
#define SMCLK_DIVIDER (1 << TARGET_SMCLK_DIV)

// Evaluate the MCLK setting
#if (TARGET_MCLK_SELECT == LFXT)
#define __MASTER_CLOCK (TARGET_LFXT_HZ / MCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == VLO)
#define __MASTER_CLOCK (__VLOCLK / MCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == REFO)
#define __MASTER_CLOCK (__REFOCLK_L / MCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == DCO)
#define __MASTER_CLOCK ( (1500000 << TARGET_DCO_RSEL) / MCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == MOD)
#define __MASTER_CLOCK (__MODCLK / MCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == HFXT)
#define __MASTER_CLOCK (TARGET_HFXT_HZ / MCLK_DIVIDER)
#else
#error No MCLK source defined (TARGET_MCLK_SELECT)
#endif

// Evaluate the SMCLK setting
#if (TARGET_MCLK_SELECT == LFXT)
#define __SUBSYS_CLOCK (TARGET_LFXT_HZ / SMCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == VLO)
#define __SUBSYS_CLOCK (__VLOCLK / SMCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == REFO)
#define __SUBSYS_CLOCK (__REFOCLK_L / SMCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == DCO)
#define __SUBSYS_CLOCK ((1500000 << TARGET_DCO_RSEL) / SMCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == MOD)
#define __SUBSYS_CLOCK (__MODCLK / SMCLK_DIVIDER)
#elif (TARGET_MCLK_SELECT == HFXT)
#define __SUBSYS_CLOCK (TARGET_HFXT_HZ / SMCLK_DIVIDER)
#else
#error No SMCLK source defined (TARGET_SMCLK_SELECT)
#endif

// Global clock variables
uint32_t SystemCoreClock      = __MASTER_CLOCK;  // the value of MCLK in Hz
uint32_t SubsystemMasterClock = __SUBSYS_CLOCK;  // the value of SMCLK in Hz

// Global xtal frequencies. If the xtal oscillators are enabled
// during run-time, the frequencies have to be set here so that
// SystemCoreClockUpdate can use them.
uint32_t HfxtFrequency = 0;
uint32_t LfxtFrequency = 0;


//
// Initialize the system
//
// @param  none
// @return none
//
// @brief  Setup the microcontroller system.
//
// Performs the following initialization steps:
//     1. Enables the FPU
//     2. Enables all SRAM banks
//     3. Sets up power regulator and VCORE
//     4. Enable Flash wait states if needed and read buffering
//     5. Enable HFXT and/or LFXT if needed
//     6. Configure the Clock System (CS)
//
void SystemInit(void)
{
    // Control the watchdog timer
#if (TARGET_WDT_DISABLE == 1)
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
#endif

    // Enable FPU (CP10 full access, CP11 full access)
    SCB->CPACR |= (SCB_CPACR_CP10_MASK | SCB_CPACR_CP11_MASK);
    // Enable all SRAM banks
    SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK7_EN;

#if (__MASTER_CLOCK >= 48000000)
    // Switches to DCDC VCORE1
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_DCDC_VCORE1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    // 1 flash wait states (BANK0 VCORE1 max is 16 MHz,
    // BANK1 VCORE1 max is 32 MHz)
    FLCTL->BANK0_RDCTL = FLCTL_BANK0_RDCTL_WAIT_1 | FLCTL_BANK0_RDCTL_BUFD
                                                  | FLCTL_BANK0_RDCTL_BUFI;
    FLCTL->BANK1_RDCTL = FLCTL_BANK1_RDCTL_WAIT_1 | FLCTL_BANK1_RDCTL_BUFD
                                                  | FLCTL_BANK1_RDCTL_BUFI;
#elif (__MASTER_CLOCK >= 24000000)
    // Switches to DCDC VCORE0
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_DCDC_VCORE0;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    // Enable read buffering and 1 flash wait state (BANK0 VCORE0 max is 12 MHz)
    FLCTL->BANK0_RDCTL = FLCTL_BANK0_RDCTL_WAIT_1 | FLCTL_BANK0_RDCTL_BUFD
                                                  | FLCTL_BANK0_RDCTL_BUFI;
#endif

    // Unlock CS module
    CS->KEY = CS_KEY_VAL;
#ifdef TARGET_HFXT_HZ
    HfxtFrequency = TARGET_HFXT_HZ;
    // Enable the HFXT crystal oscillator.
    // Initialize PJ for HFXT
    PJ->SEL0 |=  BIT3;
    PJ->SEL1 &= ~BIT3;
    CS->CTL2 |=  CS_CTL2_HFXT_EN | HFXT_FREQ;
    // Wait for the HFXT to stabilize. After a soft reset
    // this code might run with 48MHz, so we make sure the
    // clock is stable for some loop iterations...
    for (int count=0; count < 200; ++count) {
        if (CS->IFG & CS_IFG_HFXTIFG) {
            CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;
        }
    }
#endif

#ifdef TARGET_LFXT_HZ
    LfxtFrequency = TARGET_LFXT_HZ;
    // Enable the LFXT crystal oscillator. If the LFXT is not
    // available, the system will switch automatically to
    // REFOCLK with 32768Hz mode (less precision...).
    // Initialize PJ for LFXT
    PJ->SEL0 |=  BIT0;
    PJ->SEL1 &= ~BIT0;
    // Enable LFXT
    CS->CTL2 |= CS_CTL2_LFXT_EN;  // Enable LFXT
    // Wait for the LFXT to stabilize. After a soft reset
    // this code might run with 48MHz, so we make sure the
    // clock is stable for some loop iterations...
    for (int count=0; count < 200; ++count) {
        if (CS->IFG & CS_IFG_LFXTIFG) {
            CS->CLRIFG |= CS_CLRIFG_CLR_LFXTIFG;
        }
    }
#endif

    // Set CTL0 and CTL1
    CS->CTL0 =(TARGET_DCO_RSEL     << CS_CTL0_DCORSEL_OFS) |
              (TARGET_DCO_TUNE & 0x3ff);
    CS->CTL1 = TARGET_MCLK_SELECT  << CS_CTL1_SELM_OFS |
               TARGET_MCLK_DIV     << CS_CTL1_DIVM_OFS |
               TARGET_SMCLK_SELECT << CS_CTL1_SELS_OFS |
               TARGET_SMCLK_DIV    << CS_CTL1_DIVS_OFS;

    // Lock CS module
    CS->KEY = 0;

    // Update the global clock values.
    SystemCoreClockUpdate();
}


//
// Calculate the DCO clock using the DCO tune value
//
// @param  dco_base_clock: The DCO base clock without
//                         using the DCO tune value
// @return The real DCO clock
//
// @brief  Calcluates the DCO clock using the DCO tune value
//
uint32_t calculate_DCO_clock(uint32_t dco_base_clock)
{
    // Get DCO tune value
    int16_t __DCOTUNE = (CS->CTL0 & CS_CTL0_DCOTUNE_MASK)
                                 >> CS_CTL0_DCOTUNE_OFS;
    // Check if we have a zero tune value
    if (!__DCOTUNE) {
        return dco_base_clock;
    }
    // Convert 10 bits signed int to 16 bits signed int
    if (__DCOTUNE &  0x0200) {
        __DCOTUNE |= 0xFC00;
    }
    // Get calibration data
    float    __DCO_CONSTK;
    uint32_t __DCO_FCAL;
    if (CS->CTL0 & CS_CTL0_DCORES) {
        // external resistor
        if ((CS->CTL0 & CS_CTL0_DCORSEL_MASK) == CS_CTL0_DCORSEL_5) {
            // DCORSEL is 5
            __DCO_CONSTK = TLV->DCOER_CONSTK_RSEL5;
            __DCO_FCAL   = TLV->DCOER_FCAL_RSEL5;
        } else {
            // DCORSEL is 0..4
            __DCO_CONSTK = TLV->DCOER_CONSTK_RSEL04;
            __DCO_FCAL   = TLV->DCOER_FCAL_RSEL04;
        }
    } else {
        // internal resistor
        if ((CS->CTL0 & CS_CTL0_DCORSEL_MASK) == CS_CTL0_DCORSEL_5) {
            // DCORSEL is 5
            __DCO_CONSTK = TLV->DCOIR_CONSTK_RSEL5;
            __DCO_FCAL   = TLV->DCOIR_FCAL_RSEL5;
        } else {
            // DCORSEL is 0..4
            __DCO_CONSTK = TLV->DCOIR_CONSTK_RSEL04;
            __DCO_FCAL   = TLV->DCOIR_FCAL_RSEL04;
        }
    }
    // Calculate tuned frequency
    float denom = 1.0f / __DCO_CONSTK + 768 - (float)__DCO_FCAL;
    return (float)dco_base_clock / (1.0f - (float)__DCOTUNE / denom);
}


//
// Update SystemCoreClock and SubsystemMasterClock variables
//
// @param  none
// @return none
//
// @brief  Updates the SystemCoreClock and SubsystemMasterClock
//         with current core Clock retrieved from CS registers.
//
void SystemCoreClockUpdate(void)
{
    // Check which source is selected for MCLK
    switch (CS->CTL1 & CS_CTL1_SELM_MASK) {
        //////////////////////////
        case CS_CTL1_SELM__LFXTCLK: {
        //////////////////////////
            // Check if we still have a LFXT fault
            if (CS->IFG & CS_IFG_LFXTIFG) {
                // According to the TRM, a LFXT fault will
                // always switch to REFOCLK with 32768Hz
                SystemCoreClock = __REFOCLK_L;
            } else {
                SystemCoreClock = LfxtFrequency;
            }
            break;
        }
        /////////////////////////
        case CS_CTL1_SELM__VLOCLK: {
        /////////////////////////
            SystemCoreClock = __VLOCLK;
            break;
        }
        //////////////////////////
        case CS_CTL1_SELM__REFOCLK: {
        //////////////////////////
            if (CS->CLKEN & CS_CLKEN_REFOFSEL) {
                SystemCoreClock = __REFOCLK_H;
            } else {
                SystemCoreClock = __REFOCLK_L;
            }
            break;
        }
        /////////////////////////
        case CS_CTL1_SELM__DCOCLK: {
        ////////////////////////
            // Set the center frequency
            SystemCoreClock = 1500000 << ((CS->CTL0 & CS_CTL0_DCORSEL_MASK)
                                                   >> CS_CTL0_DCORSEL_OFS);
            SystemCoreClock = calculate_DCO_clock(SystemCoreClock);
            break;
        }
        /////////////////////////
        case CS_CTL1_SELM__MODOSC: {
        /////////////////////////
            SystemCoreClock = __MODCLK;
            break;
        }
        //////////////////////////
        case CS_CTL1_SELM__HFXTCLK: {
        //////////////////////////
            // Check if we still have a HFXT fault
            if (CS->IFG & CS_IFG_HFXTIFG) {
                // According to the TRM, a HFXT fault will
                // switch over to SYSOSC...
                SystemCoreClock = __SYSCLK;
            } else {
                SystemCoreClock = HfxtFrequency;
            }
            break;
        }
    }
    // Check which source is selected for SMCLK
    switch (CS->CTL1 & CS_CTL1_SELS_MASK) {
        //////////////////////////
        case CS_CTL1_SELS__LFXTCLK: {
        //////////////////////////
            // Check if we still have a LFXT fault
            if (CS->IFG & CS_IFG_LFXTIFG_OFS) {
                // According to the TRM, a LFXT fault will
                // always switch to REFOCLK with 32768Hz
                SubsystemMasterClock = __REFOCLK_L;
            } else {
                SubsystemMasterClock = LfxtFrequency;
            }
            break;
        }
        /////////////////////////
        case CS_CTL1_SELS__VLOCLK: {
        /////////////////////////
            SubsystemMasterClock = __VLOCLK;
            break;
        }
        //////////////////////////
        case CS_CTL1_SELS__REFOCLK: {
        //////////////////////////
            if (CS->CLKEN & CS_CLKEN_REFOFSEL) {
                SubsystemMasterClock = __REFOCLK_H;
            } else {
                SubsystemMasterClock = __REFOCLK_L;
            }
            break;
        }
        /////////////////////////
        case CS_CTL1_SELS__DCOCLK: {
        /////////////////////////
            // Set the center frequency
            SubsystemMasterClock = 1500000 << ((CS->CTL0 & CS_CTL0_DCORSEL_MASK)
                                                        >> CS_CTL0_DCORSEL_OFS);
            SubsystemMasterClock = calculate_DCO_clock(SubsystemMasterClock);
            break;
        }
        /////////////////////////
        case CS_CTL1_SELS__MODOSC: {
        /////////////////////////
            SubsystemMasterClock = __MODCLK;
            break;
        }
        //////////////////////////
        case CS_CTL1_SELS__HFXTCLK: {
        //////////////////////////
            // Check if we still have a HFXT fault
            if (CS->IFG & CS_IFG_HFXTIFG) {
                // According to the TRM, a HFXT fault will
                // switch over to SYSOSC...
                SubsystemMasterClock = __SYSCLK;
            } else {
                SubsystemMasterClock = HfxtFrequency;
            }
            break;
        }
    }
    // Get the MCLK and SMCLK dividers
    int32_t __DIVM = 1 << ((CS->CTL1 & CS_CTL1_DIVM_MASK) >> CS_CTL1_DIVM_OFS);
    int32_t __DIVS = 1 << ((CS->CTL1 & CS_CTL1_DIVS_MASK) >> CS_CTL1_DIVS_OFS);
    // Update SystemCoreClock (MCLK) with divider value
    SystemCoreClock /= __DIVM;
    // Update SubsystemMasterClock (SMCLK) with divider value
    SubsystemMasterClock /= __DIVS;
}
