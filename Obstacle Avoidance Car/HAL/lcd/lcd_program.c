/*
 * lcd_program.c
 *
 *   Created on: Apr 20, 2021
 *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *  Description: This file contains all Liquid Crystal Display (LCD) functions' implementation.
 *    Datasheet: https://datasheetspdf.com/pdf/746281/Topway/LMB161ABC-1/1
 *    Model No.: LMB161A - LCD
 */

/* HAL */
#include "lcd_private.h"
#include "lcd_config.h"
#include "lcd_interface.h"

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_initialization
 Input: void
 Output: void
 Description: Function to initialize ( both 4 Bit and 8 Bit Modes ) LCD peripheral.
*/
vd LCD_initialization   ( void )
{
	/* Step 1: Delay more than 30 ms */
	_delay_ms(35);

	/* Check: Required LCD Mode */
	switch ( LCD_U8_MODE_SELECT )
	{
		case LCD_U8_4_BIT_MODE:

			/* Step 2: Send Function Set Control Command with 4 Bit Mode */
			LCD_sendCmnd( 0x02 );
			LCD_sendCmnd( LCD_U8_FUNC_SET_4_BIT );

		break;

		case LCD_U8_8_BIT_MODE:

			/* Step 2: Send Function Set Control Command with 8 Bit Mode */
			LCD_sendCmnd( LCD_U8_FUNC_SET_8_BIT );
			
		break;
	}

	/* Step 3: Delay more than 39 us */
	_delay_us(40);

	/* Step 4: Send Display On/Off control command with Display on, Cursor on, and Blink on */
	LCD_sendCmnd( LCD_U8_DISP_ON_OFF_CTRL );

	/* Step 5: Delay more than 39 us */
	_delay_us(40);

	/* Step 6: Send Display Clear control command */
	LCD_sendCmnd( LCD_U8_DISP_CLEAR );

	/* Step 7: Delay more than 1.53 ms */
	_delay_ms(2);

	/* Step 8: Send Entry Mode control command with Increment on and Shift off */
	LCD_sendCmnd( LCD_U8_ENTRY_MODE );
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_sendCmnd
 Input: u8 Cmnd
 Output: void
 Description: Function to send a Command to LCD through Data pins.
*/
vd LCD_sendCmnd		    ( u8 u8_a_cmnd )
{
	/* Step 1: Set RS = 0 ( i.e. Command ) */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_RS_PIN, DIO_U8_PIN_LOW );

	/* Step 2: Set RW = 0 ( i.e. Write ) */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_RW_PIN, DIO_U8_PIN_LOW );

	/* Check 1: Required LCD Mode */
	switch ( LCD_U8_MODE_SELECT )
	{
		case LCD_U8_4_BIT_MODE:

			/* Check 1.1: Required Pins Location in 4-bit Mode */
			switch ( LCD_U8_DATA_PINS_LOCATION )
			{				
				/* Define Local Variable to get Port Value */
				u8 u8_l_lcdPortValue = 0;
		
				case LCD_U8_LOWER_PORT_PINS:

					/* 
					 * Step 3: Put the Command on the Data pins 
					 */

					/* Step 3.1: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.2: Put the Upper nibble Command on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0xF0 ) | ( u8_a_cmnd >> 4 );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

					/* Step 3.3: Send Enable high and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_HIGH );
					_delay_us(1);

					/* Step 3.4: Send Enable low and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_LOW );
					_delay_us(1);

					/* Step 3.5: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.6: Put the Lower nibble Command on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0xF0 ) | ( u8_a_cmnd & 0x0F );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

				break;

				case LCD_U8_UPPER_PORT_PINS:

					/* 
					 * Step 3: Put the Command on the DATA pins 
					 */

					/* Step 3.1: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.2: Put the Upper nibble Command on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0x0F ) | ( u8_a_cmnd & 0xF0 );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

					/* Step 3.3: Send Enable high and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_HIGH );
					_delay_us(1);

					/* Step 3.4: Send Enable low and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_LOW );
					_delay_us(1);

					/* Step 3.5: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.6: Put the Lower nibble Command on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0x0F ) | ( u8_a_cmnd << 4 );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

				break;
			}			
		break;
	
		case LCD_U8_8_BIT_MODE:

			/* Step 3: Put the Command on the Data pins */
			DIO_setPortValue( LCD_U8_DATA_PORT, u8_a_cmnd );

		break;
	}

	/* Step 7: Send Enable high and delay 230 ns -> 1 us */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_HIGH );
	_delay_us(1);

	/* Step 8: Send Enable low and delay 230 ns -> 1 us */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_LOW );
	_delay_us(1);
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_sendChar
 Input: u8 Char
 Output: void
 Description: Function to send a Character to LCD through Data pins.
