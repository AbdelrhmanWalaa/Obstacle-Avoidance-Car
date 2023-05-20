/*
* app_program.c
*
*   Created on: May 16, 2023
*       Author: Team 3 - https://github.com/AbdelrhmanWalaa/Obstacle-Avoidance-Car.git
*  Description: This file contains all Application (APP) functions' implementation.
*/

/* APP */
#include "app_config.h"
#include "app_interface.h"

/*******************************************************************************************************************************************************************/
/* Declaration and Initialization */

 DCM_ST_CONFIG ast_g_DCMs[2] =
 {
	 { APP_U8_DCM_R_DIR_CW_PORT, APP_U8_DCM_R_DIR_CW_PIN, APP_U8_DCM_R_DIR_CCW_PORT, APP_U8_DCM_R_DIR_CCW_PIN, APP_U8_DCM_R_PWM_PORT, APP_U8_DCM_R_PWM_PIN },
	 { APP_U8_DCM_L_DIR_CW_PORT, APP_U8_DCM_L_DIR_CW_PIN, APP_U8_DCM_L_DIR_CCW_PORT, APP_U8_DCM_L_DIR_CCW_PIN, APP_U8_DCM_L_PWM_PORT, APP_U8_DCM_L_PWM_PIN }
 };
 
/* Global variable to store appMode */
// static u8 u8_gs_appMode = APP_CAR_STOP;
// static u8 u8_gs_diagonalFlag = APP_LONG_DGNL;
//
// u8 u8_g_suddenBreakFlag = APP_BREAK_FLAG_DOWN;
// u8 *u8Ptr_g_suddenBreakPtr = &u8_g_suddenBreakFlag;

/*******************************************************************************************************************************************************************/
/*
Name:
Input:
Output:
Description: Function to .
*/
void APP_initialization(void)
{
	/* MCAL Initialization */
	GLI_enableGIE();
// 
// 	EXI_intSetCallBack( EXI_U8_INT0, &APP_stopCar );
// 	EXI_enablePIE( EXI_U8_INT0, EXI_U8_SENSE_FALLING_EDGE );
// 
// 	EXI_intSetCallBack( EXI_U8_INT1, &APP_startCar );
// 	EXI_enablePIE( EXI_U8_INT1, EXI_U8_SENSE_FALLING_EDGE );
// 
// 	TIMER_timer0NormalModeInit( DISABLED, &u8Ptr_g_suddenBreakPtr);
// 	TIMER_timer2NormalModeInit( ENABLED, &u8Ptr_g_suddenBreakPtr);

	/* HAL Initialization */
// 	BTN_init( APP_STOP_BTN , PORT_D );
// 	BTN_init( APP_START_BTN, PORT_D );
// 	LED_arrayInit( PORT_A, DIO_MASK_BITS_0_1_2_3 );
// 	DCM_motorInit(&u8Ptr_g_suddenBreakPtr);
// 
// 	u8_gs_appMode = APP_CAR_STOP;
	//LCD_init();
	//KPD_initialization();
	
	DCM_initialization( ast_g_DCMs );
	
// 	PWM_initialization( DIO_U8_PORTD, DIO_U8_PIN0, 1 );
// 	
// 	PWM_generatePWM( 50 );
// 
// 	TMR2_PWM_Init (1, 3, 0);
// 	TMR2_PWM_start(50);

	DCM_controlDCM( &ast_g_DCMs[0], DCM_U8_ROTATE_CCW, 10 );
}

