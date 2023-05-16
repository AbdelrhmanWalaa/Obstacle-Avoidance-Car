/*
 * app_interface.h
 *
 *   Created on: May 16, 2023
 *       Author: Team 3 - https://github.com/AbdelrhmanWalaa/Obstacle-Avoidance-Car.git
 *  Description: This file contains all Application (APP) functions' prototypes and definitions (Macros) to avoid magic numbers.
 */ 

#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

/*******************************************************************************************************************************************************************/
/* APP Includes */

/* LIB */
#include "../LIB/std_types/std_types.h"
#include "../LIB/bit_math/bit_math.h"

/* MCAL */
#include "../MCAL/dio/dio_interface.h"
#include "../MCAL/gli/gli_interface.h"
#include "../MCAL/exi/exi_interface.h"
#include "../MCAL/tmr/tmr_interface.h"

/* HAL */
#include "../HAL/btn/btn_interface.h"
#include "../HAL/lcd/lcd_interface.h"
#include "../HAL/kpd/kpd_interface.h"
#include "../HAL/dcm/dcm_interface.h"
#include "../HAL/usi/usi_interface.h"

/*******************************************************************************************************************************************************************/
/* APP Macros */

// /* APP Modes */
// #define APP_CAR_STOP			0
// #define APP_CAR_START			1
// #define APP_CAR_MOVE_FWD_LD		2
// #define APP_CAR_ROT_90_DEG		3
// #define APP_CAR_MOVE_FWD_SD		4
// 
// /* APP Diagonals Values */
// #define APP_SHORT_DGNL			0
// #define APP_LONG_DGNL			1

/*******************************************************************************************************************************************************************/
/* APP Functions' Prototypes */

vd APP_initialization( void );
vd APP_startProgram  ( void );
vd APP_startCar	     ( void );
vd APP_stopCar	     ( void );

/*******************************************************************************************************************************************************************/

#endif /* APP_INTERFACE_H_ */