*/
vd LCD_sendChar		   ( u8 u8_a_char )
{
	/* Step 1: Set RS = 1 (i.e. Data) */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_RS_PIN, DIO_U8_PIN_HIGH );

	/* Step 2: Set RW = 0 (i.e. Write) */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_RW_PIN, DIO_U8_PIN_LOW );

	/* Check 1: Required LCD Mode */
	switch ( LCD_U8_MODE_SELECT )
	{
		case LCD_U8_4_BIT_MODE:

			/* Check 1.1: Required Pins Location in 4-bit Mode */
			switch ( LCD_U8_DATA_PINS_LOCATION )
			{
				/* Define Local Variable to get Port Value */
				u8 u8_l_lcdPortValue = 0;

				case LCD_U8_LOWER_PORT_PINS:

					/* 
					 * Step 3: Put the Character on the Data pins 
					 */

					/* Step 3.1: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.2: Put the Upper nibble Character on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0xF0 ) | ( u8_a_char >> 4 );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

					/* Step 3.3: Send Enable high and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_HIGH );
					_delay_us(1);

					/* Step 3.4: Send Enable low and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_LOW );
					_delay_us(1);

					/* Step 3.5: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );					

					/* Step 3.6: Put the Lower nibble Character on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0xF0 ) | ( u8_a_char & 0x0F );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );				

				break;

				case LCD_U8_UPPER_PORT_PINS:

					/* 
					 * Step 3: Put the Character on the DATA pins
					 */

					/* Step 3.1: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.2: Put the Upper nibble Character on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0x0F ) | ( u8_a_char & 0xF0 );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

					/* Step 3.3: Send Enable high and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_HIGH );
					_delay_us(1);

					/* Step 3.4: Send Enable low and delay 230 ns -> 1 us */
					DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_LOW );
					_delay_us(1);

					/* Step 3.5: Get LCD Port Value */
					DIO_getPortValue( LCD_U8_DATA_PORT, &u8_l_lcdPortValue );

					/* Step 3.6: Put the Lower nibble Character on the Data pins, without altering the rest of Port pins. */
					u8_l_lcdPortValue = ( u8_l_lcdPortValue & 0x0F ) | ( u8_a_char << 4 );
					DIO_setPortValue( LCD_U8_DATA_PORT, u8_l_lcdPortValue );

				break;
			}			
		break;
	
		case LCD_U8_8_BIT_MODE:

			/* Step 3: Put the Character on the Data pins */
			DIO_setPortValue( LCD_U8_DATA_PORT, u8_a_char );

		break;
	}

	/* Step 4: Send Enable high and delay 230 ns -> 1 us */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_HIGH );
	_delay_us(1);

	/* Step 5: Send Enable low and delay 230 ns -> 1 us */
	DIO_setPinValue( LCD_U8_CTRL_PORT, LCD_U8_EN_PIN, DIO_U8_PIN_LOW );
	_delay_us(1);
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_clearDisplay
 Input: void
 Output: void
 Description: Function to clear LCD display screen in DDRAM.
