// /*
//  * TMR2_interface.h
//  *
//  * Created: 2023-05-17 10:52:22 AM
//  *  Author: HAZEM-PC
//  */ 
// 
// 
// #ifndef TMR2_INTERFACE_H_
// #define TMR2_INTERFACE_H_
// 
// /*============= FILE INCLUSION =============*/
// #include "../../LIB/std_types/std_types.h"
// #include "../TMR_UTILITIES/TMR_UTILITIES.h"
// #include "../TMR_UTILITIES/TMR_Registers.h"
// /*============= extern variables =============*/
// 
// 
// /*============= MACRO DEFINITION =============*/
// 
// 
// /*============= TYPE DEFINITION =============*/
// 
// 
// /*============= FUNCTION PROTOTYPE =============*/
// 
// /*
// Description:Used to initialize PWM frequency and pin to be used with PWM
// pwm_frequency:frequency of PWM in KHZ
// port_ID: port name
// pin_num: pin name
// */
// void TMR2_PWM_Init(f32 pwm_frequency,u8 port_ID,u8 pin_num);
// 
// /*
// Description:Used to start generate PWM with the required duty cycle
// duty cycle percentage 0--100
// */
// 
// void TMR2_PWM_start(u8 duty_cycle);
// 
// 
// #endif /* TMR2_INTERFACE_H_ */