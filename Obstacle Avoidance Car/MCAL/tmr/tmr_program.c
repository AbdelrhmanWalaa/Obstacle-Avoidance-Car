/*
 * tmr_program.c
 *
 *     Created on: Jul 27, 2021
 *         Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *    Description: This file contains all Timers (TMR) functions' implementation, and ISR functions' prototypes and implementation.
 *  MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
 */

/* MCAL */
#include "tmr_private.h"
#include "tmr_config.h"
#include "tmr_interface.h"

/*******************************************************************************************************************************************************************/
/* Declaration and Initialization */

/* Global Arrays of 3 Pointers to Functions ( because we have 3 Timers ), these functions ( in APP Layer ) which those 3 Pointers will hold their addresses; are having void input arguments and void return type. */
static void ( *apf_gs_OVFInterruptsAction[3] ) ( void ) = { NULL, NULL, NULL };
static void ( *apf_gs_COMPInterruptsAction[3] ) ( void ) = { NULL, NULL, NULL };

/* Global Array of OVF Counters, in case the desired time to be counted; is more than Timer counting register ( TCNTn ), i.e.: to count the number of Overflows. */
static u16 au16_gs_OVFCounters[3] = { 0, 0, 0 };

/* Global Variables ( Flags ) to be altered when CTC Mode is selected in Timer 1, and depending on which Channel is selected. */
static u8 u8_gs_timer1COMPAFlag = TMR_U8_FLAG_DOWN, u8_gs_timer1COMPBFlag = TMR_U8_FLAG_DOWN;

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_TMR0Initialization
 Input: void
 Output: void
 Description: Function to Initialize TMR0 peripheral.
