// /*
//  * tmr_interface.h
//  *
//  *     Created on: Jul 27, 2021
//  *         Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
//  *    Description: This file contains all Timers (TMR) functions' prototypes and definitions (Macros) to avoid magic numbers.
//  *  MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
//  */
// 
// #ifndef TMR_INTERFACE_H_
// #define TMR_INTERFACE_H_
// 
// /*******************************************************************************************************************************************************************/
// /* TMR Includes */
// 
// /* LIB */
// #include "../../LIB/std_types/std_types.h"
// #include "../../LIB/bit_math/bit_math.h"
// #include "../../LIB/mcu_config/mcu_config.h"
// 
// /*******************************************************************************************************************************************************************/
// /* TMR Macros */
// 
// /* The 3 Timers counted from 0 to 2 */
// #define TMR_U8_TMR0							0
// #define TMR_U8_TMR1 						1
// #define TMR_U8_TMR2 						2
// 
// /* The 3 Timers Channels counted from 0 to 2 */
// #define TMR_U8_NO_CHANNEL					0
// #define TMR_U8_TMR_1_CHANNEL_A				1
// #define TMR_U8_TMR_1_CHANNEL_B				2
// 
// /* The 3 Timers Interrupts counted from 0 to 2 */
// #define TMR_U8_CAPT_INTERRUPT				0
// #define TMR_U8_COMP_INTERRUPT				1
// #define TMR_U8_OVF_INTERRUPT				2
// 
// /* The 3 Timers Compare Match Output Modes counted from 0 to 2 */
// #define TMR_U8_TOG_OCR_PIN					0
// #define TMR_U8_CLR_OCR_PIN					1
// #define TMR_U8_SET_OCR_PIN					2
// 
// /*******************************************************************************************************************************************************************/
// /* TMR Functions' prototypes */
// 
// vd TMR_TMR0Initialization  ( void );
// vd TMR_TMR1Initialization  ( void );
// vd TMR_TMR2Initialization  ( void );
// 
// u8 TMR_enableTMR		   ( u8 u8_a_timerId );
// u8 TMR_disableTMR		   ( u8 u8_a_timerId );
// 
// u8 TMR_delayMS			   ( u8 u8_a_timerId, u32 u32_a_delayMS );
// u8 TMR_delayUS			   ( u8 u8_a_timerId, u32 u32_a_delayUS );
// 
// u8 TMR_enableTMRInterrupt  ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u8 u8_a_interruptType );
// u8 TMR_disableTMRInterrupt ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u8 u8_a_interruptType );
// 
// u8 TMR_enableCOMPPin	   ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u8 u8_a_COMPMode );
// u8 TMR_disableCOMPPin	   ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId );
// 
// u8 TMR_getNumberOfOVF	   ( u8 u8_a_timerId, u16 *pu16_a_returnedNumberOfOVF );
// u8 TMR_getOVFFlagStatus	   ( u8 u8_a_timerId, u8 *pu8_a_returnedFlagStatus );
// u8 TMR_clearOVFFlag		   ( u8 u8_a_timerId );
// 
// u8 TMR_OVFSetCallBack	   ( u8 u8_a_timerId, void ( *pf_a_OVFInterruptAction ) ( void ) );
// u8 TMR_COMPSetCallBack	   ( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, void ( *pf_a_COMPInterruptAction ) ( void ) );
// 
// u8 TMR_getCounterValue	   ( u8 u8_a_timerId, u16 *pu16_a_returnedCounterValue );
// 
// u8 TMR_setCompareMatchValue( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u16 u16_a_compareMatchValue );
// u8 TMR_getCompareMatchValue( u8 u8_a_timerId, u8 u8_a_timer1ChannelId, u16 *pu16_a_returnedCompareMatchValue );
// 
// vd TMR_setInputCaptureValue( u16 u16_a_inputCaptureValue );
// u8 TMR_getInputCaptureValue( u16 *pu16_a_returnedInputCaptureValue );
// 
// /*******************************************************************************************************************************************************************/
// 
// #endif /* TMR_INTERFACE_H_ */