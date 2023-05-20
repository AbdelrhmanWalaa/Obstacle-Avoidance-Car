/*
 * ICU_interface.h
 *
 * Created: 2023-05-19 6:56:43 PM
 *  Author: HAZEM-PC
 */ 


#ifndef ICU_INTERFACE_H_
#define ICU_INTERFACE_H_

/*============= FILE INCLUSION =============*/
#include "../../MCAL/exi/exi_interface.h"
#include "../../MCAL/TMR1/TMR1_interface.h"

/*============= extern variables =============*/


/*============= MACRO DEFINITION =============*/


/*============= TYPE DEFINITION =============*/
typedef enum
{
	NO_CLOCK,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,MAX_F_CPU
}EN_ICU_Clock;

typedef enum
{
	FALLING=2,RISING
}EN_ICU_EdgeType;

typedef enum{
	EN_INT0,EN_INT1,EN_INT2
}EN_ICU_Source;

typedef struct
{
	EN_ICU_Clock clock;
	EN_ICU_EdgeType edge;
	EN_ICU_Source source;
}ST_ICU_ConfigType;

/*============= FUNCTION PROTOTYPE =============*/

/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the External Interrupt source and edge.
 * 	4. Initialize Timer1 Registers
 */
void Icu_init(const ST_ICU_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void Icu_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to set the required edge detection.
 */
void Icu_setEdgeDetectionType(const EN_ICU_EdgeType edgeType);

/*
 * Description: Function to get the Timer1 Value when the external interrupt is capture edge
 *             
 */
u16 Icu_getInputCaptureValue(void);

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void Icu_clearTimerValue(void);

/*
 * Description: Function to disable the Timer1 and External interrupt to stop the ICU Driver
 */
void Icu_DeInit(void);

#endif /* ICU_INTERFACE_H_ */