*/
vd TMR_TMR0Initialization  ( void )
{
	/* Step 1: Select Waveform Generation Mode */
	switch ( TMR_U8_TMR_0_MODE_SELECT )
	{
		/* Case 1: Waveform Generation Mode = Normal Mode */
		case TMR_U8_TMR_0_NORMAL_MODE			: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM01_BIT ); break;
		/* Case 2: Waveform Generation Mode = PWM, Phase Correct Mode */
		case TMR_U8_TMR_0_PWM_PHASE_CORRECT_MODE: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM01_BIT ); break;
		/* Case 3: Waveform Generation Mode = CTC Mode */
		case TMR_U8_TMR_0_CTC_MODE				: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM01_BIT ); break;
		/* Case 4: Waveform Generation Mode = Fast PWM Mode */
		case TMR_U8_TMR_0_FAST_PWM_MODE			: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_WGM01_BIT ); break;
	}

	/* Step 2: Select Compare Match Output Mode */
	switch ( TMR_U8_TMR_0_COMP_OUTPUT_MODE )
	{
		/* Case 1: Waveform Generation Mode = Normal Mode */
		case TMR_U8_TMR_0_DISCONNECT_OC0_PIN: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
		/* Case 2: Waveform Generation Mode = PWM, Phase Correct Mode */
		case TMR_U8_TMR_0_TOG_OC0_PIN		: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
		/* Case 3: Waveform Generation Mode = CTC Mode */
		case TMR_U8_TMR_0_CLR_OC0_PIN		: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
		/* Case 4: Set OC0 on compare match ( PWM -> Inverting Mode ) */
		case TMR_U8_TMR_0_SET_OC0_PIN		: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
	}

	/* Step 3: Select Interrupt Source */
	switch ( TMR_U8_TMR_0_INTERRUPT_SELECT )
	{
		/* Case 1: Interrupt Source = No Interrupt ( i.e.: Interrupts are disabled, as in PWM two modes ) */
		case TMR_U8_TMR_0_NO_INTERRUPT  : CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE0_BIT ); CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE0_BIT );	break;
		/* Case 2: Interrupt Source = Compare Interrupt */
		case TMR_U8_TMR_0_COMP_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE0_BIT ); break;
		/* Case 3: Interrupt Source = Overflow Interrupt */
		case TMR_U8_TMR_0_OVF_INTERRUPT : SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE0_BIT ); break;
	}

	/* Step 4: Set Compare Value and Preload Value */
	/* Compare Value */
	TMR_U8_OCR0_REG  = TMR_U8_TMR_0_COMPARE_VALUE;
	/* Preload value */
	TMR_U8_TCNT0_REG = TMR_U8_TMR_0_PRELOAD_VALUE;

	/* Step 5: Select Clock Source ( i.e.: Start Counting ) */
	switch ( TMR_U8_TMR_0_CLOCK_SELECT )
	{
		/* Case 1: Clock Source = No Clock Source (Timer/Counter0 stopped) */
		case TMR_U8_TMR_0_NO_CLOCK_SOURCE				 : CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
	    /* Case 2: Clock Source =  No Prescaler */
		case TMR_U8_TMR_0_NO_PRESCALER					 : SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
		/* Case 3: Clock Source = 8 Prescaler */
		case TMR_U8_TMR_0_8_PRESCALER					 : CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
		/* Case 4: Clock Source = 64 Prescaler */
		case TMR_U8_TMR_0_64_PRESCALER					 : SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
		/* Case 5: Clock Source = 256 Prescaler */
		case TMR_U8_TMR_0_256_PRESCALER					 : CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
		/* Case 6: Clock Source = 1024 Prescaler */
		case TMR_U8_TMR_0_1024_PRESCALER				 : SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
		/* Case 7: Clock Source = External Clock Source with Falling Edge, on Pin TO */
		case TMR_U8_TMR_0_EXTERNAL_CLOCK_SOURCE_FALL_EDGE: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
		/* Case 8: Clock Source = External Clock Source with Rising Edge, on Pin TO */
		case TMR_U8_TMR_0_EXTERNAL_CLOCK_SOURCE_RISE_EDGE: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
														   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_TMR1Initialization
 Input: void
 Output: void
 Description: Function to Initialize TMR1 peripheral.
*/
vd TMR_TMR1Initialization  ( void )
{
	/* Step 1: Select Waveform Generation Mode */
	switch ( TMR_U8_TMR_1_MODE_SELECT )
	{
		/* Case 1: Waveform Generation Mode = Normal Mode */
		case TMR_U8_TMR_1_NORMAL_MODE                      	  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
																CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
		   	   	   	   	   	   	   	   	   	   	   	   	     	break;
		/* Case 2: Waveform Generation Mode = PWM, Phase Correct, 8-bit Mode */
		case TMR_U8_TMR_1_PWM_PHASE_CORRECT_8_BIT_MODE     	  : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
																CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 3: Waveform Generation Mode = PWM, Phase Correct, 9-bit Mode */
		case TMR_U8_TMR_1_PWM_PHASE_CORRECT_9_BIT_MODE	   	  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
															 	CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 		break;
		/* Case 4: Waveform Generation Mode = PWM, Phase Correct, 10-bit Mode */
		case TMR_U8_TMR_1_PWM_PHASE_CORRECT_10_BIT_MODE	   	  : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
															 	CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 5: Waveform Generation Mode = CTC OCR1A Top */
		case TMR_U8_TMR_1_CTC_OCR1A_TOP					   	  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 		break;
		/* Case 6: Waveform Generation Mode = Fast PWM, 8-bit Mode */
		case TMR_U8_TMR_1_FAST_PWM_8_BIT_MODE			   	  : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 7: Waveform Generation Mode = Fast PWM, 9-bit Mode */
		case TMR_U8_TMR_1_FAST_PWM_9_BIT_MODE			   	  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 		SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 8: Waveform Generation Mode = Fast PWM, 10-bit Mode */
		case TMR_U8_TMR_1_FAST_PWM_10_BIT_MODE			   	  : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 9: Waveform Generation Mode = PWM, Phase and Frequency Correct ICR1 Top */
		case TMR_U8_TMR_1_PWM_PHASE_AND_FREQ_CORRECT_ICR1_TOP : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
															 	CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 10: Waveform Generation Mode = PWM, Phase and Frequency Correct OCR1A Top */
		case TMR_U8_TMR_1_PWM_PHASE_AND_FREQ_CORRECT_OCR1A_TOP: SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
															 	CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 11: Waveform Generation Mode = PWM, Phase Correct ICR1 Top */
		case TMR_U8_TMR_1_PWM_PHASE_CORRECT_ICR1_TOP		  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
															 	CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 12: Waveform Generation Mode = PWM, Phase Correct OCR1A Top */
		case TMR_U8_TMR_1_PWM_PHASE_CORRECT_OCR1A_TOP		  : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
															 	CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 13: Waveform Generation Mode = CTC ICR1 Top */
		case TMR_U8_TMR_1_CTC_ICR1_TOP					   	  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 14: Waveform Generation Mode = Fast PWM ICR1 Top */
		case TMR_U8_TMR_1_FAST_PWM_ICR1_TOP				   	  : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 		SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
		/* Case 15: Waveform Generation Mode = Fast PWM OCR1A Top */
		case TMR_U8_TMR_1_FAST_PWM_OCR1A_TOP				  : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM10_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_WGM11_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM12_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_WGM13_BIT );
	   	   	   	   	   	     	 	 	 	 	 	 	 	 	break;
	}

	/* Step 2: Select Compare Match Output Mode */
	/* Channel A */
	switch ( TMR_U8_TMR_1_COMP_OUTPUT_MODE_A )
	{
		/* Case 1: Normal port operation, OC1A disconnected */
		case TMR_U8_TMR_1_DISCONNECT_OC1A_PIN: CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
		/* Case 2: Toggle OC1A on compare match */
		case TMR_U8_TMR_1_TOG_OC1A_PIN		 : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
		/* Case 3: Clear OC1A on compare match ( PWM -> Non-Inverting Mode ) */
		case TMR_U8_TMR_1_CLR_OC1A_PIN		 : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
		/* Case 4: Set OC1A on compare match ( PWM -> Inverting Mode ) */
		case TMR_U8_TMR_1_SET_OC1A_PIN		 : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
	}

	/* Channel B */
	switch ( TMR_U8_TMR_1_COMP_OUTPUT_MODE_B )
	{
		/* Case 1: Normal port operation, OC1B disconnected */
		case TMR_U8_TMR_1_DISCONNECT_OC1B_PIN: CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
		/* Case 2: Toggle OC1B on compare match */
		case TMR_U8_TMR_1_TOG_OC1B_PIN		 : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
		/* Case 3: Clear OC1B on compare match ( PWM -> Non-Inverting Mode ) */
		case TMR_U8_TMR_1_CLR_OC1B_PIN		 : CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
		/* Case 4: Set OC1B on compare match ( PWM -> Inverting Mode ) */
		case TMR_U8_TMR_1_SET_OC1B_PIN		 : SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
	}

	/* Step 3: Select Interrupt Source */
	switch ( TMR_U8_TMR_1_INTERRUPT_SELECT )
	{
		/* Case 1: Interrupt Source = No Interrupt ( i.e.: Interrupts are disabled, as in PWM modes ) */
		case TMR_U8_TMR_1_NO_INTERRUPT    : CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TICIE1_BIT );	CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1A_BIT );
											CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1B_BIT );	CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE1_BIT  ); break;
		/* Case 2: Interrupt Source = Capture Event Interrupt */
		case TMR_U8_TMR_1_CAPT_INTERRUPT  : SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TICIE1_BIT ); break;
		/* Case 3: Interrupt Source = Compare A Interrupt */
		case TMR_U8_TMR_1_COMP_A_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1A_BIT ); break;
		/* Case 4: Interrupt Source = Compare B Interrupt */
		case TMR_U8_TMR_1_COMP_B_INTERRUPT:	SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1B_BIT ); break;
		/* Case 5: Interrupt Source = Overflow Interrupt */
		case TMR_U8_TMR_1_OVF_INTERRUPT	  :	SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE1_BIT  ); break;
	}

	/* Step 4: Set Compare Values in both Channels, Input Capture Value, and Preload Value  */
	/* Compare Value A */
	TMR_U16_OCR1A_REG = TMR_U16_TMR_1_COMPARE_VALUE_A;
	/* Compare Value B */
	TMR_U16_OCR1B_REG = TMR_U16_TMR_1_COMPARE_VALUE_B;
	/* Input Capture Value */
	TMR_U16_ICR1_REG  = TMR_U16_TMR_1_INPUT_CAPTURE_VALUE;
	/* Preload value */
	TMR_U16_TCNT1_REG = TMR_U16_TMR_1_PRELOAD_VALUE;

	/* Step 5: Select Clock Source ( i.e.: Start Counting ) */
	switch ( TMR_U8_TMR_1_CLOCK_SELECT )
	{
		/* Case 1: Clock Source = No Clock Source (Timer/Counter1 stopped) */
		case TMR_U8_TMR_1_NO_CLOCK_SOURCE				 : CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 2: Clock Source =  No Prescaler */
		case TMR_U8_TMR_1_NO_PRESCALER					 : SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 3: Clock Source = 8 Prescaler */
		case TMR_U8_TMR_1_8_PRESCALER					 : CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 4: Clock Source = 64 Prescaler */
		case TMR_U8_TMR_1_64_PRESCALER					 : SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 5: Clock Source = 256 Prescaler */
		case TMR_U8_TMR_1_256_PRESCALER					 : CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 6: Clock Source = 1024 Prescaler */
		case TMR_U8_TMR_1_1024_PRESCALER				 : SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 7: Clock Source = External Clock Source with Falling Edge, on Pin T1 */
		case TMR_U8_TMR_1_EXTERNAL_CLOCK_SOURCE_FALL_EDGE: CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
		/* Case 8: Clock Source = External Clock Source with Rising Edge, on Pin T1 */
		case TMR_U8_TMR_1_EXTERNAL_CLOCK_SOURCE_RISE_EDGE: SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
														   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_TMR2Initialization
 Input: void
 Output: void
 Description: Function to Initialize TMR2 peripheral.
*/
vd TMR_TMR2Initialization  ( void )
{
	/* Step 1: Select Waveform Generation Mode */
	switch ( TMR_U8_TMR_2_MODE_SELECT )
	{
		/* Case 1: Waveform Generation Mode = Normal Mode */
		case TMR_U8_TMR_2_NORMAL_MODE			: CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM21_BIT ); break;
		/* Case 2: Waveform Generation Mode = PWM, Phase Correct Mode */
		case TMR_U8_TMR_2_PWM_PHASE_CORRECT_MODE: SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM21_BIT ); break;
		/* Case 3: Waveform Generation Mode = CTC Mode */
		case TMR_U8_TMR_2_CTC_MODE			    : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM21_BIT ); break;
		/* Case 4: Waveform Generation Mode = Fast PWM Mode */
		case TMR_U8_TMR_2_FAST_PWM_MODE		    : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_WGM21_BIT ); break;
	}

	/* Step 2: Select Compare Match Output Mode */
	switch ( TMR_U8_TMR_2_COMP_OUTPUT_MODE )
	{
		/* Case 1: Normal port operation, OC2 disconnected */
		case TMR_U8_TMR_2_DISCONNECT_OC2_PIN: CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
		/* Case 2: Toggle OC2 on compare match */
		case TMR_U8_TMR_2_TOG_OC2_PIN		: SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
		/* Case 3: Clear OC2 on compare match ( PWM -> Non-Inverting Mode ) */
		case TMR_U8_TMR_2_CLR_OC2_PIN		: CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
		/* Case 4: Set OC2 on compare match ( PWM -> Inverting Mode ) */
		case TMR_U8_TMR_2_SET_OC2_PIN		: SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
	}

	/* Step 3: Select Interrupt Source */
	switch ( TMR_U8_TMR_2_INTERRUPT_SELECT )
	{
		/* Case 1: Interrupt Source = No Interrupt ( i.e.: Interrupts are disabled, as in PWM two modes ) */
		case TMR_U8_TMR_2_NO_INTERRUPT  : CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE2_BIT ); CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE2_BIT ); break;
		/* Case 2: Interrupt Source = Compare Interrupt */
		case TMR_U8_TMR_2_COMP_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE2_BIT ); break;
		/* Case 3: Interrupt Source = Overflow Interrupt */
		case TMR_U8_TMR_2_OVF_INTERRUPT : SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE2_BIT ); break;
	}
	
	/* Step 4: Set Compare Value and Preload Value */
	/* Compare Value */
	TMR_U8_OCR2_REG  = TMR_U8_TMR_2_COMPARE_VALUE;
	/* Set Preload value */
	TMR_U8_TCNT2_REG = TMR_U8_TMR_2_PRELOAD_VALUE;

	/* Step 5: Select Clock Source ( i.e.: Start Counting ) */
	switch ( TMR_U8_TMR_2_CLOCK_SELECT )
	{
		/* Case 1: Clock Source = No Clock Source (Timer/Counter2 stopped) */
		case TMR_U8_TMR_2_NO_CLOCK_SOURCE: CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 2: Clock Source =  No Prescaler */
		case TMR_U8_TMR_2_NO_PRESCALER	 : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 3: Clock Source = 8 Prescaler */
		case TMR_U8_TMR_2_8_PRESCALER	 : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 4: Clock = 32 Prescaler */
		case TMR_U8_TMR_2_32_PRESCALER	 : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
			 	 	 	 	 	 	 	   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 5: Clock Source = 64 Prescaler */
		case TMR_U8_TMR_2_64_PRESCALER	 : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 6: Clock Source = 128 Prescaler */
		case TMR_U8_TMR_2_128_PRESCALER	 : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 7: Clock Source = 256 Prescaler */
		case TMR_U8_TMR_2_256_PRESCALER	 : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
		/* Case 8: Clock Source = 1024 Prescaler */
		case TMR_U8_TMR_2_1024_PRESCALER : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
										   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_enableTMR
 Input: u8 TimerId
 Output: u8 Error or No Error
 Description: Function to Enable TMR peripheral.
*/
u8 TMR_enableTMR		   ( u8 u8_a_timerId )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range */
	if ( u8_a_timerId <= TMR_U8_TMR2 )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0:

				/* Step 1: Set Compare Value and Preload Value */
				/* Compare Value */
				TMR_U8_OCR0_REG  = TMR_U8_TMR_0_COMPARE_VALUE;
				/* Preload value */
				TMR_U8_TCNT0_REG = TMR_U8_TMR_0_PRELOAD_VALUE;

				/* Step 2: Select Clock Source ( i.e.: Start Counting ) */
				switch ( TMR_U8_TMR_0_CLOCK_SELECT )
				{
				    /* Case 1: Clock Source =  No Prescaler */
					case TMR_U8_TMR_0_NO_PRESCALER					 : SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
					/* Case 2: Clock Source = 8 Prescaler */
					case TMR_U8_TMR_0_8_PRESCALER					 : CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
					/* Case 3: Clock Source = 64 Prescaler */
					case TMR_U8_TMR_0_64_PRESCALER					 : SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
					/* Case 4: Clock Source = 256 Prescaler */
					case TMR_U8_TMR_0_256_PRESCALER					 : CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
					/* Case 5: Clock Source = 1024 Prescaler */
					case TMR_U8_TMR_0_1024_PRESCALER				 : SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
					/* Case 6: Clock Source = External Clock Source with Falling Edge, on Pin TO */
					case TMR_U8_TMR_0_EXTERNAL_CLOCK_SOURCE_FALL_EDGE: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
					/* Case 7: Clock Source = External Clock Source with Rising Edge, on Pin TO */
					case TMR_U8_TMR_0_EXTERNAL_CLOCK_SOURCE_RISE_EDGE: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );
																	   SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT ); break;
				}
			break;

			case TMR_U8_TMR1:

				/* Step 1: Set Compare Value in both Channels, Input Capture Value, and Preload Value  */
				/* Compare Value A */
				TMR_U16_OCR1A_REG = TMR_U16_TMR_1_COMPARE_VALUE_A;
				/* Compare Value B */
				TMR_U16_OCR1B_REG = TMR_U16_TMR_1_COMPARE_VALUE_B;
				/* Input Capture Value */
				TMR_U16_ICR1_REG  = TMR_U16_TMR_1_INPUT_CAPTURE_VALUE;
				/* Preload value */
				TMR_U16_TCNT1_REG = TMR_U16_TMR_1_PRELOAD_VALUE;

				/* Step 2: Select Clock Source ( i.e.: Start Counting ) */
				switch ( TMR_U8_TMR_1_CLOCK_SELECT )
				{
					/* Case 1: Clock Source =  No Prescaler */
					case TMR_U8_TMR_1_NO_PRESCALER					 : SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
					/* Case 2: Clock Source = 8 Prescaler */
					case TMR_U8_TMR_1_8_PRESCALER					 : CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
					/* Case 3: Clock Source = 64 Prescaler */
					case TMR_U8_TMR_1_64_PRESCALER					 : SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
					/* Case 4: Clock Source = 256 Prescaler */
					case TMR_U8_TMR_1_256_PRESCALER					 : CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
					/* Case 5: Clock Source = 1024 Prescaler */
					case TMR_U8_TMR_1_1024_PRESCALER				 : SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
					/* Case 6: Clock Source = External Clock Source with Falling Edge, on Pin T1 */
					case TMR_U8_TMR_1_EXTERNAL_CLOCK_SOURCE_FALL_EDGE: CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
					/* Case 7: Clock Source = External Clock Source with Rising Edge, on Pin T1 */
					case TMR_U8_TMR_1_EXTERNAL_CLOCK_SOURCE_RISE_EDGE: SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT );
																	   SET_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT ); break;
				}
			break;

			case TMR_U8_TMR2:

				/* Step 1: Set Compare Value and Preload Value */
				/* Compare Value */
				TMR_U8_OCR2_REG  = TMR_U8_TMR_2_COMPARE_VALUE;
				/* Set Preload value */
				TMR_U8_TCNT2_REG = TMR_U8_TMR_2_PRELOAD_VALUE;

				/* Step 2: Select Clock Source ( i.e.: Start Counting ) */
				switch ( TMR_U8_TMR_2_CLOCK_SELECT )
				{
					/* Case 1: Clock Source =  No Prescaler */
					case TMR_U8_TMR_2_NO_PRESCALER	 : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
													   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
					/* Case 2: Clock Source = 8 Prescaler */
					case TMR_U8_TMR_2_8_PRESCALER	 : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
													   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
					/* Case 3: Clock = 32 Prescaler */
					case TMR_U8_TMR_2_32_PRESCALER	 : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
						 	 	 	 	 	 	 	   CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
					/* Case 4: Clock Source = 64 Prescaler */
					case TMR_U8_TMR_2_64_PRESCALER	 : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
													   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
					/* Case 5: Clock Source = 128 Prescaler */
					case TMR_U8_TMR_2_128_PRESCALER	 : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
													   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
					/* Case 6: Clock Source = 256 Prescaler */
					case TMR_U8_TMR_2_256_PRESCALER	 : CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
													   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
					/* Case 7: Clock Source = 1024 Prescaler */
					case TMR_U8_TMR_2_1024_PRESCALER : SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );
													   SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT ); break;
				}
			break;
		}
	}
	/* Check 2: TimertId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_disableTMR
 Input: u8 TimerId
 Output: u8 Error or No Error
 Description: Function to Disable TMR peripheral.
*/
u8 TMR_disableTMR		   ( u8 u8_a_timerId )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range */
	if ( u8_a_timerId <= TMR_U8_TMR2 )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0:
				
				/* Step 1: Disconnect COMP ( OC0 ) Pin */
				CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT );
				
				/* Step 2: Clock Source = No Clock Source ( Timer/Counter0 stopped ) */
				CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS01_BIT );	CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_CS02_BIT );

				/* Step 3: Clear Timer/Counter0 OVF Flag */
				SET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV0_BIT );

				/* Step 4: Clear Compare Value and Preload Value */
				/* Compare Value */
				TMR_U8_OCR0_REG  = 0x00;
				/* Clear TMR0 */
				TMR_U8_TCNT0_REG = 0x00;

				/* Step 5: Reset the OVF Counter of Timer/Counter0 to 0 */
				au16_gs_OVFCounters[TMR_U8_TMR0] = 0;

			break;

			case TMR_U8_TMR1:
				
				/* Step 1: Disconnect COMP ( OC1A and OC1B ) Pins */
				CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT );
				CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT );

				/* Step 1: Clock Source = No Clock Source ( Timer/Counter1 stopped ) */
				CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS10_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS11_BIT ); CLR_BIT( TMR_U8_TCCR1B_REG, TMR_U8_CS12_BIT );

				/* Step 2: Clear Timer/Counter1 OVF Flag */
				SET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV1_BIT );

				/* Step 3: Clear Compare Values in both Channels, Input Capture Value, and Preload Value  */
				/* Input Capture Value */
				TMR_U16_ICR1_REG  = 0x0000;
				/* Compare Value A */
				TMR_U16_OCR1A_REG = 0x0000;
				/* Compare Value B */
				TMR_U16_OCR1B_REG = 0x0000;
				/* Clear TMR1 */
				TMR_U16_TCNT1_REG = 0x0000;

				/* Step 4: Reset the OVF Counter of Timer/Counter1 to 0 */
				au16_gs_OVFCounters[TMR_U8_TMR1] = 0;

			break;

			case TMR_U8_TMR2:
			
				/* Step 1: Disconnect COMP ( OC2 ) Pin */
				CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT );

				/* Step 2: Clock Source = No Clock Source ( Timer/Counter2 stopped ) */
				CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS21_BIT );	CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_CS22_BIT );

				/* Step 3: Clear Timer/Counter2 OVF Flag */
				SET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV2_BIT );

				/* Step 4: Clear Compare Value and Preload Value */
				/* Compare Value */
				TMR_U8_OCR2_REG  = 0x00;
				/* Clear TMR2 */
				TMR_U8_TCNT2_REG = 0x00;

				/* Step 5: Reset the OVF Counter of Timer/Counter2 to 0 */
				au16_gs_OVFCounters[TMR_U8_TMR2] = 0;

			break;
		}
	}
	/* Check 2: TimertId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
Name: TMR_delayMS
Input: u8 TimerId and u32 DelayMS
Output: u8 Error or No Error
Description: Function to use TMR peripheral as Delay in MS.
*/
u8 TMR_delayMS			   ( u8 u8_a_timerId, u32 u32_a_delayMS )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range */
	if ( u8_a_timerId <= TMR_U8_TMR2 )
	{
		u32 u32_l_numberOfOVFs = 1, u32_l_counter = 0;
		
		/* Step 1: Disable Timer */
		TMR_disableTMR( u8_a_timerId );
		
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0:
			{
				/* Step 2: Calculate TickTime = ( Prescaler / F_CPU ) * 1000 */
				/* Example: Prescaler = 1, F_CPU = 1MHz */
				/* Answer : TickTime = ( 1 / 1000000 ) = 0.000001 sec * 1000 = 0.001 msec */
				f32 f32_l_tickTime = ( 1.0 / F_CPU ) * 1000;

				/* Step 3: Calculate TimerMaxDelay = TickTime * ( 2 ^ TimerResolution ) */
				/* Example: TickTime = 0.001 msec, TimerResolution = 8 */
				/* Answer : TimerMaxDelay = 0.001 * 256 = 0.256 msec */
				f32 f32_l_timerMaxDelay = f32_l_tickTime * pow( 2, TMR_U8_TMR0_RESOLUTION );
				
				/* Check 1.1.1: TimerMaxDelay > Delay */
				if ( f32_l_timerMaxDelay > u32_a_delayMS )
				{
					/* Step 3.1: Calculate TimerPreloadValue = ( TimerMaxDelay - Delay ) / TickTime */
					/* Example: TimerMaxDelay = 0.256 msec => 256 usec, Delay = 0.200 msec => 200 usec, TickTime = 0.001 msec */
					/* Answer : TimerPreloadValue = ( 0.256 - 0.200 ) / 0.001 = 56 */
					TMR_U8_TCNT0_REG = ( u8 ) ( ( f32_l_timerMaxDelay - u32_a_delayMS ) / f32_l_tickTime );
				}
				/* Check 1.1.2: TimerMaxDelay <= Delay */
				else if ( f32_l_timerMaxDelay <= u32_a_delayMS )
				{
					/* Step 3.1: Calculate NumberOfOVFs and TimerPreloadValue */
					/*           1. Delay / TimerMaxDelay = Number.Fraction
					*			 2. NumberOfOVFs = Number + 1
					*			 3. TimerPreloadValue = ( 2 ^ TimerResolution ) - ( Fraction * ( 2 ^ TimerResolution ) )
					*/
					/* Example: TimerMaxDelay = 0.256 msec, Delay = 100 msec */
					/* Answer : 100 / 0.256 = 390.625
					*			NumberOfOVFs = 390 + 1 = 391
					*			TimerPreloadValue = ( 2 ^ 8 ) - ( 0.625 * ( 2 ^ 8 ) ) = 96
					*/
					f32 f32_l_result = ( f32 ) ( u32_a_delayMS / f32_l_timerMaxDelay );
					f32 f32_l_number;
					f32 f32_L_fraction = modff( f32_l_result, &f32_l_number );
					u32_l_numberOfOVFs = ( u32 ) ( f32_l_number + 1 );
					TMR_U8_TCNT0_REG = ( u8 ) ( pow( 2, TMR_U8_TMR0_RESOLUTION ) - ( f32_L_fraction * pow( 2, TMR_U8_TMR0_RESOLUTION ) ) );
				}
			}
			break;
			
			case TMR_U8_TMR1: /* Future Improvements ?? ??? ???? */ break;
			case TMR_U8_TMR2: /* Future Improvements ?? ??? ???? */ break;
		}

		/* Step 4: Enable Timer */
		TMR_enableTMR( u8_a_timerId );
		
		u8 Loc_u8OVFFlagStatus = TMR_U8_FLAG_DOWN;
		
		/* Loop: Until Counter = NumberOfOVFs */
		while ( u32_l_numberOfOVFs > u32_l_counter )
		{
			/* Loop: Until OVFFlagStatus is set ( i.e.: Timer overflow occurs ) */
			while ( Loc_u8OVFFlagStatus == TMR_U8_FLAG_DOWN )
			{
				/* Step 5: Get OVFFlagStatus */
				TMR_getOVFFlagStatus( u8_a_timerId, &Loc_u8OVFFlagStatus );
			}
			
			/* Step 6: Reset OVFFlagStatus value */
			Loc_u8OVFFlagStatus = TMR_U8_FLAG_DOWN;
			/* Step 7: Increment Counter */
			u32_l_counter++;
			/* Step 8: Clear OVFFlag ( i.e.: TOVn bit in TIFR register ) */
			TMR_clearOVFFlag( u8_a_timerId );
		}
		
		/* Step 9: Disable Timer */
		TMR_disableTMR( u8_a_timerId );
	}
	/* Check 2: TimertId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
Name: TMR_delayUS
Input: u8 TimerId and u32 DelayUS
Output: u8 Error or No Error
Description: Function to use TMR peripheral as Delay in US.
*/
u8 TMR_delayUS			   ( u8 u8_a_timerId, u32 u32_a_delayUS )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* To be Implemented */
	
	return u8_l_errorState;
}
/*******************************************************************************************************************************************************************/
/*
 Name: TMR_enableTMRInterrupt
 Input: u8 TimerId, u8 Timer1ChannelId, and u8 InterruptType
 Output: u8 Error or No Error
 Description: Function to Enable Interrupt of TMR peripheral.
*/
u8 TMR_enableTMRInterrupt  ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u8 u8_a_interruptType )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId, ChannelId and InterruptType are in the valid range */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) && ( u8_a_interruptType <= TMR_U8_COMP_INTERRUPT ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0:
				/* Check 1.1.1: Required InterruptType */
				switch ( u8_a_interruptType )
				{
					case TMR_U8_COMP_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE0_BIT ); break;
					case TMR_U8_OVF_INTERRUPT : SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE0_BIT ); break;
				}
			break;

			case TMR_U8_TMR1:
				/* Check 1.1.2: Required InterruptType */
				switch ( u8_a_interruptType )
				{
					case TMR_U8_CAPT_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TICIE1_BIT ); break;

					case TMR_U8_COMP_INTERRUPT:
						/* Check 1.1.2.1: Required Timer1ChannelId */
						switch ( u8_a_timer1ChannelId )
						{
							case TMR_U8_NO_CHANNEL     :				/* Do Nothing */  				 break;
							case TMR_U8_TMR_1_CHANNEL_A: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1A_BIT ); break;
							case TMR_U8_TMR_1_CHANNEL_B: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1B_BIT ); break;
						}
					break;

					case TMR_U8_OVF_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE1_BIT ); break;
				}
			break;

			case TMR_U8_TMR2:
				/* Check 1.1.3: Required InterruptType */
				switch ( u8_a_interruptType )
				{
					case TMR_U8_COMP_INTERRUPT: SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE2_BIT ); break;
					case TMR_U8_OVF_INTERRUPT : SET_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE2_BIT ); break;
				}
			break;
		}
	}
	/* Check 2: TimertId, ChannelId or InterruptType is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId, ChannelId or InterruptType! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_disableTMRInterrupt
 Input: u8 TimerId, u8 Timer1ChannelId, and u8 InterruptType
 Output: u8 Error or No Error
 Description: Function to Disable Interrupt of TMR peripheral.
*/
u8 TMR_disableTMRInterrupt ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u8 u8_a_interruptType )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId, ChannelId and InterruptType are in the valid range */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) && ( u8_a_interruptType <= TMR_U8_COMP_INTERRUPT ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0:
				/* Check 1.1.1: Required InterruptType */
				switch ( u8_a_interruptType )
				{
					case TMR_U8_COMP_INTERRUPT: CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE0_BIT ); break;
					case TMR_U8_OVF_INTERRUPT : CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE0_BIT ); break;
				}
			break;

			case TMR_U8_TMR1:
				/* Check 1.1.2: Required InterruptType */
				switch ( u8_a_interruptType )
				{
					case TMR_U8_CAPT_INTERRUPT: CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TICIE1_BIT ); break;

					case TMR_U8_COMP_INTERRUPT:
						/* Check 1.1.2.1: Required Timer1ChannelId */
						switch ( u8_a_timer1ChannelId )
						{
							case TMR_U8_NO_CHANNEL     :				/* Do Nothing */  				 break;
							case TMR_U8_TMR_1_CHANNEL_A: CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1A_BIT ); break;
							case TMR_U8_TMR_1_CHANNEL_B: CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE1B_BIT ); break;
						}
					break;

					case TMR_U8_OVF_INTERRUPT: CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE1_BIT ); break;
				}
				break;

			case TMR_U8_TMR2:
				/* Check 1.1.3: Required InterruptType */
				switch ( u8_a_interruptType )
				{
					case TMR_U8_COMP_INTERRUPT: CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_OCIE2_BIT ); break;
					case TMR_U8_OVF_INTERRUPT : CLR_BIT( TMR_U8_TIMSK_REG, TMR_U8_TOIE2_BIT ); break;
				}
			break;
		}
	}
	/* Check 2: TimertId, ChannelId or InterruptType is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId, ChannelId or InterruptType! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_enableCOMPPin
 Input: u8 TimerId, u8 Timer1ChannelId, and u8 COMPMode
 Output: u8 Error or No Error
 Description: Function to Enable COMP ( OCR ) Pin of TMR peripheral.
*/
u8 TMR_enableCOMPPin	   ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u8 u8_a_COMPMode )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId, ChannelId and COMPMode are in the valid range */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) && ( u8_a_COMPMode <= TMR_U8_SET_OCR_PIN ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0:
				/* Check 1.1.1: Required COMPMode */
				switch ( u8_a_COMPMode )
				{
					case TMR_U8_TOG_OCR_PIN: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
					case TMR_U8_CLR_OCR_PIN: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
					case TMR_U8_SET_OCR_PIN: SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); SET_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
				}
			break;

			case TMR_U8_TMR1:			
				/* Check 1.1.2: Required Timer1ChannelId */
				switch ( u8_a_timer1ChannelId )
				{
					case TMR_U8_NO_CHANNEL: /* Do Nothing */ break;
					
					case TMR_U8_TMR_1_CHANNEL_A:
						/* Check 1.1.2.1: Required COMPMode */
						switch ( u8_a_COMPMode )
						{
							case TMR_U8_TOG_OCR_PIN: SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
							case TMR_U8_CLR_OCR_PIN: CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
							case TMR_U8_SET_OCR_PIN: SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
						}
					break;
					
					case TMR_U8_TMR_1_CHANNEL_B:
						/* Check 1.1.2.2: Required COMPMode */
						switch ( u8_a_COMPMode )
						{
							case TMR_U8_TOG_OCR_PIN: SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
							case TMR_U8_CLR_OCR_PIN: CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
							case TMR_U8_SET_OCR_PIN: SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); SET_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
						}
					break;
				}
			break;

			case TMR_U8_TMR2:
				/* Check 1.1.3: Required COMPMode */
				switch ( u8_a_COMPMode )
				{
					case TMR_U8_TOG_OCR_PIN: SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
					case TMR_U8_CLR_OCR_PIN: CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
					case TMR_U8_SET_OCR_PIN: SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); SET_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
				}
			break;
		}
	}
	/* Check 2: TimertId, ChannelId or COMPMode is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId, ChannelId or COMPMode! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_disableCOMPPin
 Input: u8 TimerId and u8 Timer1ChannelId
 Output: u8 Error or No Error
 Description: Function to Disable COMP ( OCR ) Pin of TMR peripheral.
*/
u8 TMR_disableCOMPPin	   ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId )
{	 
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId and ChannelId are in the valid range */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM00_BIT ); CLR_BIT( TMR_U8_TCCR0_REG, TMR_U8_COM01_BIT ); break;
			
			case TMR_U8_TMR1:			
				/* Check 1.1.1: Required Timer1ChannelId */
				switch ( u8_a_timer1ChannelId )
				{
					case TMR_U8_NO_CHANNEL     :										/* Do Nothing */   										   break;
					case TMR_U8_TMR_1_CHANNEL_A: CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1A1_BIT ); break;
					case TMR_U8_TMR_1_CHANNEL_B: CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B0_BIT ); CLR_BIT( TMR_U8_TCCR1A_REG, TMR_U8_COM1B1_BIT ); break;
				}			
			break;
			
			case TMR_U8_TMR2: CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM20_BIT ); CLR_BIT( TMR_U8_TCCR2_REG, TMR_U8_COM21_BIT ); break;
		}
	}
	/* Check 2: TimertId or ChannelId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId or COMPMode! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_getNumberOfOVF
 Input: u8 TimerId and Pointer to u16 ReturnedNumberOfOVF
 Output: u8 Error or No Error
 Description: Function to Get number of OVFs updated by ISR in TMR peripheral.
*/
u8 TMR_getNumberOfOVF	   ( u8 u8_a_timerId, u16 *pu16_a_returnedNumberOfOVF )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range, and Pointer is not equal to NULL */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( pu16_a_returnedNumberOfOVF != NULL ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: *pu16_a_returnedNumberOfOVF = au16_gs_OVFCounters[TMR_U8_TMR0]; break;
			case TMR_U8_TMR1: *pu16_a_returnedNumberOfOVF = au16_gs_OVFCounters[TMR_U8_TMR1]; break;
			case TMR_U8_TMR2: *pu16_a_returnedNumberOfOVF = au16_gs_OVFCounters[TMR_U8_TMR2]; break;
		}
	}
	/* Check 2: TimertId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_getOVFFlagStatus
 Input: u8 TimerId and Pointer to u8 ReturnedFlagStatus
 Output: u8 Error or No Error
 Description: Function to Get status of the OVF Flag in TMR peripheral.
*/
u8 TMR_getOVFFlagStatus    ( u8 u8_a_timerId, u8 *pu8_a_returnedFlagStatus )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range, and Pointer is not equal to NULL */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( pu8_a_returnedFlagStatus != NULL ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: *pu8_a_returnedFlagStatus = GET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV0_BIT ); break;
			case TMR_U8_TMR1: *pu8_a_returnedFlagStatus = GET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV1_BIT ); break;
			case TMR_U8_TMR2: *pu8_a_returnedFlagStatus = GET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV2_BIT ); break;
		}
	}
	/* Check 2: TimertId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_clearOVFFlag
 Input: u8 TimerId
 Output: u8 Error or No Error
 Description: Function to Clear the OVF Flag in TMR peripheral.
*/
u8 TMR_clearOVFFlag	       ( u8 u8_a_timerId )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range */
	if ( u8_a_timerId <= TMR_U8_TMR2 )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: SET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV0_BIT ); break;
			case TMR_U8_TMR1: SET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV1_BIT ); break;
			case TMR_U8_TMR2: SET_BIT( TMR_U8_TIFR_REG, TMR_U8_TOV2_BIT ); break;
		}
	}
	/* Check 2: TimertId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_OVFSetCallBack
 Input: u8 TimerId and Pointer to function OVFInterruptAction taking void and returning void
 Output: u8 Error or No Error
 Description: Function to receive an address of a function ( in APP Layer ) to be called back in ISR function of the passed Timer ( TimerId ),
  	  	  	  the address is passed through a pointer to function ( OVFInterruptAction ), and then pass this address to ISR function.
*/
u8 TMR_OVFSetCallBack	   ( u8 u8_a_timerId, void ( *pf_a_OVFInterruptAction ) ( void ) )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range, and Pointer to Function is not equal to NULL */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( pf_a_OVFInterruptAction != NULL ) )
	{
		/* Store the passed address of function ( in APP Layer ) through pointer to function ( OVFInterruptAction ) into Global Array of Pointers to Functions ( OVFInterruptsAction ) in the passed index ( TimerId ). */
		apf_gs_OVFInterruptsAction[u8_a_timerId] = pf_a_OVFInterruptAction;
	}
	/* Check 2: TimertId is not in the valid range, or Pointer to Function is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId or Pointer to Function is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_COMPSetCallBack
 Input: u8 TimerId, u8 Timer1ChannelId, and Pointer to function COMPInterruptAction taking void and returning void
 Output: u8 Error or No Error
 Description: Function to receive an address of a function ( in APP Layer ) to be called back in ISR function of the passed Timer ( TimerId ),
 	 	 	  the address is passed through a pointer to function ( COMPInterruptAction ), and then pass this address to ISR function.
*/
u8 TMR_COMPSetCallBack	   ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, void ( *pf_a_COMPInterruptAction ) ( void ) )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId and ChannelId are in the valid range, and Pointer to Function is not equal to NULL */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) && ( pf_a_COMPInterruptAction != NULL ) )
	{
		/* Check 1.1: Required Timer1ChannelId */
		switch ( u8_a_timer1ChannelId )
		{
			case TMR_U8_NO_CHANNEL	   : 			/* Do Nothing */			 break;
			case TMR_U8_TMR_1_CHANNEL_A: u8_gs_timer1COMPAFlag = TMR_U8_FLAG_UP; break;
			case TMR_U8_TMR_1_CHANNEL_B: u8_gs_timer1COMPBFlag = TMR_U8_FLAG_UP; break;
		}

		/* Store the passed address of function ( in APP Layer ) through pointer to function ( COMPInterruptAction ) into Global Array of Pointers to Functions ( COMPInterruptsAction ) in the passed index ( TimerId ). */
		apf_gs_COMPInterruptsAction[u8_a_timerId] = pf_a_COMPInterruptAction;
	}
	/* Check 2: TimertId or ChannelId is not in the valid range, or Pointer to Function is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId, ChannelId, or Pointer to Function is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_getCounterValue
 Input: u8 TimerId and Pointer to u16 ReturnedCounterValue
 Output: u8 Error or No Error
 Description: Function to Get Counter Value from TCNTn Register.
*/
u8 TMR_getCounterValue     ( u8 u8_a_timerId, u16 *pu16_a_returnedCounterValue )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId is in the valid range, and Pointer is not equal to NULL */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( pu16_a_returnedCounterValue != NULL ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: *pu16_a_returnedCounterValue = ( u16 ) TMR_U8_TCNT0_REG; break;
			case TMR_U8_TMR1: *pu16_a_returnedCounterValue = TMR_U16_TCNT1_REG; 	   break;
			case TMR_U8_TMR2: *pu16_a_returnedCounterValue = ( u16 ) TMR_U8_TCNT2_REG; break;
		}
	}
	/* Check 2: TimertId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_setCompareMatchValue
 Input: u8 TimerId, u8 Timer1ChannelId, and u16 CompareMatchValue
 Output: u8 Error or No Error
 Description: Function to Set Compare Match Value in OCRn Register.
*/
u8 TMR_setCompareMatchValue( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u16 u16_a_compareMatchValue )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId and Timer1ChannelId are in the valid range */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: TMR_U8_OCR0_REG = ( u8 ) u16_a_compareMatchValue; break;

			case TMR_U8_TMR1:
				/* Check 1.1.1: Required Timer1ChannelId */
				switch ( u8_a_timer1ChannelId )
				{
					case TMR_U8_NO_CHANNEL	   : 			/* Do Nothing */ 			      break;
					case TMR_U8_TMR_1_CHANNEL_A: TMR_U16_OCR1A_REG = u16_a_compareMatchValue; break;
					case TMR_U8_TMR_1_CHANNEL_B: TMR_U16_OCR1B_REG = u16_a_compareMatchValue; break;
				}
			break;

			case TMR_U8_TMR2: TMR_U8_OCR2_REG = ( u8 ) u16_a_compareMatchValue; break;
		}
	}
	/* Check 2: TimertId or Timer1ChannelId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong TimertId or ChannelId! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_getCompareMatchValue
 Input: u8 TimerId, u8 Timer1ChannelId, and Pointer to u16 ReturnedCompareMatchValue
 Output: u8 Error or No Error
 Description: Function to Get Compare Match Value from OCRn Register.
*/
u8 TMR_getCompareMatchValue( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u16 *pu16_a_returnedCompareMatchValue )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: TimertId and Timer1ChannelId are in the valid range, and Pointer is not equal to NULL */
	if ( ( u8_a_timerId <= TMR_U8_TMR2 ) && ( u8_a_timer1ChannelId <= TMR_U8_TMR_1_CHANNEL_B ) && ( pu16_a_returnedCompareMatchValue != NULL ) )
	{
		/* Check 1.1: Required TimerId */
		switch ( u8_a_timerId )
		{
			case TMR_U8_TMR0: *pu16_a_returnedCompareMatchValue = ( u16 ) TMR_U8_OCR0_REG; break;

			case TMR_U8_TMR1:
				/* Check 1.1.1: Required Timer1ChannelId */
				switch ( u8_a_timer1ChannelId )
				{
					case TMR_U8_NO_CHANNEL	   : 					/* Do Nothing */  	      			break;
					case TMR_U8_TMR_1_CHANNEL_A: *pu16_a_returnedCompareMatchValue = TMR_U16_OCR1A_REG; break;
					case TMR_U8_TMR_1_CHANNEL_B: *pu16_a_returnedCompareMatchValue = TMR_U16_OCR1B_REG; break;
				}
			break;

			case TMR_U8_TMR2: *pu16_a_returnedCompareMatchValue = ( u16 ) TMR_U8_OCR2_REG; break;
		}
	}
	/* Check 2: TimertId or Timer1ChannelId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong TimertId or ChannelId, or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_setInputCaptureValue
 Input: u16 InputCaptureValue
 Output: void
 Description: Function to Set Input Capture Value in ICR1 Register ( in TMR1 only ).
*/
vd TMR_setInputCaptureValue( u16 u16_a_inputCaptureValue )
{
	TMR_U16_ICR1_REG = u16_a_inputCaptureValue;
}

