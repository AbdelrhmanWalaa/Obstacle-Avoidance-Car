/*
 * lcd_program.c
 *
 *   Created on: MAY 16, 2023
 *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *  Description: This file contains all Liquid Crystal Display (LCD) functions' implementation.
 *    Datasheet: https://datasheetspdf.com/pdf/746281/Topway/LMB161ABC-1/1
 *    Model No.: LMB161A - LCD
 */

/* HAL */
#include "lcd_config.h"
#include "lcd_interface.h"

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_initialization
 Input: void
 Output: void
 Description: Function to initialize ( both 4 Bit and 8 Bit Modes ) LCD peripheral.
*/
void LCD_init(void)
{	
	#if Mode == bit_8							//if LCD mode chosen in 8bit mode
	DIO_init (LCD_Data_Port, D7 ,OUT);			//make data7 pin output
	DIO_init (LCD_Data_Port, D6 ,OUT);			//make data6 pin output
	DIO_init (LCD_Data_Port, D5 ,OUT);			//make data5 pin output
	DIO_init (LCD_Data_Port, D4 ,OUT);			//make data4 pin output
	DIO_init (LCD_Data_Port, D3 ,OUT);			//make data3 pin output
	DIO_init (LCD_Data_Port, D2 ,OUT);			//make data2 pin output
	DIO_init (LCD_Data_Port, D1 ,OUT);			//make data1 pin output
	DIO_init (LCD_Data_Port, D0 ,OUT);			//make data0 pin output
	DIO_init (LCD_cmmnd_Port, EN ,OUT);			//make enable pin output
	DIO_init (LCD_cmmnd_Port, RW ,OUT);			//make Rw pin output
	DIO_init (LCD_cmmnd_Port, RS ,OUT);			//make Rs pin output
	_delay_ms(20);								//LCD power on delay is always more than 15ms
	LCD_sendcommand(0x38);						//initialization LCD 16x2 in 8bit mode
	LCD_sendcommand(0x0C);						//display on cursor off
	LCD_sendcommand(0x06);						//auto increment cursor
	LCD_sendcommand(0x01);						//clear display
	LCD_sendcommand(0x80);						//cursor at home position
	#elif Mode == bit_4							//if LCD mode chosen in 4bit mode
	DIO_init (LCD_Data_cmmnd_Port, D7 ,OUT);	//make data7 pin output
	DIO_init (LCD_Data_cmmnd_Port, D6 ,OUT);	//make data6 pin output
	DIO_init (LCD_Data_cmmnd_Port, D5 ,OUT);	//make data5 pin output
	DIO_init (LCD_Data_cmmnd_Port, D4 ,OUT);	//make data4 pin output
	DIO_init (LCD_Data_cmmnd_Port, EN ,OUT);	//make enable pin output
	DIO_init (LCD_Data_cmmnd_Port, RW ,OUT);	//make rw pin output
	DIO_init (LCD_Data_cmmnd_Port, RS ,OUT);	//make rs pin output
	_delay_ms(20);								//LCD power on delay is always more than 15ms
	LCD_sendcommand(0x02);						//initialization LCD in 4bit mode
	LCD_sendcommand(0x28);						//2 lines, 8x5 pixels in 4bit mode
	LCD_sendcommand(0x0C);						//display on cursor off
	LCD_sendcommand(0x06);						//auto increment cursor
	LCD_sendcommand(0x01);						//clear display
	LCD_sendcommand(0x80);						//cursor at home position
	#endif
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_sendcommand
 Input: u8 cmnd
 Output: void
 Description: Function to send a Command to LCD through Data pins.
*/
void LCD_sendcommand (u8 cmnd)
{
	#if Mode == bit_8								//if LCD mode chosen in 8bit mode
	DIO_setPortValue(LCD_Data_Port,cmnd);			//LCD Data Port = cmnd
	DIO_write (LCD_cmmnd_Port, RS, LOW);			//RS = 0 Command register
	DIO_write (LCD_cmmnd_Port, RW, LOW);			//RW = 0 write operation
	DIO_write (LCD_cmmnd_Port, EN, HIGH);			//EN = 1 high pulse
	_delay_us(1);									//delay 1us is always more than 450ns
	DIO_write (LCD_cmmnd_Port, EN, LOW);			//EN = 0 low pulse
	_delay_ms(3);									//delay 3ms
	#elif Mode == bit_4								//if LCD mode chosen in 4bit mode
	DIO_higher_nipple(LCD_Data_cmmnd_Port, cmnd);	//Sending upper nipple of cmnd to LCD Data Port
	DIO_write (LCD_Data_cmmnd_Port, RS ,LOW);		//RS = 0 Command register
	DIO_write (LCD_Data_cmmnd_Port, RW, LOW);		//RW = 0 write operation
	DIO_write (LCD_Data_cmmnd_Port, EN, HIGH);		//EN = 1 high pulse
	_delay_us(1);									//delay 1us is always more than 450ns
	DIO_write (LCD_Data_cmmnd_Port, EN ,LOW);		//EN = 0 low pulse
	_delay_ms(2);									//delay 2ms
	DIO_lower_nipple(LCD_Data_cmmnd_Port , cmnd);	//Sending lower nipple of cmnd to LCD Data Port
	DIO_write (LCD_Data_cmmnd_Port, RS ,LOW);		//RS = 0 Command register
	DIO_write (LCD_Data_cmmnd_Port, RW ,LOW);		//RW = 0 write operation
	DIO_write (LCD_Data_cmmnd_Port, EN ,HIGH);		//EN = 1 high pulse
	_delay_us(1);									//delay 1us is always more than 450ns
	DIO_write (LCD_Data_cmmnd_Port, EN ,LOW);		//EN = 0 low pulse
	_delay_ms(3);									//delay 3ms
	#endif
}


/*******************************************************************************************************************************************************************/
/*
 Name: LCD_sendChar
 Input: u8 Char
 Output: void
 Description: Function to send a Character to LCD through Data pins.
*/
void LCD_sendChar (u8 char_data)
{
	#if Mode == bit_8									//if LCD mode chosen in 8bit mode
	DIO_setPortValue(LCD_Data_Port,char_data);			//LCD Data Port = char data
	DIO_write (LCD_cmmnd_Port, RS ,HIGH);				//RS = 1 Data register
	DIO_write (LCD_cmmnd_Port, RW ,LOW);				//RW = 0 write operation
	DIO_write (LCD_cmmnd_Port, EN ,HIGH);				//EN = 1 high pulse
	_delay_us(1);										//delay 1us is always more than 450ns
	DIO_write (LCD_cmmnd_Port, EN ,LOW);				//EN = 0 low pulse
	_delay_ms(1);										//delay 1ms
	#elif Mode == bit_4									//if LCD mode chosen in 4bit mode
	DIO_higher_nipple(LCD_Data_cmmnd_Port,char_data);	//Sending upper nipple of char data to LCD Data Port
	DIO_write (LCD_Data_cmmnd_Port, RS ,HIGH);			//RS = 1 Data register
	DIO_write (LCD_Data_cmmnd_Port, RW ,LOW);			//RW = 0 write operation
	DIO_write (LCD_Data_cmmnd_Port, EN ,HIGH);			//EN = 1 high pulse
	_delay_us(1);										//delay 1us is always more than 450ns
	DIO_write (LCD_Data_cmmnd_Port, EN ,LOW);			//EN = 0 low pulse
	_delay_ms(2);										//delay 2ms
	DIO_lower_nipple(LCD_Data_cmmnd_Port,char_data);	//Sending lower nipple of char data to LCD Data Port
	DIO_write (LCD_Data_cmmnd_Port, RS ,HIGH);			//RS = 1 Data register
	DIO_write (LCD_Data_cmmnd_Port, RW ,LOW);			//RW = 0 write operation
	DIO_write (LCD_Data_cmmnd_Port, EN ,HIGH);			//EN = 1 high pulse
	_delay_us(1);										//delay 1us is always more than 450ns
	DIO_write (LCD_Data_cmmnd_Port, EN ,LOW);			//EN = 0 low pulse
	_delay_ms(2);										//delay 2ms
	#endif
}

/*******************************************************************************************************************************************************************/
/*
 Name: LCD_clear
 Input: void
 Output: void
 Description: Function to clear LCD display screen in DDRAM.
*/
void LCD_clear(void)
{
	/* Step 1: Send Display Clear control command */
	LCD_sendcommand(0x01);					//clear display
	/* Step 2: Delay more than 1.53 ms */
	_delay_ms(2);							//delay 2ms
	LCD_sendcommand(0x80);					//cursor at home position
}
/*******************************************************************************************************************************************************************/
/*
 Name: LCD_setCursor
 Input: u8 row , u8 column
 Output: void
 Description: Function to set the Address Counter (AC) of LCD to a certain location in DDRAM.
*/
void LCD_setCursor (u8 row , u8 column)
{
	if (row == 0 && column<16)
	LCD_sendcommand((column & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && column<16)
	LCD_sendcommand((column & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	
}
/*******************************************************************************************************************************************************************/
/*
 Name: LCD_writeString
 Input: Pointer to u8 String
 Output: vou8 Error or No Error
 Description: Function to send an array of characters to LCD through Data pins ( From CGROM to DDRAM ).
*/

void LCD_sendString (u8 *str)
{
	u16 i;
	for(i = 0;str[i]!= '\0'; i++)
	{
		LCD_sendChar(str[i]);
	}
}


/*******************************************************************************************************************************************************************/
/*
 Name: LCD_floattostring
 Input: f32 float_value
 Output: void
 Description: Function to send a float (one decimal) number ( positive or negative ) to LCD through Data pins ( From CGROM to DDRAM ).
*/
void LCD_floattostring (f32 float_value)
{
	u8 pattern[10] , tempRearrange, digit_count=0,i,j;
	u32 number;
	f32 temp_float = float_value * 10;
	number = temp_float;
	for (i=0;number>0;i++)
	{
		pattern[i] = ((number%10) +'0');
		number/=10;
		digit_count++;
	}
	
	for (j=0,i--;i>j;j++)
	{
		tempRearrange = pattern[i];
		pattern[i] = pattern[j];
		pattern[j] = tempRearrange;
		i--;
	}
	pattern[digit_count] =pattern[digit_count - 1];
	pattern[digit_count - 1] = '.';
	pattern[digit_count + 1] = '\0';
	LCD_sendString(pattern);
}
/*******************************************************************************************************************************************************************/
/*
 Name: LCD_createCustomCharacter
 Input: u8 *pattern , u8 location
 Output: void
 Description: Function to send character ( stored in array -SpecialChar- byte by byte ) and store it in CGRAM, then display it on DDRAM ( From CGRAM to DDRAM ).
*/
void LCD_createCustomCharacter (u8 *pattern , u8 location )
{
	u8 i;
	LCD_sendcommand(( 0x40 + (location*8)));
	for(i = 0; i<8; i++)
	{
		LCD_sendChar(pattern[i]);
	}
}

/*******************************************************************************************************************************************************************/