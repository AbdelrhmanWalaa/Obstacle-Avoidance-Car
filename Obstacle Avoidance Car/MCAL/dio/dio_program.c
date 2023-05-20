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
 Input: dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,dio_Direction_en en_a_direction
 Output: void
 Description: Function to initialize Pin direction.
*/
void DIO_init (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,dio_Direction_en en_a_direction){
	switch(en_a_portNumber)
	{
		case A:
		if (en_a_direction == OUT)
		{
			SET_BIT( DIO_U8_DDRA_REG , en_a_pinNumber );    //Set Initial PortA Direction
		}
		else if (en_a_direction == IN)
		{
			CLR_BIT( DIO_U8_DDRA_REG ,en_a_pinNumber);   //clear Initial PortA Direction
		}
		break;
		case B:
		if (en_a_direction == OUT)
		{
			SET_BIT( DIO_U8_DDRB_REG , en_a_pinNumber );    //Set Initial PortB Direction
		}
		else if (en_a_direction == IN)
		{
			CLR_BIT( DIO_U8_DDRB_REG , en_a_pinNumber );   //clear Initial PortB Direction
		}
		break;
		case C:
		if (en_a_direction == OUT)
		{
			SET_BIT( DIO_U8_DDRC_REG , en_a_pinNumber );    //Set Initial PortC Direction
		}
		else if (en_a_direction == IN)
		{
			CLR_BIT( DIO_U8_DDRC_REG  , en_a_pinNumber );   //clear Initial PortC Direction
		}
		break;
		case D:
		if (en_a_direction == OUT)
		{
			SET_BIT( DIO_U8_DDRD_REG , en_a_pinNumber );    //Set Initial PortD Direction
		}
		else if (en_a_direction == IN)
		{
			CLR_BIT( DIO_U8_DDRD_REG , en_a_pinNumber );   //clear Initial PortD Direction
		}
		break;
	}
	
}
/*******************************************************************************************************************************************************************/
/*
 Name: DIO_write
 Input: dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,dio_Port_value_en en_a_value
 Output: void
 Description: Function to set Pin value.
*/

