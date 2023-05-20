/*
 * usi_config.h
 *
 *   Created on: May 16, 2023
 *       Author: 
 *  Description: This file contains all Ultrasonic Interface (USI) pre-build configurations, through which user can configure before using the USI peripheral.
 */

#ifndef USI_CONFIG_H_
#define USI_CONFIG_H_

/*******************************************************************************************************************************************************************/
/* USI Configurations */

/* The Port connected to USI */
/* Options: DIO_U8_PORTA
			DIO_U8_PORTB
			DIO_U8_PORTC
			DIO_U8_PORTD
 */
#define USI_U8_PORT			DIO_U8_PORTA

/* The Pins connected to USI */
/* Options: DIO_U8_PIN0
			DIO_U8_PIN1
			DIO_U8_PIN2
			DIO_U8_PIN3
			DIO_U8_PIN4
			DIO_U8_PIN5
			DIO_U8_PIN6
			DIO_U8_PIN7
 */
#define USI_U8_1_PIN		DIO_U8_PIN0
#define USI_U8_2_PIN		DIO_U8_PIN1

/* End of Configurations */

/*******************************************************************************************************************************************************************/

#endif /* USI_CONFIG_H_ */