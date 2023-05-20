 /*
  * dcm_interface.h
  *
  *     Created on: Apr 11, 2023
  *         Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
  *    Description: This file contains all Direct Current Motor (DCM) functions' prototypes and definitions (Macros) to avoid magic numbers.
  *	 MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
  */

#ifndef DCM_INTERFACE_H_
#define DCM_INTERFACE_H_

/*******************************************************************************************************************************************************************/
/* DCM Includes */

/* LIB */
#include "../../LIB/std_types/std_types.h"
#include "../../LIB/bit_math/bit_math.h"

/* MCAL */
#include "../../MCAL/dio/dio_interface.h"
#include "../../MCAL/TMR0/TMR0_interface.h"

/*******************************************************************************************************************************************************************/
/* DCM Macros */

// #define MOTORS_NUMBER                 2
// #define ZERO_SPEED					  0
// #define MAX_DUTY_CYCLE			      100
// #define PERIOD_TIME					  10
// #define ROTATION_DUTY_CYCLE           50

//#define MOTOR_RIGHT         0
//#define MOTOR_LEFT			1
//typedef struct {
//	u8 DCM_g_motEnPinNumber0;
//	u8 DCM_g_motEnPinNumber1;
//	u8 DCM_g_motPWMPinNumber;
//	EN_DIO_PORT_T DCM_g_motEnPortNumber;
//	u8 DCM_g_motPWMPortNumber;
//}ST_DCM_g_Config_t;

//typedef enum {
//	DCM_OK,
//	DCM_ERROR
//}EN_DCM_ERROR_T;
//
//typedef enum {
//	MOTOR_RIGHT,
//	MOTOR_LEFT
//}EN_DCM_MOTORSIDE;
//
//typedef enum {
//	FALSE,
//	TRUE
//}EN_DCM_FLAG;

/*******************************************************************************************************************************************************************/
/* DCM Functions' Prototypes */

u8 DCM_motorInit         ( u8 ** u8_a_shutdownFlag );
u8 DCM_rotateDCM         ( void );
u8 DCM_changeDCMDirection( u8 u8_a_motorId );
u8 DCM_setDutyCycleOfPWM ( u8 u8_a_dutyCycleValue );
vd DCM_stopDCM           ( void );
vd DCM_updateStopFlag    ( void );

/*******************************************************************************************************************************************************************/

#endif /* DCM_INTERFACE_H_ */