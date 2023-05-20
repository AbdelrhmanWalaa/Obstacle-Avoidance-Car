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
/*
 Name: DCM_initialization
 Input: void
 Output: void
 Description: Function to Initialize DCM peripheral.
*/
void DCM_initialization( DCM_ST_CONFIG *st_a_DCMConfig )
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
	for ( u8 u8_l_index = 0; u8_l_index < DCM_U8_NUMBER_OF_DCM; u8_l_index++ )
	{
		/* Initialize initial direction of DCM, in both Direction and PWM Pins */
		DIO_init ( st_a_DCMConfig[u8_l_index].u8_g_directionCWPort, st_a_DCMConfig[u8_l_index].u8_g_directionCWPin  , OUT );
		DIO_init ( st_a_DCMConfig[u8_l_index].u8_g_directionCCWPort, st_a_DCMConfig[u8_l_index].u8_g_directionCCWPin, OUT );
		DIO_init ( st_a_DCMConfig[u8_l_index].u8_g_pwmPort, st_a_DCMConfig[u8_l_index].u8_g_pwmPin, OUT );
	
		/* Initialize initial value of DCM, in both Direction Pins */
		DIO_write ( st_a_DCMConfig[u8_l_index].u8_g_directionCWPort, st_a_DCMConfig[u8_l_index].u8_g_directionCWPin  , LOW );
		DIO_write ( st_a_DCMConfig[u8_l_index].u8_g_directionCCWPort, st_a_DCMConfig[u8_l_index].u8_g_directionCCWPin, HIGH );
		TMR2_PWM_Init( 0.2, st_a_DCMConfig[u8_l_index].u8_g_pwmPort, st_a_DCMConfig[u8_l_index].u8_g_pwmPin );
	}
}

/*******************************************************************************************************************************************************************/
/*
 Name: DCM_controlDCM
 Input: Pointer to st DCMConfig, u8 ControlMode, and u8 SpeedPercentage
 Output: u8 Error or No Error
 Description: Function Control DCM with one of DCM Modes.
*/
u8 DCM_controlDCM      ( DCM_ST_CONFIG *pst_a_DCMConfig, u8 u8_a_controlMode, u8 u8_a_speedPercentage )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: Pointer is not equal to NULL, and ControlMode and SpeedPercentage are in the valid range */
	if ( ( pst_a_DCMConfig != NULL ) && ( u8_a_controlMode <= DCM_U8_ROTATE_CCW ) && ( u8_a_speedPercentage <= 100 ) )
	{
		switch ( u8_a_controlMode )
		{
			case DCM_U8_STOP:
			DIO_write( pst_a_DCMConfig->u8_g_directionCWPort, pst_a_DCMConfig->u8_g_directionCWPin , LOW );
			DIO_write( pst_a_DCMConfig->u8_g_directionCCWPort, pst_a_DCMConfig->u8_g_directionCCWPin, LOW );
			break;
			
			case DCM_U8_ROTATE_CW:
			DIO_write( pst_a_DCMConfig->u8_g_directionCWPort, pst_a_DCMConfig->u8_g_directionCWPin , HIGH );
			DIO_write( pst_a_DCMConfig->u8_g_directionCCWPort, pst_a_DCMConfig->u8_g_directionCCWPin, LOW  );
			break;
			
			case DCM_U8_ROTATE_CCW:
			DIO_write( pst_a_DCMConfig->u8_g_directionCWPort, pst_a_DCMConfig->u8_g_directionCWPin , LOW  );
			DIO_write( pst_a_DCMConfig->u8_g_directionCCWPort, pst_a_DCMConfig->u8_g_directionCCWPin, HIGH );
			break;
		}
		
		TMR2_PWM_start( u8_a_speedPercentage );
	}
	/* Check 2: Pointer is equal to NULL, or ControlMode or SpeedPercentage is not in the valid range */
	else
	{
		/* Update error state = NOK, Pointer is NULL, or wrong ControlMode or SpeedPercentage! */
		u8_l_errorState = STD_TYPES_NOK;
	}
}

/*******************************************************************************************************************************************************************/