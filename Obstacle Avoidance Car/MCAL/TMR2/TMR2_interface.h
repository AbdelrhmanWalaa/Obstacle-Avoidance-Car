/*
 * TMR2_interface.h
 *
 * Created: 2023-05-17 10:52:22 AM
 *  Author: HAZEM-PC
 */ 


#ifndef TMR2_INTERFACE_H_
#define TMR2_INTERFACE_H_

/*============= FILE INCLUSION =============*/
#include "../UTILITIES/STD_TYPES.h"
#include "../TMR_UTILITIES/TMR_UTILITIES.h"
#include "../TMR_UTILITIES/TMR_Registers.h"
/*============= extern variables =============*/


/*============= MACRO DEFINITION =============*/


/*============= TYPE DEFINITION =============*/


/*============= FUNCTION PROTOTYPE =============*/
void TMR2_PWM_Init(f32 pwm_frequency,u8 port_ID,u8 pin_num);
void TMR2_PWM_start(u8 duty_cycle);


#endif /* TMR2_INTERFACE_H_ */