/*******************************************************************************************************************************************************************/
/*
 Name: TMR_getInputCaptureValue
 Input: Pointer to u16 ReturnedInputCaptureValue
 Output: u8 Error or No Error
 Description: Function to Get Input Capture Value from ICR1 Register ( in TMR1 only ).
*/
u8 TMR_getInputCaptureValue( u16 *pu16_a_returnedInputCaptureValue )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: Pointer is not equal to NULL */
	if ( pu16_a_returnedInputCaptureValue != NULL )
	{
		*pu16_a_returnedInputCaptureValue = TMR_U16_ICR1_REG;
	}	
	/* Check 2: Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/

/*
 *  8-bit Timer/Counter2 ISR
 */
/* ISR functions' prototypes of TMR2 Compare Match ( COMP ), and TMR2 Overflow ( OVF ) respectively */
void __vector_4( void )		__attribute__((signal));
void __vector_5( void )		__attribute__((signal));

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR2 COMP */
void __vector_4( void )
{
	/* Static Counter, in case the desired time to be counted; is more than Timer2 counting register ( TCNT2 = 8 bits ), i.e.: to count the number of Overflows */
	static u16 u16_l_counterCOMP = 0;

	/* Increment the COMP Counter after every Overflow in Timer2 counting register */
	u16_l_counterCOMP++;

	/* Check 1: The COMP Counter reached the desired number of Overflows */
	if ( u16_l_counterCOMP >= TMR_U16_TMR_2_NUM_OF_OVERFLOWS )
	{
		/* Reset the COMP Counter to 0 */
		u16_l_counterCOMP = 0;

		/* Check 1.1: TMR2 index of the Global Array is not equal to NULL */
		if ( apf_gs_COMPInterruptsAction[TMR_U8_TMR2] != NULL )
		{
			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( COMPInterruptsAction ) */
			apf_gs_COMPInterruptsAction[TMR_U8_TMR2]();
		}
	}
}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR2 OVF */
void __vector_5( void )
{
	/* Increment the OVF Counter after every Overflow in Timer2 counting register */
	au16_gs_OVFCounters[TMR_U8_TMR2]++;

	/* Check 1: The OVF Counter reached the desired number of Overflows */
	if ( au16_gs_OVFCounters[TMR_U8_TMR2] >= TMR_U16_TMR_2_NUM_OF_OVERFLOWS )
	{
		/* Reset Preload Value */
		TMR_U8_TCNT2_REG = TMR_U8_TMR_2_PRELOAD_VALUE;

		/* Reset the OVF Counter to 0 */
		au16_gs_OVFCounters[TMR_U8_TMR2] = 0;

		/* Check 1.1: TMR2 index of the Global Array is not equal to NULL */
		if ( apf_gs_OVFInterruptsAction[TMR_U8_TMR2] != NULL )
		{
			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( OVFInterruptsAction ) */
			apf_gs_OVFInterruptsAction[TMR_U8_TMR2]();
		}
	}
}

