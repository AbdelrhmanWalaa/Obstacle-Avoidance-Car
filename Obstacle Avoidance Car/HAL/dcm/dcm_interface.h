 /*
  * dcm_interface.h
  *
  *   Created on: Apr 11, 2023
  *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
  *  Description: This file contains all Direct Current Motor (DCM) functions' prototypes and definitions (Macros) to avoid magic numbers.
  *    Datasheet: https://datasheetspdf.com/pdf/917207/KYSAN/RF-300CA-11440/1
  *    Model No.: RF-300CA-11440 - DCM
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
#include "../../MCAL/pwm/pwm_interface.h"

/*******************************************************************************************************************************************************************/
/* DCM Macros */

// #define MOTORS_NUMBER                 2
// #define ZERO_SPEED					  0
// #define MAX_DUTY_CYCLE			      100
// #define PERIOD_TIME					  10
// #define ROTATION_DUTY_CYCLE           50

#define DCM_U8_RIGHT_DCM    0
#define DCM_U8_LEFT_DCM		1



typedef struct
{
	u8 DCM_U8_DCM_DIR_PORT;
	u8 DCM_U8_DCM_DIR_PIN_A;
	u8 DCM_U8_DCM_DIR_PIN_B;
	u8 DCM_U8_DCM_PWM_PORT;
	u8 DCM_U8_DCM_PWM_PIN;
} DCM_ST_CONFIG;

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

void DCM_initialization( void );
//u8 DCM_rotateDCM       ( u8 u8_a_DCMId, u8 u8_a_rotateDirection, u8 u8_a_speedPercentage );
u8 DCM_rotateDCM       ( u8 u8_a_speedPercentage );
void DCM_stopDCM       ( void );

/*******************************************************************************************************************************************************************/

#endif /* DCM_INTERFACE_H_ */