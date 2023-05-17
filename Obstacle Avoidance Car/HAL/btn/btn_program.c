/*
 * btn_program.c
 *
 *   Created on: MAY 16, 2023
 *       Author: Mohamed Abdelsalam - https://github.com/m3adel
 *  Description: This file contains all Button (BTN) functions' implementation.
 */

/* HAL */
#include "btn_interface.h"

/*******************************************************************************************************************************************************************/
/*
 Name: BUTTON_init
 Input: dio_Port_number_en PortNumber,dio_Pin_number_en PinNumber
 Output: void
 Description: Function to initialize BUTTON pin as INPUT.
*/
void BUTTON_init( dio_Port_number_en PortNumber,dio_Pin_number_en PinNumber )
{
	DIO_init (PortNumber,PinNumber,IN);
}
/*
 Name: BUTTON_read
 Input: dio_Port_number_en PortNumber,dio_Pin_number_en PinNumber,u8 *state
 Output: void
 Description: Function to get BTN state.
*/
void BUTTON_read( dio_Port_number_en PortNumber,dio_Pin_number_en PinNumber,u8 *state )
{
	DIO_read (PortNumber,PinNumber,state);
}

/*******************************************************************************************************************************************************************/