/*******************************************************************************************************************************************************************/

/*
 * 16-bit Timer/Counter1 ISR
 */
/* ISR functions' prototypes of TMR1 Capture Event ( CAPT ), TMR1 Compare Match A ( COMPA ), TMR1 Compare Match B ( COMPB ), and TMR1 Overflow ( OVF ) respectively */
void __vector_6( void )		__attribute__((signal));
void __vector_7( void )		__attribute__((signal));
void __vector_8( void )		__attribute__((signal));
void __vector_9( void )		__attribute__((signal));

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR1 CAPT */
void __vector_6( void )
{

}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR1 COMPA */
void __vector_7( void )
{
	/* Static Counter, in case the desired time to be counted; is more than Timer1 counting register ( TCNT1 = 16 bits ), i.e.: to count the number of Overflows */
	static u16 u16_l_counterCOMP = 0;

	/* Increment the COMP Counter after every Overflow in Timer1 counting register */
	u16_l_counterCOMP++;

	/* Check 1: The COMP Counter reached the desired number of Overflows */
	if ( u16_l_counterCOMP >= TMR_U16_TMR_1_NUM_OF_OVERFLOWS )
	{
		/* Reset the COMP Counter to 0 */
		u16_l_counterCOMP = 0;

		/* Check 1.1: TMR1 index of the Global Array is not equal to NULL, and Channel A COMPFlag is raised */
		if ( ( apf_gs_COMPInterruptsAction[TMR_U8_TMR1] != NULL ) && ( u8_gs_timer1COMPAFlag != TMR_U8_FLAG_DOWN ) )
		{
			/* Reset Channel A COMPFlag to 0 */
			u8_gs_timer1COMPAFlag = TMR_U8_FLAG_DOWN;

			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( COMPInterruptsAction ) */
			apf_gs_COMPInterruptsAction[TMR_U8_TMR1]();
		}
	}
}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR1 COMPB */
void __vector_8( void )
{
	/* Static Counter, in case the desired time to be counted; is more than Timer1 counting register ( TCNT1 = 16 bits ), i.e.: to count the number of Overflows */
	static u16 u16_l_counterCOMP = 0;

	/* Increment the COMP Counter after every Overflow in Timer1 counting register */
	u16_l_counterCOMP++;

	/* Check 1: The COMP Counter reached the desired number of Overflows */
	if ( u16_l_counterCOMP >= TMR_U16_TMR_1_NUM_OF_OVERFLOWS )
	{
		/* Reset the COMP Counter to 0 */
		u16_l_counterCOMP = 0;

		/* Check 1.1: TMR1 index of the Global Array is not equal to NULL, and Channel B COMPFlag is raised */
		if ( ( apf_gs_COMPInterruptsAction[TMR_U8_TMR1] != NULL ) && ( u8_gs_timer1COMPBFlag != TMR_U8_FLAG_DOWN ) )
		{
			/* Reset Channel B COMPFlag to 0 */
			u8_gs_timer1COMPBFlag = TMR_U8_FLAG_DOWN;

			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( COMPInterruptsAction ) */
			apf_gs_COMPInterruptsAction[TMR_U8_TMR1]();
		}
	}
}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR1 OVF */
void __vector_9( void )
{
	/* Increment the OVF Counter after every Overflow in Timer1 counting register */
	au16_gs_OVFCounters[TMR_U8_TMR1]++;

	/* Check 1: The OVF Counter reached the desired number of Overflows */
	if ( au16_gs_OVFCounters[TMR_U8_TMR1] >= TMR_U16_TMR_1_NUM_OF_OVERFLOWS )
	{
		/* Reset Preload Value */
		TMR_U16_TCNT1_REG = TMR_U16_TMR_1_PRELOAD_VALUE;

		/* Reset the OVF Counter to 0 */
		au16_gs_OVFCounters[TMR_U8_TMR1] = 0;

		/* Check 1.1: TMR1 index of the Global Array is not equal to NULL */
		if ( apf_gs_OVFInterruptsAction[TMR_U8_TMR1] != NULL )
		{
			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( OVFInterruptsAction ) */
			apf_gs_OVFInterruptsAction[TMR_U8_TMR1]();
		}
	}
}

