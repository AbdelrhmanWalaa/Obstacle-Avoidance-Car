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
 Input: dio_Port_number_en en_a_PortNumber,dio_Pin_number_en en_a_PinNumber
 Output: void
 Description: Function to initialize BUTTON pin as INPUT.
*/
void BUTTON_init( dio_Port_number_en en_a_PortNumber,dio_Pin_number_en en_a_PinNumber )
{
	DIO_init (en_a_PortNumber,en_a_PinNumber,IN);
}
/*
 Name: BUTTON_read
 Input: dio_Port_number_en en_a_PortNumber,dio_Pin_number_en en_a_PinNumber,u8 *en_a_state
 Output: void
 Description: Function to get BTN state.
*/
void BUTTON_read( dio_Port_number_en en_a_PortNumber,dio_Pin_number_en en_a_PinNumber,u8 *en_a_state )
{
	DIO_read (en_a_PortNumber,en_a_PinNumber,en_a_state);
}

/*******************************************************************************************************************************************************************/