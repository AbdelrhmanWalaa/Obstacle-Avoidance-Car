/*
 * dio_program.c
 *
 *     Created on: MAY 16, 2023
 *         Author: Mohamed Abdelsalam - https://github.com/m3adel
 *    Description: This file contains all Digital Input Output (DIO) functions' implementation.
 *  MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
 */

/* MCAL */
#include "dio_private.h"
#include "dio_config.h"
#include "dio_interface.h"

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_init
 Input: dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,dio_Direction_en direction
 Output: void
 Description: Function to initialize Pin direction.
*/
void DIO_init (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,dio_Direction_en direction){
	switch(portNumber)
	{
		case A:
		if (direction == OUT)
		{
			SET_BIT( DIO_U8_DDRA_REG , pinNumber );    //Set Initial PortA Direction
		}
		else if (direction == IN)
		{
			CLR_BIT( DIO_U8_DDRA_REG ,pinNumber);   //clear Initial PortA Direction
		}
		break;
		case B:
		if (direction == OUT)
		{
			SET_BIT( DIO_U8_DDRB_REG , pinNumber );    //Set Initial PortB Direction
		}
		else if (direction == IN)
		{
			CLR_BIT( DIO_U8_DDRB_REG , pinNumber );   //clear Initial PortB Direction
		}
		break;
		case C:
		if (direction == OUT)
		{
			SET_BIT( DIO_U8_DDRC_REG , pinNumber );    //Set Initial PortC Direction
		}
		else if (direction == IN)
		{
			CLR_BIT( DIO_U8_DDRC_REG  , pinNumber );   //clear Initial PortC Direction
		}
		break;
		case D:
		if (direction == OUT)
		{
			SET_BIT( DIO_U8_DDRD_REG , pinNumber );    //Set Initial PortD Direction
		}
		else if (direction == IN)
		{
			CLR_BIT( DIO_U8_DDRD_REG , pinNumber );   //clear Initial PortD Direction
		}
		break;
	}
	
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_write
 Input: dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,dio_Port_value_en value
 Output: void
 Description: Function to set Pin value.
*/

void DIO_write (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,dio_Port_value_en value){
	switch(portNumber)
	{
		case A:
		if (value == HIGH)
		{
			SET_BIT( DIO_U8_PORTA_REG , pinNumber );	//Set value of pinNumber in PortA
		}
		else if (value == LOW)
		{
			CLR_BIT( DIO_U8_PORTA_REG , pinNumber );	//clear value of pinNumber in PortA
		}
		break;
		case B:
		if (value == HIGH)
		{
			SET_BIT( DIO_U8_PORTB_REG , pinNumber );	//Set value of pinNumber in PortB
		}
		else if (value == LOW)
		{
			CLR_BIT( DIO_U8_PORTB_REG , pinNumber );	//clear value of pinNumber in PortB
		}
		break;
		case C:
		if (value == HIGH)
		{
			SET_BIT( DIO_U8_PORTC_REG , pinNumber );	//Set value of pinNumber in PortC
		}
		else if (value == LOW)
		{
			CLR_BIT( DIO_U8_PORTC_REG , pinNumber );	//clear value of pinNumber in PortC
		}
		break;
		case D:
		if (value == HIGH)
		{
			SET_BIT( DIO_U8_PORTD_REG , pinNumber );	//Set value of pinNumber in PortD
		}
		else if (value == LOW)
		{
			CLR_BIT( DIO_U8_PORTD_REG , pinNumber );	//clear value of pinNumber in PortD
		}
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_read
 Input: dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,u8 *data
 Output: void
 Description: Function to get Pin value.
*/

void DIO_read (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber,u8 *data){
	
	switch(portNumber)
	{
		case A:
		*data = GET_BIT( DIO_U8_PINA_REG , pinNumber );
		break;
		case B:
		*data = GET_BIT( DIO_U8_PINB_REG , pinNumber );
		break;
		case C:
		*data = GET_BIT( DIO_U8_PINC_REG , pinNumber );
		break;
		case D:
		*data = GET_BIT( DIO_U8_PIND_REG , pinNumber );
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_toggle
 Input: dio_Port_number_en portNumber,dio_Pin_number_en pinNumber
 Output: void
 Description: Function to toggle Pin value.
*/
void DIO_toggle (dio_Port_number_en portNumber,dio_Pin_number_en pinNumber){
	switch(portNumber)
	{
		case A:
		TOG_BIT( DIO_U8_PORTA_REG , pinNumber );
		break;
		case B:
		TOG_BIT( DIO_U8_PORTB_REG , pinNumber );
		break;
		case C:
		TOG_BIT( DIO_U8_PORTC_REG , pinNumber );
		break;
		case D:
		TOG_BIT( DIO_U8_PORTD_REG , pinNumber );
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPortDirection
 Input: dio_Port_number_en portNumber,u8 Port_direction
 Output: void
 Description: Function to set Port direction.
*/
void DIO_setPortDirection( dio_Port_number_en portNumber,u8 Port_direction )
{
		switch(portNumber)
		{
			case A:
			DIO_U8_DDRA_REG = Port_direction;
			break;
			case B:
			DIO_U8_DDRB_REG = Port_direction;
			break;
			case C:
			DIO_U8_DDRC_REG = Port_direction;
			break;
			case D:
			DIO_U8_DDRD_REG = Port_direction;
			break;
		}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPortValue
 Input: dio_Port_number_en portNumber,u8 Port_value
 Output: void
 Description: Function to set Port value.
*/
void DIO_setPortValue ( dio_Port_number_en portNumber,u8 Port_value )
{
	switch(portNumber)
	{
		case A:
		DIO_U8_PORTA_REG = Port_value;
		break;
		case B:
		DIO_U8_PORTB_REG = Port_value;
		break;
		case C:
		DIO_U8_PORTC_REG = Port_value;
		break;
		case D:
		DIO_U8_PORTD_REG = Port_value;
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_getPortValue
 Input: dio_Port_number_en portNumber,u8 *Port_value
 Output: void
 Description: Function to get Port value.
*/
void DIO_getPortValue	   ( dio_Port_number_en portNumber,u8 *Port_value )
{
	switch(portNumber)
	{
		case A:
		*Port_value = DIO_U8_PINA_REG;
		break;
		case B:
		*Port_value = DIO_U8_PINB_REG;
		break;
		case C:
		*Port_value = DIO_U8_PINC_REG;
		break;
		case D:
		*Port_value = DIO_U8_PIND_REG;
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_higher_nipple
 Input: dio_Port_number_en portNumber,u8 data 
 Output: void
 Description: Function to set Higher Nipple of Port value.
*/
// void DIO_higher_nipple ( dio_Port_number_en portNumber,u8 data )
// {
// 	switch(portNumber)
// 	{
// 		case A:
// 		UPPER_NIPPLE( DIO_U8_PORTA_REG, data );
// 		break;
// 		case B:
// 		UPPER_NIPPLE( DIO_U8_PORTB_REG, data );
// 		break;
// 		case C:
// 		UPPER_NIPPLE( DIO_U8_PORTC_REG, data );
// 		break;
// 		case D:
// 		UPPER_NIPPLE( DIO_U8_PORTD_REG, data );
// 		break;
// 	}
// }

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_lower_nipple
 Input: dio_Port_number_en portNumber,u8 data 
 Output: void
 Description: Function to set LOWER Nipple of Port value.
*/
// void DIO_lower_nipple ( dio_Port_number_en portNumber,u8 data )
// {
// 	switch(portNumber)
// 	{
// 		case A:
// 		LOWER_NIPPLE( DIO_U8_PORTA_REG, data );
// 		break;
// 		case B:
// 		LOWER_NIPPLE( DIO_U8_PORTB_REG, data );
// 		break;
// 		case C:
// 		LOWER_NIPPLE( DIO_U8_PORTC_REG, data );
// 		break;
// 		case D:
// 		LOWER_NIPPLE( DIO_U8_PORTD_REG, data );
// 		break;
// 	}
// }

/*******************************************************************************************************************************************************************/