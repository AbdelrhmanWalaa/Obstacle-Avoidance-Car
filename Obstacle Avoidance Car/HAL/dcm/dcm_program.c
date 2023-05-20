/*
 * dcm_program.c
 *
 *   Created on: Apr 11, 2023
 *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *  Description: This file contains all Direct Current Motor (DCM) functions' implementation.
 *    Datasheet: https://datasheetspdf.com/pdf/917207/KYSAN/RF-300CA-11440/1
 *    Model No.: RF-300CA-11440 - DCM
 */

/* HAL */
#include "dcm_config.h"
#include "dcm_interface.h"

/*******************************************************************************************************************************************************************/
/* Declaration and Initialization */

/* Global Array of Structures to store initial DCM configurations. */
 DCM_ST_CONFIG ast_g_DCMs[2] =
 {
 	{ DCM_U8_DCM_R_DIR_PORT, DCM_U8_DCM_R_DIR_PIN_1A, DCM_U8_DCM_R_DIR_PIN_1B, DCM_U8_DCM_R_PWM_PORT, DCM_U8_DCM_R_PWM_PIN },
 	{ DCM_U8_DCM_L_DIR_PORT, DCM_U8_DCM_L_DIR_PIN_2A, DCM_U8_DCM_L_DIR_PIN_2B, DCM_U8_DCM_L_PWM_PORT, DCM_U8_DCM_L_PWM_PIN }
 };
 
/*******************************************************************************************************************************************************************/
/*
 Name: DCM_initialization
 Input: void
 Output: void
 Description: Function to Initialize DCM peripheral.
*/
void DCM_initialization  ( void )
{	
	/* Method 1: Static (pre-compile) Initialization */
	/* Initialize initial direction of right DCM, in both Direction and PWM Pins */	
// 	DIO_init( DCM_U8_DCM_R_DIR_PORT, DCM_U8_DCM_R_DIR_PIN_1A, OUT );
// 	DIO_init( DCM_U8_DCM_R_DIR_PORT, DCM_U8_DCM_R_DIR_PIN_1B, OUT );
// 	DIO_init( DCM_U8_DCM_R_PWM_PORT, DCM_U8_DCM_R_PWM_PIN, OUT );
	
	/* Initialize initial value of right DCM, in both Direction Pins */	
// 	DIO_write( DCM_U8_DCM_R_DIR_PORT, DCM_U8_DCM_R_DIR_PIN_1A, HIGH );
// 	DIO_write( DCM_U8_DCM_R_DIR_PORT, DCM_U8_DCM_R_DIR_PIN_1B, LOW );
	
	/* Initialize initial direction of left DCM, both Direction and PWM Pins */	
// 	DIO_init( DCM_U8_DCM_L_DIR_PORT, DCM_U8_DCM_L_DIR_PIN_2A, OUT );
// 	DIO_init( DCM_U8_DCM_L_DIR_PORT, DCM_U8_DCM_L_DIR_PIN_2B, OUT );
// 	DIO_init( DCM_U8_DCM_L_PWM_PORT, DCM_U8_DCM_L_PWM_PIN, OUT );
	
	/* Initialize initial value of left DCM, in both Direction Pins */	
// 	DIO_write( DCM_U8_DCM_L_DIR_PORT, DCM_U8_DCM_L_DIR_PIN_2A, HIGH );
// 	DIO_write( DCM_U8_DCM_L_DIR_PORT, DCM_U8_DCM_L_DIR_PIN_2B, LOW );

	/* Method 2: Linking Configuration Initialization */
	for (u8 u8_l_index = 0; u8_l_index < 2; u8_l_index++)
	{
		/* Initialize initial direction of DCM, in both Direction and PWM Pins */	
		DIO_init ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PIN_A, OUT );
		DIO_init ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PIN_B, OUT );
		DIO_init ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_PWM_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_PWM_PIN, OUT );
		
		/* Initialize initial value of DCM, in both Direction Pins */	
		DIO_write ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PIN_A, HIGH );
		DIO_write ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PIN_B, LOW );
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DCM_rotateDCM
 Input: u8 SpeedPercentage
 Output: u8 Error or No Error
 Description: Function Rotate DCM.
