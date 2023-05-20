/*
 * app_config.h
 *
 *   Created on: May 16, 2023
 *       Author: Team 3 - https://github.com/AbdelrhmanWalaa/Obstacle-Avoidance-Car.git
 *  Description: This file contains all Application (APP) pre-build configurations, through which user can configure before using the APP.
 */ 

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/*******************************************************************************************************************************************************************/
/* APP Configurations */

// /* BTNs Config */
// #define APP_STOP_BTN			2
// #define APP_START_BTN			3
// 
// /* LEDs Config */
// #define APP_MOVE_FWD_LD_LED		0
// #define APP_MOVE_FWD_SD_LED		1
// #define APP_ROTATE_LED			2
// #define APP_STOP_LED			3
// 
// /* Delays/Timing Config */
// #define APP_STARTING_DELAY      1000
// #define APP_FWD_LD_DURATION     3000
// #define APP_FWD_LD_DUTY         50
// #define APP_ROTATION_DELAY      500
// #define APP_ROTATION_DURATION   620
// #define APP_FWD_SD_DURATION     2000
// #define APP_FWD_SD_DUTY         30
// 
// /* APP_BREAK_FLAG */
// #define APP_BREAK_FLAG_UP       1
// #define APP_BREAK_FLAG_DOWN     0

/* DCM Port(s) */
/* Options: A:0
			B:1
			C:2
			D:3
 */
/* Right DCM */
#define	APP_U8_DCM_R_DIR_CW_PORT	3
#define	APP_U8_DCM_R_DIR_CCW_PORT	3
#define	APP_U8_DCM_R_PWM_PORT		3

/* Left DCM  */
#define	APP_U8_DCM_L_DIR_CW_PORT	3
#define	APP_U8_DCM_L_DIR_CCW_PORT	3
#define	APP_U8_DCM_L_PWM_PORT		3

/* DCM Pins */
/* Options: P0
			P1
			P2
			P3
			P4
			P5
			P6
			P7
 */
/* Right DCM */
#define APP_U8_DCM_R_DIR_CW_PIN		2
#define	APP_U8_DCM_R_DIR_CCW_PIN	3
#define	APP_U8_DCM_R_PWM_PIN		4

/* Left DCM */
#define APP_U8_DCM_L_DIR_CW_PIN		6
#define	APP_U8_DCM_L_DIR_CCW_PIN	7
#define	APP_U8_DCM_L_PWM_PIN		5

/* End of Configurations */

/*******************************************************************************************************************************************************************/

#endif /* APP_CONFIG_H_ */