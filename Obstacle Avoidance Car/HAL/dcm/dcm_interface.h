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
//#include "../../MCAL/pwm/pwm_interface.h"
#include "../../MCAL/tmr2/TMR2_interface.h"

/*******************************************************************************************************************************************************************/
/* DCM Macros */

typedef struct
{
	u8 u8_g_directionCWPort;
	u8 u8_g_directionCWPin;
	u8 u8_g_directionCCWPort;
	u8 u8_g_directionCCWPin;
	u8 u8_g_pwmPort;
	u8 u8_g_pwmPin;
} DCM_ST_CONFIG;

#define DCM_U8_STOP				0
#define DCM_U8_ROTATE_CW		1
#define DCM_U8_ROTATE_CCW		2

/*******************************************************************************************************************************************************************/
/* DCM Functions' Prototypes */

void DCM_initialization( DCM_ST_CONFIG *st_a_DCMConfig );
u8 DCM_controlDCM      ( DCM_ST_CONFIG *st_a_DCMConfig, u8 u8_a_controlMode, u8 u8_a_speedPercentage );

/*******************************************************************************************************************************************************************/

#endif /* DCM_INTERFACE_H_ */