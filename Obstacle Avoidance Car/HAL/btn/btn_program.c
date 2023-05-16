/*
 * btn_program.c
 *
 *   Created on: Nov 20, 2022
 *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *  Description: This file contains all Button (BTN) functions' implementation.
 */

/* HAL */
#include "btn_config.h"
#include "btn_interface.h"

/*******************************************************************************************************************************************************************/
/*
 Name: BTN_getBTNState
 Input: u8 BTNId and Pointer to u8 ReturnedBTNState
 Output: u8 Error or No Error
 Description: Function to get BTN state.
*/
u8 BTN_getBTNState( u8 u8_a_BTNId, u8 *pu8_a_returnedBTNState )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: BTNId is in the valid range, and Pointer is not equal to NULL */
	if ( ( u8_a_BTNId <= BTN_U8_8 ) && ( pu8_a_returnedBTNState != NULL ) )
	{
		/* Check 1.1: Required BTNId */
		switch( u8_a_BTNId )
		{
			case BTN_U8_1: DIO_getPinValue( BTN_U8_PORT, BTN_U8_1_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_2: DIO_getPinValue( BTN_U8_PORT, BTN_U8_2_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_3: DIO_getPinValue( BTN_U8_PORT, BTN_U8_3_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_4: DIO_getPinValue( BTN_U8_PORT, BTN_U8_4_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_5: DIO_getPinValue( BTN_U8_PORT, BTN_U8_5_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_6: DIO_getPinValue( BTN_U8_PORT, BTN_U8_6_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_7: DIO_getPinValue( BTN_U8_PORT, BTN_U8_7_PIN, pu8_a_returnedBTNState ); break;
			case BTN_U8_8: DIO_getPinValue( BTN_U8_PORT, BTN_U8_8_PIN, pu8_a_returnedBTNState ); break;
		}		
	}
	/* Check 2: BTNId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong BTNId or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}
		
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/