/*
 * btn_interface.h
 *
 *   Created on: MAY 16, 2023
 *       Author: Mohamed Abdelsalam - https://github.com/m3adel
 *  Description: This file contains all Button (BTN) functions' prototypes and definitions (Macros) to avoid magic numbers.
 */

#ifndef BTN_INTERFACE_H_
#define BTN_INTERFACE_H_

/*******************************************************************************************************************************************************************/
/* BTN Includes */

/* LIB */
#include "../../LIB/std_types/std_types.h"
#include "../../LIB/bit_math/bit_math.h"

/* MCAL */
#include "../../MCAL/dio/dio_interface.h"

/*******************************************************************************************************************************************************************/
/* BTN Functions' Prototypes */
/*******************************************************************************************************************************************************************/
void BUTTON_init( dio_Port_number_en PortNumber,dio_Pin_number_en PinNumber );
void BUTTON_read( dio_Port_number_en PortNumber,dio_Pin_number_en PinNumber,u8 *state );
/*******************************************************************************************************************************************************************/

#endif /* BTN_INTERFACE_H_ */