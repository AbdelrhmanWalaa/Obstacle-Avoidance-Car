/*
 * dcm_config.h
 *
 *    Created on: Apr 11, 2023
 *        Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *   Description: This file contains all Direct Current Motor (DCM) pre-build configurations, through which user can configure before using the DCM peripheral.
 * MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
 */

#ifndef DCM_CONFIG_H_
#define DCM_CONFIG_H_

/*******************************************************************************************************************************************************************/
/* ATmega32(L) interfacing with DCM: */
/*
								 One-Direction Mode											Two-Direction Mode
				  --------------                   ----------				 --------------                   ----------
				 |   ATmega32   |                 |   DCM    |				|   ATmega32   |                 |   DCM    |
				 |              |                 |          |				|              |                 |          |
				 | 	  (OCR0) PB4|---------------->|Ctrl Pin  |				| 	 (OC1B) PD4|---------------->|CW        |
				  -------------- 				   ----------				| 	 (OC1A) PD5|---------------->|CCW       |
																			 --------------                   ----------
 */
 
/*******************************************************************************************************************************************************************/
/* DCM Configurations' Definitions */
 
// #define MOT0_EN_PIN_NUMBER_0				4
// #define	MOT0_EN_PIN_NUMBER_1				5	
// #define	MOT0_PWM_PIN_NUMBER					0
// #define	MOT0_EN_PORT_NUMBER					PORT_C
// #define	MOT0_PWM_PORT_NUMBER				PORT_C
// 
// #define MOT1_EN_PIN_NUMBER_0				6
// #define	MOT1_EN_PIN_NUMBER_1				7
// #define	MOT1_PWM_PIN_NUMBER					1
// #define	MOT1_EN_PORT_NUMBER					PORT_C
// #define	MOT1_PWM_PORT_NUMBER				PORT_C

/* End of Configurations' Definitions */

/*******************************************************************************************************************************************************************/
/* DCM Configurations */


/* End of Configurations */

/*******************************************************************************************************************************************************************/

#endif /* DCM_CONFIG_H_ */