*/
u8 DCM_rotateDCM         ( u8 u8_a_speedPercentage )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
		
	/* Check 1: SpeedPercentage is in the valid range */
	if ( u8_a_speedPercentage <= 100 )
	{
		PWM_generatePWM( u8_a_speedPercentage );
	}
	/* Check 2: SpeedPercentage is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong SpeedPercentage! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DCM_changeDCMDirection
 Input: u8 DCMId
 Output: u8 Error or No Error
 Description: Function to Change DCM Direction.
*/
u8 DCM_changeDCMDirection( u8 u8_a_DCMId )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
		
	/* Check 1: DCMId is in the valid range */
	if ( u8_a_DCMId <= DCM_U8_LEFT_DCM )
	{
		DIO_toggle ( ast_g_DCMs[u8_a_DCMId].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_a_DCMId].DCM_U8_DCM_DIR_PIN_A );
		DIO_toggle ( ast_g_DCMs[u8_a_DCMId].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_a_DCMId].DCM_U8_DCM_DIR_PIN_B );
	}
	/* Check 2: DCMId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong DCMId! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: DCM_stopDCM
 Input: void
 Output: void
 Description: Function to Stop DCM.
*/
void DCM_stopDCM         ( void )
{
	for (u8 u8_l_index = 0; u8_l_index < 2; u8_l_index++)
	{
		DIO_write ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PIN_A, LOW );
		DIO_write ( ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PORT, ast_g_DCMs[u8_l_index].DCM_U8_DCM_DIR_PIN_B, LOW );
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name:
 Input:
 Output:
 Description: Function to .
*/
// EN_DCM_ERROR_T DCM_setDutyCycleOfPWM(u8 u8_a_dutyCycleValue)
// {
// 
// 	if (u8_a_dutyCycleValue > MAX_DUTY_CYCLE)
// 		return DCM_ERROR;
// 	else
// 	{
// 		u8 DCM_a_mappedDuty = u8_a_dutyCycleValue / PERIOD_TIME;
// 		u16 u16_onTime = DCM_a_mappedDuty;
// 		u16 u16_offTime = PERIOD_TIME - DCM_a_mappedDuty;
// 
// 		while (en_g_stopFlag != TRUE && (u8_g_shutdownFlag == NULL || *u8_g_shutdownFlag == 0))
// 		{
// 			DIO_portWrite(st_g_DCMs[0].DCM_g_motEnPortNumber, DIO_U8_PORT_HIGH, DIO_MASK_BITS_0_1);
// 			TIMER_timer0Delay(u16_onTime);
// 			DIO_portWrite(st_g_DCMs[0].DCM_g_motEnPortNumber, DIO_U8_PORT_LOW, DIO_MASK_BITS_0_1);
// 			TIMER_timer0Delay(u16_offTime);
// 		}
// 		en_g_stopFlag = FALSE;
// 	}
//     return DCM_OK;
// }

/*******************************************************************************************************************************************************************/
/*
 Name:
 Input:
 Output:
 Description: Function to .
*/
// void DCM_updateStopFlag(void)
// {
// 	en_g_stopFlag = TRUE;
// }

/*******************************************************************************************************************************************************************/
/*
 Name:
 Input:
 Output:
 Description: Function to .
*/
// EN_DCM_ERROR_T DCM_rotateDCM()
// {
//     if(u8_g_shutdownFlag != NULL && *u8_g_shutdownFlag == 1) return DCM_ERROR;
// 	DCM_changeDCMDirection(MOTOR_RIGHT);
// 	DCM_setDutyCycleOfPWM (ROTATION_DUTY_CYCLE);
// 	DCM_changeDCMDirection(MOTOR_RIGHT);
//     return DCM_OK;
// }

/*******************************************************************************************************************************************************************/