/*******************************************************************************************************************************************************************/
/*
Name:
Input:
Output:
Description: Function to .
*/
void APP_startProgram(void)
{
// 	u8 u8_l_keyValue = KPD_U8_KEY_NOT_PRESSED;
// 	
// 	while ( u8_l_keyValue != '1' )
// 	{
// 		KPD_getPressedKey( &u8_l_keyValue );
// 	}
	
	/* Toggle forever */
	while (1)
	{
		
		//PWM_generatePWM( 50 );
// 		if ( u8_l_keyValue != KPD_U8_KEY_NOT_PRESSED )
// 		{
// 			LCD_sendCharacter( u8_l_keyValue );
// 			
// 			switch (u8_l_keyValue)
// 			{
// 				case '1':  break;
// 				case '2':  break;
// 			}
// 		}
		

// 		/* Check 1: Required appMode */
// 		switch ( u8_gs_appMode )
// 		{
// 			case APP_CAR_STOP:
// 			
// 				/* Step A1: Turn on red LED, and turn off other LEDs */
// 				LED_arrayOff( PORT_A, DIO_MASK_BITS_0_1_2 );
// 				LED_on( PORT_A, APP_STOP_LED );
// 				/* Step A2: Stop both motors */
// 				DCM_stopDCM();
// 			
// 				break;
// 			
// 			case APP_CAR_START:
// 			
// 				/* Step B1: Delay 1 sec. */
// 				TIMER_timer0Delay( APP_STARTING_DELAY );
// 			
// 				/* Check 1.1: appMode is not "CAR_STOP" mode */
// 				if ( u8_gs_appMode != APP_CAR_STOP )
// 				{
// 					/* Step B2: Update appMode to "CAR_MOVE_FWD_LD" mode */
// 					u8_gs_appMode = APP_CAR_MOVE_FWD_LD;
// 				}
// 				break;
// 			
// 			case APP_CAR_MOVE_FWD_LD:
// 			
// 				/* Step C1: Update diagonalFlag to "LONG_DGNL" */
// 				u8_gs_diagonalFlag = APP_LONG_DGNL;
// 				/* Step C2: Turn on green(LD) LED, and turn off other LEDs */
// 				LED_arrayOff( PORT_A, DIO_MASK_BITS_1_2_3 );
// 				LED_on( PORT_A, APP_MOVE_FWD_LD_LED );
// 				/* Step C3: Car moves for 3 sec. with 50% of speed */
// 				TIMER_timer2Delay( APP_FWD_LD_DURATION );
// 				DCM_setDutyCycleOfPWM( APP_FWD_LD_DUTY );
// 				DCM_stopDCM();
// 			
// 				/* Check 1.2: appMode is not "CAR_STOP" mode */
// 				if ( u8_gs_appMode != APP_CAR_STOP )
// 				{
// 					/* Step C4: Update appMode to "CAR_ROT_90_DEG" mode */
// 					u8_gs_appMode = APP_CAR_ROT_90_DEG;
// 				}
// 				break;
// 			
// 			case APP_CAR_ROT_90_DEG:
// 			
// 				/* Step D1: Turn on yellow LED, and turn off other LEDs */
// 				LED_arrayOff( PORT_A, DIO_MASK_BITS_0_1_3 );
// 				LED_on( PORT_A, APP_ROTATE_LED );
// 				/* Step D2: Delay 0.5 sec. */
// 				TIMER_timer0Delay( APP_ROTATION_DELAY );
// 				/* Step D3: Car rotates for 620 msec. with 50% of speed */
// 				TIMER_timer2Delay( APP_ROTATION_DURATION );
// 				DCM_rotateDCM();
// 				DCM_stopDCM();
// 				/* Step D4: Delay 0.5 sec. */
// 				TIMER_timer0Delay( APP_ROTATION_DELAY );
// 			
// 				/* Check 1.3: appMode is not "CAR_STOP" mode */
// 				if ( u8_gs_appMode != APP_CAR_STOP )
// 				{
// 					/* Step D5: Update appMode to "CAR_MOVE_FWD_SD" or "CAR_MOVE_FWD_LD" modes */
// 					/* Check 1.3.1: Required diagonalFlag  */
// 					switch ( u8_gs_diagonalFlag )
// 					{
// 						case APP_SHORT_DGNL:
// 						u8_gs_appMode = APP_CAR_MOVE_FWD_LD;
// 						break;
// 						case APP_LONG_DGNL :
// 						u8_gs_appMode = APP_CAR_MOVE_FWD_SD;
// 						break;
// 						default:
// 						u8_gs_diagonalFlag = APP_LONG_DGNL;
// 						u8_gs_appMode = APP_CAR_MOVE_FWD_LD;
// 					}
// 				}
// 				break;
// 				
// 			case APP_CAR_MOVE_FWD_SD:
// 				
// 				/* Step E1: Update diagonalFlag to "SHORT_DGNL" */
// 				u8_gs_diagonalFlag = APP_SHORT_DGNL;
// 				/* Step E2: Turn on green(SD) LED, and turn off other LEDs */
// 				LED_arrayOff( PORT_A, DIO_MASK_BITS_0_2_3 );
// 				LED_on( PORT_A, APP_MOVE_FWD_SD_LED );
// 				/* Step E3: Car moves for 2 sec. with 30% of speed */
// 				TIMER_timer2Delay( APP_FWD_SD_DURATION );
// 				DCM_setDutyCycleOfPWM( APP_FWD_SD_DUTY );
// 				
// 				/* Check 1.4: appMode is not "CAR_STOP" mode */
// 				if ( u8_gs_appMode != APP_CAR_STOP )
// 				{
// 				/* Step E4: Update appMode to "CAR_ROT_90_DEG" mode */
// 				u8_gs_appMode = APP_CAR_ROT_90_DEG;
// 				}
// 				break;
// 				
// 			default:
// 				u8_gs_appMode = APP_CAR_STOP;
// 				u8_gs_diagonalFlag = APP_LONG_DGNL;
// 				break;
/*		}*/
	}
}

/*******************************************************************************************************************************************************************/
/*
Name:
Input:
Output:
Description: Function to .
*/
// void APP_startCar( void )
// {
//     if(u8_gs_appMode != APP_CAR_STOP) return;
// 	/* Update appMode to "CAR_START" mode */
//     u8_gs_diagonalFlag = APP_LONG_DGNL;
//     u8_g_suddenBreakFlag = APP_BREAK_FLAG_DOWN;
//     u8_gs_appMode = APP_CAR_START;
// }

/*******************************************************************************************************************************************************************/
/*
Name:
Input:
Output:
Description: Function to .
*/
// void APP_stopCar( void )
// {
// 	/* Update appMode to "CAR_STOP" mode */
//     u8_g_suddenBreakFlag = APP_BREAK_FLAG_UP;
//     u8_gs_appMode = APP_CAR_STOP;
// }

/*******************************************************************************************************************************************************************/