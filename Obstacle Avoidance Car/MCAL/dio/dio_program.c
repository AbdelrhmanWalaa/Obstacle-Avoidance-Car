/*
 * dio_program.c
 *
 *     Created on: Mar 31, 2021
 *         Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *    Description: This file contains all Digital Input Output (DIO) functions' implementation.
 *  MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
 */

/* MCAL */
#include "dio_private.h"
#include "dio_config.h"
#include "dio_interface.h"

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_initialization
 Input: void
 Output: void
 Description: Function to initialize DIO peripheral.
*/
vd DIO_initialization  ( void )
{
	/* Set Initial Port Direction */
	/* PORTA */
	DIO_U8_DDRA_REG = DIO_U8_CONC( DIO_U8_PA7_INITIAL_DIRECTION, DIO_U8_PA6_INITIAL_DIRECTION, DIO_U8_PA5_INITIAL_DIRECTION, DIO_U8_PA4_INITIAL_DIRECTION, DIO_U8_PA3_INITIAL_DIRECTION, DIO_U8_PA2_INITIAL_DIRECTION, DIO_U8_PA1_INITIAL_DIRECTION, DIO_U8_PA0_INITIAL_DIRECTION );
	/* PORTB */
	DIO_U8_DDRB_REG = DIO_U8_CONC( DIO_U8_PB7_INITIAL_DIRECTION, DIO_U8_PB6_INITIAL_DIRECTION, DIO_U8_PB5_INITIAL_DIRECTION, DIO_U8_PB4_INITIAL_DIRECTION, DIO_U8_PB3_INITIAL_DIRECTION, DIO_U8_PB2_INITIAL_DIRECTION, DIO_U8_PB1_INITIAL_DIRECTION, DIO_U8_PB0_INITIAL_DIRECTION );
	/* PORTC */
	DIO_U8_DDRC_REG = DIO_U8_CONC( DIO_U8_PC7_INITIAL_DIRECTION, DIO_U8_PC6_INITIAL_DIRECTION, DIO_U8_PC5_INITIAL_DIRECTION, DIO_U8_PC4_INITIAL_DIRECTION, DIO_U8_PC3_INITIAL_DIRECTION, DIO_U8_PC2_INITIAL_DIRECTION, DIO_U8_PC1_INITIAL_DIRECTION, DIO_U8_PC0_INITIAL_DIRECTION );
	/* PORTD */
	DIO_U8_DDRD_REG = DIO_U8_CONC( DIO_U8_PD7_INITIAL_DIRECTION, DIO_U8_PD6_INITIAL_DIRECTION, DIO_U8_PD5_INITIAL_DIRECTION, DIO_U8_PD4_INITIAL_DIRECTION, DIO_U8_PD3_INITIAL_DIRECTION, DIO_U8_PD2_INITIAL_DIRECTION, DIO_U8_PD1_INITIAL_DIRECTION, DIO_U8_PD0_INITIAL_DIRECTION );
	
	/* Set Initial Port Value */
	/* PORTA */
	DIO_U8_PORTA_REG = DIO_U8_CONC( DIO_U8_PA7_INITIAL_VALUE, DIO_U8_PA6_INITIAL_VALUE, DIO_U8_PA5_INITIAL_VALUE, DIO_U8_PA4_INITIAL_VALUE, DIO_U8_PA3_INITIAL_VALUE, DIO_U8_PA2_INITIAL_VALUE, DIO_U8_PA1_INITIAL_VALUE, DIO_U8_PA0_INITIAL_VALUE );
	/* PORTB */
	DIO_U8_PORTB_REG = DIO_U8_CONC( DIO_U8_PB7_INITIAL_VALUE, DIO_U8_PB6_INITIAL_VALUE, DIO_U8_PB5_INITIAL_VALUE, DIO_U8_PB4_INITIAL_VALUE, DIO_U8_PB3_INITIAL_VALUE, DIO_U8_PB2_INITIAL_VALUE, DIO_U8_PB1_INITIAL_VALUE, DIO_U8_PB0_INITIAL_VALUE );
	/* PORTC */
	DIO_U8_PORTC_REG = DIO_U8_CONC( DIO_U8_PC7_INITIAL_VALUE, DIO_U8_PC6_INITIAL_VALUE, DIO_U8_PC5_INITIAL_VALUE, DIO_U8_PC4_INITIAL_VALUE, DIO_U8_PC3_INITIAL_VALUE, DIO_U8_PC2_INITIAL_VALUE, DIO_U8_PC1_INITIAL_VALUE, DIO_U8_PC0_INITIAL_VALUE );
	/* PORTD */
	DIO_U8_PORTD_REG = DIO_U8_CONC( DIO_U8_PD7_INITIAL_VALUE, DIO_U8_PD6_INITIAL_VALUE, DIO_U8_PD5_INITIAL_VALUE, DIO_U8_PD4_INITIAL_VALUE, DIO_U8_PD3_INITIAL_VALUE, DIO_U8_PD2_INITIAL_VALUE, DIO_U8_PD1_INITIAL_VALUE, DIO_U8_PD0_INITIAL_VALUE );
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPinDirection
 Input: u8 PortId, u8 PinId, and u8 PinDirection
 Output: u8 Error or No Error
 Description: Function to set Pin direction.
*/
u8 DIO_setPinDirection ( u8 u8_a_portId, u8 u8_a_pinId, u8 u8_a_pinDirection )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range */
	if ( ( u8_a_portId <= DIO_U8_PORTD ) && ( u8_a_pinId <= DIO_U8_PIN7 ) )
	{
		/* Check 1.1: PinDirection is in the valid range */
		if ( u8_a_pinDirection == DIO_U8_PIN_INPUT )
		{
			/* Check 1.1.1: Required PortId */
			switch ( u8_a_portId )
			{
				case DIO_U8_PORTA: CLR_BIT( DIO_U8_DDRA_REG, u8_a_pinId ); break;
				case DIO_U8_PORTB: CLR_BIT( DIO_U8_DDRB_REG, u8_a_pinId ); break;
				case DIO_U8_PORTC: CLR_BIT( DIO_U8_DDRC_REG, u8_a_pinId ); break;
				case DIO_U8_PORTD: CLR_BIT( DIO_U8_DDRD_REG, u8_a_pinId ); break;
			}
		}
		/* Check 1.2: PinDirection is in the valid range */
		else if ( u8_a_pinDirection == DIO_U8_PIN_OUTPUT )
		{
			/* Check 1.2.1: Required PortId */
			switch ( u8_a_portId )
			{
				case DIO_U8_PORTA: SET_BIT( DIO_U8_DDRA_REG, u8_a_pinId ); break;
				case DIO_U8_PORTB: SET_BIT( DIO_U8_DDRB_REG, u8_a_pinId ); break;
				case DIO_U8_PORTC: SET_BIT( DIO_U8_DDRC_REG, u8_a_pinId ); break;
				case DIO_U8_PORTD: SET_BIT( DIO_U8_DDRD_REG, u8_a_pinId ); break;
			}
		}
		/* Check 1.3: PinDirection is not in the valid range */
		else
		{
			/* Update error state = NOK, wrong PinDirection! */
			u8_l_errorState = STD_TYPES_NOK;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId or PinId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPinValue
 Input: u8 PortId, u8 PinId, and u8 PinValue
 Output: u8 Error or No Error
 Description: Function to set Pin value.
*/
u8 DIO_setPinValue     ( u8 u8_a_portId, u8 u8_a_pinId, u8 Cpy_u8PinValue )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range */
	if ( ( u8_a_portId <= DIO_U8_PORTD ) && ( u8_a_pinId <= DIO_U8_PIN7 ) )
	{
		/* Check 1.1: PinValue is in the valid range */
		if ( Cpy_u8PinValue == DIO_U8_PIN_LOW )
		{
			/* Check 1.1.1: Required PortId */
			switch ( u8_a_portId )
			{
				case DIO_U8_PORTA: CLR_BIT( DIO_U8_PORTA_REG, u8_a_pinId ); break;
				case DIO_U8_PORTB: CLR_BIT( DIO_U8_PORTB_REG, u8_a_pinId ); break;
				case DIO_U8_PORTC: CLR_BIT( DIO_U8_PORTC_REG, u8_a_pinId ); break;
				case DIO_U8_PORTD: CLR_BIT( DIO_U8_PORTD_REG, u8_a_pinId ); break;
			}
		}
		/* Check 1.2: PinValue is in the valid range */
		else if ( Cpy_u8PinValue == DIO_U8_PIN_HIGH )
		{
			/* Check 1.2.1: Required PortId */
			switch( u8_a_portId )
			{
				case DIO_U8_PORTA: SET_BIT( DIO_U8_PORTA_REG, u8_a_pinId ); break;
				case DIO_U8_PORTB: SET_BIT( DIO_U8_PORTB_REG, u8_a_pinId ); break;
				case DIO_U8_PORTC: SET_BIT( DIO_U8_PORTC_REG, u8_a_pinId ); break;
				case DIO_U8_PORTD: SET_BIT( DIO_U8_PORTD_REG, u8_a_pinId ); break;
			}
		}
		/* Check 1.3: PinValue is not in the valid range */
		else
		{
			/* Update error state = NOK, wrong PinValue! */
			u8_l_errorState = STD_TYPES_NOK;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId or PinId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_getPinValue
 Input: u8 PortId, u8 PinId, and Pointer to u8 ReturnedPinValue
 Output: u8 Error or No Error
 Description: Function to get Pin value.
*/
u8 DIO_getPinValue 	   ( u8 u8_a_portId, u8 u8_a_pinId, u8 *pu8_a_returnedPinValue )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range, and Pointer is not equal to NULL */
	if ( ( u8_a_portId <= DIO_U8_PORTD ) && ( u8_a_pinId <= DIO_U8_PIN7 ) && ( pu8_a_returnedPinValue != NULL ) )
	{	
		/* Check 1.1: Required PortId */
		switch ( u8_a_portId )
		{
			case DIO_U8_PORTA: *pu8_a_returnedPinValue = GET_BIT( DIO_U8_PINA_REG, u8_a_pinId ); break;
			case DIO_U8_PORTB: *pu8_a_returnedPinValue = GET_BIT( DIO_U8_PINB_REG, u8_a_pinId ); break;
			case DIO_U8_PORTC: *pu8_a_returnedPinValue = GET_BIT( DIO_U8_PINC_REG, u8_a_pinId ); break;
			case DIO_U8_PORTD: *pu8_a_returnedPinValue = GET_BIT( DIO_U8_PIND_REG, u8_a_pinId ); break;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong PortId, PinId, or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_togglePinValue
 Input: u8 PortId and u8 PinId
 Output: u8 Error or No Error
 Description: Function to toggle Pin value.
*/
u8 DIO_togglePinValue  ( u8 u8_a_portId, u8 u8_a_pinId )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId and PinId are in the valid range */
	if ( ( u8_a_portId <= DIO_U8_PORTD ) && ( u8_a_pinId <= DIO_U8_PIN7 ) )
	{
		/* Check 1.1: Required PortId */
		switch ( u8_a_portId )
		{
			case DIO_U8_PORTA: TOG_BIT( DIO_U8_PORTA_REG, u8_a_pinId ); break;
			case DIO_U8_PORTB: TOG_BIT( DIO_U8_PORTB_REG, u8_a_pinId ); break;
			case DIO_U8_PORTC: TOG_BIT( DIO_U8_PORTC_REG, u8_a_pinId ); break;
			case DIO_U8_PORTD: TOG_BIT( DIO_U8_PORTD_REG, u8_a_pinId ); break;
		}
	}
	/* Check 2: PortId or PinId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId or PinId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPortDirection
 Input: u8 PortId and u8 PortDirection
 Output: u8 Error or No Error
 Description: Function to set Port direction.
*/
u8 DIO_setPortDirection( u8 u8_a_portId, u8 u8_a_portDirection )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId is in the valid range */
	if( u8_a_portId <= DIO_U8_PORTD )
	{		
		/* Check 1.1: Required PortId */
		switch( u8_a_portId )
		{
			case DIO_U8_PORTA: DIO_U8_DDRA_REG = u8_a_portDirection; break;
			case DIO_U8_PORTB: DIO_U8_DDRB_REG = u8_a_portDirection; break;
			case DIO_U8_PORTC: DIO_U8_DDRC_REG = u8_a_portDirection; break;
			case DIO_U8_PORTD: DIO_U8_DDRD_REG = u8_a_portDirection; break;
		}
	}
	/* Check 2: PortId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_setPortValue
 Input: u8 PortId and u8 PortValue
 Output: u8 Error or No Error
 Description: Function to set Port value.
*/
u8 DIO_setPortValue	   ( u8 u8_a_portId, u8 u8_a_portValue )
{
	/* Define Local Variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId is in the valid range */
	if( u8_a_portId <= DIO_U8_PORTD )
	{
		/* Check 1.1: Required PortId */
		switch( u8_a_portId )
		{
			case DIO_U8_PORTA: DIO_U8_PORTA_REG = u8_a_portValue; break;
			case DIO_U8_PORTB: DIO_U8_PORTB_REG = u8_a_portValue; break;
			case DIO_U8_PORTC: DIO_U8_PORTC_REG = u8_a_portValue; break;
			case DIO_U8_PORTD: DIO_U8_PORTD_REG = u8_a_portValue; break;
		}
	}
	/* Check 2: PortId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong PortId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_getPortValue
 Input: u8 PortId and Pointer to u8 ReturnedPortValue
 Output: u8 Error or No Error
 Description: Function to get Port value.
*/
u8 DIO_getPortValue	   ( u8 u8_a_portId, u8 *pu8_a_returnedPortValue )
{
	/* Define Local Variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: PortId is in the valid range, and Pointer is not equal to NULL */
	if( ( u8_a_portId <= DIO_U8_PORTD ) && ( pu8_a_returnedPortValue != NULL ) )
	{
		/* Check 1.1: Required PortId */
		switch( u8_a_portId )
		{
			case DIO_U8_PORTA: *pu8_a_returnedPortValue = DIO_U8_PINA_REG; break;
			case DIO_U8_PORTB: *pu8_a_returnedPortValue = DIO_U8_PINB_REG; break;
			case DIO_U8_PORTC: *pu8_a_returnedPortValue = DIO_U8_PINC_REG; break;
			case DIO_U8_PORTD: *pu8_a_returnedPortValue = DIO_U8_PIND_REG; break;
		}		
	}
	/* Check 2: PortId is not in the valid range, or Pointer is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong PortId, or Pointer is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DIO_togglePortValue
 Input: u8 PortId
 Output: u8 Error or No Error
 Description: Function to toggle Port value.
*/
u8 DIO_togglePortValue ( u8 u8_a_portId )
{
	/* Define Local Variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: PortId is in the valid range */
	if ( u8_a_portId <= DIO_U8_PORTD )
	{		
		/* Define Local Variable to get Port Value */
		u8 u8_l_portValue;
		
		/* Check 1.1: Required PortId */
		switch ( u8_a_portId )
		{
			case DIO_U8_PORTA: DIO_getPortValue( DIO_U8_PORTA, &u8_l_portValue ); DIO_setPortValue( DIO_U8_PORTA, ~u8_l_portValue ); break;
			case DIO_U8_PORTB: DIO_getPortValue( DIO_U8_PORTB, &u8_l_portValue ); DIO_setPortValue( DIO_U8_PORTB, ~u8_l_portValue ); break;
			case DIO_U8_PORTC: DIO_getPortValue( DIO_U8_PORTC, &u8_l_portValue ); DIO_setPortValue( DIO_U8_PORTC, ~u8_l_portValue ); break;
			case DIO_U8_PORTD: DIO_getPortValue( DIO_U8_PORTD, &u8_l_portValue ); DIO_setPortValue( DIO_U8_PORTD, ~u8_l_portValue ); break;
		}
	}
	/* Check 2: PortId is not in the valid range*/
	else
	{
		/* Update error state = NOK, wrong PortId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
		
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/