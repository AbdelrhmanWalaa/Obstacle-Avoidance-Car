/*
 * TMR0_interface.h
 *
 * Created: 2023-05-16 6:56:32 PM
 *  Author: HAZEM-PC
 */ 


#ifndef TMR0_INTERFACE_H_
#define TMR0_INTERFACE_H_

/*============= FILE INCLUSION =============*/
#include "../UTILITIES/STD_TYPES.h"
#include "../TMR_UTILITIES/TMR_UTILITIES.h"
#include "../TMR_UTILITIES/TMR_Registers.h"

/*============= extern variables =============*/
extern volatile u8 g_timeout_flag;


/*============= MACRO DEFINITION =============*/
#define MAX_DELAY_COMP_MODE 40  //evaluate to 40*250ms=10 seconds

/*============= FUNCTION PROTOTYPE =============*/

/*
Description
use to apply block delay using overflow mode 
*/
void TMR0_Delay_MS(f32 delay);

/*
Description
use to stop timer functionality
*/
void TMR0_Stop(void);


/*
Description
use to call event after elapse timeout delay using overflow mode
minimum delay is (1 ms)
*/
void TMR0_CallEvent(f32 delay,void(*g_ptr)(void));

/*
Description
use to apply timeout function using compare match mode 
maximum timeout is 10.2 second
to configure maximum timeout redefine MAX_DELAY_COMP_MODE 
for example:
assume OCR register (0--255) 
max_timeout = MAX_DELAY_COMP_MODE * OCR
*/
void TMR0_TimeOut_Ms(f32 delay); 

//void TMR0_OVFSetCallBack (void(*g_ptr)(void));
//void TMR0_CMPSetCallBack (void(*g_ptr)(void));

#endif /* TMR0_INTERFACE_H_ */