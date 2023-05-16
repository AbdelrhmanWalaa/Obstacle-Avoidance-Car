/*
 * kpd_interface.h
 *
 *   Created on: Apr 3, 2021
 *       Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *  Description: This file contains all Keypad (KPD) functions' prototypes and definitions (Macros) to avoid magic numbers.
 */
 
#ifndef KPD_INTERFACE_H_
#define KPD_INTERFACE_H_

/*******************************************************************************************************************************************************************/
/* KPD Includes */

/* LIB */
#include "../../LIB/std_types/std_types.h"
#include "../../LIB/bit_math/bit_math.h"
#include "../../LIB/mcu_config/mcu_config.h"

/* MCAL */
#include "../../MCAL/dio/dio_Interface.h"

/*******************************************************************************************************************************************************************/
/* KPD Macros */

/* KPD Flag Values */
#define KPD_U8_KEY_NOT_FOUND		0
#define KPD_U8_KEY_FOUND			1

/* KPD Initial Value of Key */
#define KPD_U8_KEY_NOT_PRESSED		0xff

/*******************************************************************************************************************************************************************/
/* KPD Functions' Prototypes */

vd KPD_enableKPD    ( void );
vd KPD_disableKPD   ( void );

u8 KPD_getPressedKey( u8 *pu8_a_returnedKeyValue );

/*******************************************************************************************************************************************************************/

#endif /* KPD_INTERFACE_H_ */