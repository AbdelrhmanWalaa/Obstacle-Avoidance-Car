/*
 * dcm_config.h
 *
 *   Created on: Apr 11, 2023
 *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *  Description: This file contains all Direct Current Motor (DCM) pre-build configurations, through which user can configure before using the DCM peripheral.
 *    Datasheet: https://datasheetspdf.com/pdf/917207/KYSAN/RF-300CA-11440/1
 *    Model No.: RF-300CA-11440 - DCM
 */

#ifndef DCM_CONFIG_H_
#define DCM_CONFIG_H_

/*******************************************************************************************************************************************************************/
/* DCM Configurations */
 
/* DCM Port(s) */
/* Options: A:0
			B:1
			C:2
			D:3
 */
/* Right DCM */
#define	DCM_U8_DCM_R_DIR_PORT		3
#define	DCM_U8_DCM_R_PWM_PORT		3

/* Left DCM  */
#define	DCM_U8_DCM_L_DIR_PORT		3
#define	DCM_U8_DCM_L_PWM_PORT		3

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
#define DCM_U8_DCM_R_DIR_PIN_1A		P2
#define	DCM_U8_DCM_R_DIR_PIN_1B		P3
#define	DCM_U8_DCM_R_PWM_PIN		P4

/* Left DCM */
#define DCM_U8_DCM_L_DIR_PIN_2A		P6
#define	DCM_U8_DCM_L_DIR_PIN_2B		P7
#define	DCM_U8_DCM_L_PWM_PIN		P5

/* End of Configurations */

/*******************************************************************************************************************************************************************/

#endif /* DCM_CONFIG_H_ */