*/
vd LCD_clearDisplay	   ( void )
{
	/* Step 1: Send Display Clear control command */
	LCD_sendCmnd( LCD_U8_DISP_CLEAR );

	/* Step 2: Delay more than 1.53 ms */
	_delay_ms(2);
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_goToLocation
 Input: u8 LineNumber and u8 DisplayLocation
 Output: u8 Error or No Error
 Description: Function to set the Address Counter (AC) of LCD to a certain location in DDRAM.
*/
u8 LCD_goToLocation	   ( u8 u8_a_lineNumber, u8 u8_a_displayLocation )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: LineNumber and the Location are in the valid range */
	if ( ( u8_a_lineNumber <= LCD_U8_LINE2 ) && ( u8_a_displayLocation <= LCD_U8_LINE_LOCATIONS ) )
	{
		/* Check 1.1: the Required LineNumber */
		switch ( u8_a_lineNumber )
		{
			case LCD_U8_LINE1: LCD_sendCmnd( LCD_U8_LOC_SET_TO_LINE1 + u8_a_displayLocation ); break;
			case LCD_U8_LINE2: LCD_sendCmnd( LCD_U8_LOC_SET_TO_LINE2 + u8_a_displayLocation ); break;
		}
	}
	/* Check 2: LineNumber or the Location is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong LineNumber or Location! */
		u8_l_errorState = STD_TYPES_NOK;
	}

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_writeString
 Input: Pointer to u8 String
 Output: vou8 Error or No Error
 Description: Function to send an array of characters to LCD through Data pins ( From CGROM to DDRAM ).
*/
u8 LCD_writeString 	   ( u8 *pu8_a_string )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: Pointer is not equal to NULL */
	if ( pu8_a_string != NULL )
	{
		/* Loop: Until the end of string */
		while ( *pu8_a_string != '\0' )
		{
			LCD_sendChar( *pu8_a_string );
			/* Increment String */
			pu8_a_string++;
		}
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
 Name: LCD_writeNumber
 Input: s64 Number
 Output: void
 Description: Function to send a number ( positive or negative ) to LCD through Data pins ( From CGROM to DDRAM ).
*/
vd LCD_writeNumber	   ( s64 s64_a_number )
{
	u8  au8_l_number[LCD_U8_NUM_OF_DIGITS_OF_S64];
	s64 s64_l_tempNumber = 0;
	s8  s8_l_index = LCD_U8_FIRST_ELEMENT;

	/* Step 1: Check if the Number is negative */
	if ( s64_a_number < 0 )
	{
		/* Save the Number into TempNumber */
		s64_l_tempNumber = s64_a_number;
		/* Convert the Number to positive */
		s64_a_number = s64_a_number - ( s64_a_number * 2 );
	}

	/* Step 2: Save the Number in a reversed manner in NumberArray */
	while ( s64_a_number >= 10 )
	{
		/* Use modulo 10 to get last digit, and save this digit to NumberArray */
		au8_l_number[s8_l_index] = s64_a_number % 10;

		/* Divide Number by 10 */
		s64_a_number = s64_a_number / 10;
		s8_l_index++;
	}

	/* Step 3: Put the first digit in the array when Number < 10 */
	au8_l_number[s8_l_index] = s64_a_number;

	/* Step 4: Check if the TempNumber was negative, display '-' sign before the TempNumber on LCD */
	if ( s64_l_tempNumber < 0 )
	{
		LCD_sendChar( '-' );
	}

	/* Step 5: Display Number on LCD */
	while ( s8_l_index >= LCD_U8_FIRST_ELEMENT )
	{
		LCD_sendChar( au8_l_number[s8_l_index] + '0' );
		s8_l_index--;
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_writeSpecialChar
 Input: u8 StoreLocation, u8 LineNumber, u8 DisplayLocation, and Pointer to u8 SpecialChar
 Output: u8 Error or No Error
 Description: Function to send character ( stored in array -SpecialChar- byte by byte ) and store it in CGRAM, then display it on DDRAM ( From CGRAM to DDRAM ).
*/
u8 LCD_writeSpecialChar( u8 u8_a_storeLocation, u8 u8_a_lineNumber, u8 u8_a_displayLocation, u8 *pu8_a_specialChar )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Step 1: Set CGRAM in Address Counter (AC) */

	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/