void DIO_write (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,dio_Port_value_en en_a_value){
	switch(en_a_portNumber)
	{
		case A:
		if (en_a_value == HIGH)
		{
			SET_BIT( DIO_U8_PORTA_REG , en_a_pinNumber );	//Set value of pinNumber in PortA
		}
		else if (en_a_value == LOW)
		{
			CLR_BIT( DIO_U8_PORTA_REG , en_a_pinNumber );	//clear value of pinNumber in PortA
		}
		break;
		case B:
		if (en_a_value == HIGH)
		{
			SET_BIT( DIO_U8_PORTB_REG , en_a_pinNumber );	//Set value of pinNumber in PortB
		}
		else if (en_a_value == LOW)
		{
			CLR_BIT( DIO_U8_PORTB_REG , en_a_pinNumber );	//clear value of pinNumber in PortB
		}
		break;
		case C:
		if (en_a_value == HIGH)
		{
			SET_BIT( DIO_U8_PORTC_REG , en_a_pinNumber );	//Set value of pinNumber in PortC
		}
		else if (en_a_value == LOW)
		{
			CLR_BIT( DIO_U8_PORTC_REG , en_a_pinNumber );	//clear value of pinNumber in PortC
		}
		break;
		case D:
		if (en_a_value == HIGH)
		{
			SET_BIT( DIO_U8_PORTD_REG , en_a_pinNumber );	//Set value of pinNumber in PortD
		}
		else if (en_a_value == LOW)
		{
			CLR_BIT( DIO_U8_PORTD_REG , en_a_pinNumber );	//clear value of pinNumber in PortD
		}
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_read
 Input: dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,u8 *en_a_data
 Output: void
 Description: Function to get Pin value.
*/

void DIO_read (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber,u8 *en_a_data){
	
	switch(en_a_portNumber)
	{
		case A:
		*en_a_data = GET_BIT( DIO_U8_PINA_REG , en_a_pinNumber );
		break;
		case B:
		*en_a_data = GET_BIT( DIO_U8_PINB_REG , en_a_pinNumber );
		break;
		case C:
		*en_a_data = GET_BIT( DIO_U8_PINC_REG , en_a_pinNumber );
		break;
		case D:
		*en_a_data = GET_BIT( DIO_U8_PIND_REG , en_a_pinNumber );
		break;
	}
}


/*******************************************************************************************************************************************************************/
/*
 Name: DIO_toggle
 Input: dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber
 Output: void
 Description: Function to toggle Pin value.
*/
void DIO_toggle (dio_Port_number_en en_a_portNumber,dio_Pin_number_en en_a_pinNumber){
	switch(en_a_portNumber)
	{
		case A:
		TOG_BIT( DIO_U8_PORTA_REG , en_a_pinNumber );
		break;
		case B:
		TOG_BIT( DIO_U8_PORTB_REG , en_a_pinNumber );
		break;
		case C:
		TOG_BIT( DIO_U8_PORTC_REG , en_a_pinNumber );
		break;
		case D:
		TOG_BIT( DIO_U8_PORTD_REG , en_a_pinNumber );
		break;
	}
}
/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPortDirection
 Input: dio_Port_number_en en_a_portNumber,u8 u8_a_PortDirection
 Output: void
 Description: Function to set Port direction.
*/

void DIO_setPortDirection( dio_Port_number_en en_a_portNumber,u8 u8_a_PortDirection )
{
		switch(en_a_portNumber)
		{
			case A:
			DIO_U8_DDRA_REG = u8_a_PortDirection;
			break;
			case B:
			DIO_U8_DDRB_REG = u8_a_PortDirection;
			break;
			case C:
			DIO_U8_DDRC_REG = u8_a_PortDirection;
			break;
			case D:
			DIO_U8_DDRD_REG = u8_a_PortDirection;
			break;
		}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPortValue
 Input: dio_Port_number_en en_a_portNumber,u8 u8_a_PortValue
 Output: void
 Description: Function to set Port value.
*/
void DIO_setPortValue ( dio_Port_number_en en_a_portNumber,u8 u8_a_PortValue )
{
	switch(en_a_portNumber)
	{
		case A:
		DIO_U8_PORTA_REG = u8_a_PortValue;
		break;
		case B:
		DIO_U8_PORTB_REG = u8_a_PortValue;
		break;
		case C:
		DIO_U8_PORTC_REG = u8_a_PortValue;
		break;
		case D:
		DIO_U8_PORTD_REG = u8_a_PortValue;
		break;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_getPortValue
 Input: dio_Port_number_en en_a_portNumber,u8 *u8_a_PortValue
 Output: void
 Description: Function to get Port value.
*/
void DIO_getPortValue ( dio_Port_number_en en_a_portNumber,u8 *u8_a_PortValue )
{
	switch(en_a_portNumber)
	{
		case A:
		*u8_a_PortValue = DIO_U8_PINA_REG;
		break;
		case B:
		*u8_a_PortValue = DIO_U8_PINB_REG;
		break;
		case C:
		*u8_a_PortValue = DIO_U8_PINC_REG;
		break;
		case D:
		*u8_a_PortValue = DIO_U8_PIND_REG;
		break;
	}
}
/*******************************************************************************************************************************************************************/
/*
 Name: DIO_higher_nipple
 Input: dio_Port_number_en en_a_portNumber,u8 u8_a_data 
 Output: void
 Description: Function to set Higher Nipple of Port value.
*/
void DIO_higher_nipple ( dio_Port_number_en en_a_portNumber,u8 u8_a_data )
{
	switch(en_a_portNumber)
	{
		case A:
		UPPER_NIPPLE( DIO_U8_PORTA_REG, u8_a_data );
		break;
		case B:
		UPPER_NIPPLE( DIO_U8_PORTB_REG, u8_a_data );
		break;
		case C:
		UPPER_NIPPLE( DIO_U8_PORTC_REG, u8_a_data );
		break;
		case D:
		UPPER_NIPPLE( DIO_U8_PORTD_REG, u8_a_data );
		break;
	}
}
/*******************************************************************************************************************************************************************/
/*
 Name: DIO_lower_nipple
 Input: dio_Port_number_en en_a_portNumber,u8 u8_a_data 
 Output: void
 Description: Function to set LOWER Nipple of Port value.
*/
void DIO_lower_nipple ( dio_Port_number_en en_a_portNumber,u8 u8_a_data )
{
	switch(en_a_portNumber)
	{
		case A:
		LOWER_NIPPLE( DIO_U8_PORTA_REG, u8_a_data );
		break;
		case B:
		LOWER_NIPPLE( DIO_U8_PORTB_REG, u8_a_data );
		break;
		case C:
		LOWER_NIPPLE( DIO_U8_PORTC_REG, u8_a_data );
		break;
		case D:
		LOWER_NIPPLE( DIO_U8_PORTD_REG, u8_a_data );
		break;
	}
}
/*******************************************************************************************************************************************************************/