/*******************************************************************************************************************************************************************/

/*
 *  8-bit Timer/Counter0 ISR
 */
/* ISR functions' prototypes of TMR0 Compare Match ( COMP ), and TMR0 Overflow ( OVF ) respectively */
void __vector_10( void )	__attribute__((signal));
void __vector_11( void )	__attribute__((signal));

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR0 COMP */
void __vector_10( void )
{
	/* Static Counter, in case the desired time to be counted; is more than Timer0 counting register ( TCNT0 = 8 bits ), i.e.: to count the number of Overflows */
	static u16 u16_l_counterCOMP = 0;

	/* Increment the COMP Counter after every Overflow in Timer0 counting register */
	u16_l_counterCOMP++;

	/* Check 1: The COMP Counter reached the desired number of Overflows */
	if ( u16_l_counterCOMP >= TMR_U16_TMR_0_NUM_OF_OVERFLOWS )
	{
		/* Reset the COMP Counter to 0 */
		u16_l_counterCOMP = 0;

		/* Check 1.1: TMR0 index of the Global Array is not equal to NULL */
		if ( apf_gs_COMPInterruptsAction[TMR_U8_TMR0] != NULL )
		{
			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( COMPInterruptsAction ) */
			apf_gs_COMPInterruptsAction[TMR_U8_TMR0]();
		}
	}
}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of TMR0 OVF */
void __vector_11( void )
{
	/* Increment the OVF Counter after every Overflow in Timer0 counting register */
	au16_gs_OVFCounters[TMR_U8_TMR0]++;

	/* Check 1: The OVF Counter reached the desired number of Overflows */
	if ( au16_gs_OVFCounters[TMR_U8_TMR0] >= TMR_U16_TMR_0_NUM_OF_OVERFLOWS )
	{
		/* Reset Preload Value */
		TMR_U8_TCNT0_REG = TMR_U8_TMR_0_PRELOAD_VALUE;

		/* Reset the OVF Counter to 0 */
		au16_gs_OVFCounters[TMR_U8_TMR0] = 0;

		/* Check 1.1: TMR0 index of the Global Array is not equal to NULL */
		if ( apf_gs_OVFInterruptsAction[TMR_U8_TMR0] != NULL )
		{
			/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( OVFInterruptsAction ) */
			apf_gs_OVFInterruptsAction[TMR_U8_TMR0]();
		}
	}
}

/*******************************************************************************************************************************************************************/