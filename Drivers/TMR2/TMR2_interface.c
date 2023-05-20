/*
 * TMR2_interface.c
 *
 * Created: 2023-05-17 10:52:03 AM
 *  Author: HAZEM-PC
 */ 

/*============= FILE INCLUSION =============*/
#include "TMR2_interface.h"
#include "../DIO/dio_interface.h"

/*============= global variables =============*/
u8 g_port,g_pin,g_PWM_flag=0;
u8 g_Ton_initValue,g_Toff_initValue;
u16 g_Ton_prescale,g_Toff_prescale;
f32 g_period_time;
/*============= TYPE DEFINITION =============*/


/*============= PRIVATE FUNCTIONS =============*/
static u8 calc_prescaler(f32 delay,u16* prescale);
static void calc_initialValue(u16 prescaler,u8* initial_value,f32 delay);
static void set_prescale(u16 prescaler);

/*============= FUNCTION DEFINITIONS =============*/

//pwm_frequency:frequency of PWM in KHZ
void TMR2_PWM_Init(f32 pwm_frequency,u8 port_ID,u8 pin_num)
{
	g_port=port_ID;
	g_pin=pin_num;
	//GPIO_setPinDirection(port_ID,pin_num,PIN_OUTPUT);
	DIO_init(port_ID,pin_num,OUT); 
	//GPIO_writePin(port_ID,pin_num,Low);
	DIO_write(port_ID,pin_num,LOW);
	g_period_time=1.0F/pwm_frequency;							//PWM periodic time in milli_seconds
	TIMSK |= (1<<TOIE2);										// Enable Timer2 Overflow Interrupt
}

void TMR2_PWM_start(u8 duty_cycle)
{
	f32 time_on,time_off;
	time_on=((f32)g_period_time*duty_cycle)/100.0F;					//calculate on time delay
	time_off=g_period_time-time_on;									//calculate off time delay
	calc_prescaler(time_on,&g_Ton_prescale);						//calculate prescaler value of on time
	calc_prescaler(time_off,&g_Toff_prescale);						//calculate prescaler value of off time
	calc_initialValue(g_Ton_prescale,&g_Ton_initValue,time_on);		//calculate initial value of on time
	calc_initialValue(g_Toff_prescale,&g_Toff_initValue,time_off);	//calculate initial value of off time
	TCNT2=g_Ton_initValue;											//set timer initial value
	set_prescale(g_Ton_prescale);									//set timer prescaler value
}

ISR_HANDLER(TMR2_OVF)
{
	if(g_PWM_flag==0)
	{
		set_prescale(g_Toff_prescale);
		TCNT2=g_Toff_initValue;
		g_PWM_flag=1;
	}
	else
	{
		set_prescale(g_Ton_prescale);
		TCNT2=g_Ton_initValue;
		g_PWM_flag=0;
	}
	//GPIO_togglePin(g_port, g_pin);
	DIO_toggle(g_port,g_pin);
}



static u8 calc_prescaler(f32 delay,u16* prescaler)
{
	if(delay <= MAX_DELAY_MS(P_1024))
	{
		if(delay <= MAX_DELAY_MS(P_1))
		*prescaler=P_1;
		else if(delay <= MAX_DELAY_MS(P_8))
		*prescaler=P_8;
		else if(delay <= MAX_DELAY_MS(P_32))
		*prescaler=P_32;
		else if(delay <= MAX_DELAY_MS(P_64))
		*prescaler=P_64;
		else if(delay <= MAX_DELAY_MS(P_128))
		*prescaler=P_128;
		else if(delay <= MAX_DELAY_MS(P_256))
		*prescaler=P_256;
		else if(delay <= MAX_DELAY_MS(P_1024))
		*prescaler=P_1024;
		else
		*prescaler = P_0;
		return TRUE;
	}
	else
	return FALSE;
}

static void calc_initialValue(u16 prescaler,u8* init_value,f32 delay)
{
	switch(prescaler)
	{
		case P_1:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_1),delay,MIN_DELAY_MS(P_1));
		break;
		case P_8:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_8),delay,MIN_DELAY_MS(P_8));
		break;
		case P_32:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_32),delay,MIN_DELAY_MS(P_32));
		break;
		case P_64:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_64),delay,MIN_DELAY_MS(P_64));
		break;
		case P_128:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_128),delay,MIN_DELAY_MS(P_128));
		break;
		case P_256:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_256),delay,MIN_DELAY_MS(P_256));
		break;
		case P_1024:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_1024),delay,MIN_DELAY_MS(P_1024));
		break;
		default:
		*init_value=FALSE;
		break;
	}
}

static void set_prescale(u16 prescaler)
{
	switch(prescaler)
	{
		case P_1:
		TCCR2 = (1<<FOC2) | (1<<CS20);
		break;
		case P_8:
		TCCR2 = (1<<FOC2) | (1<<CS21);
		break;
		case P_32:
		TCCR2 = (1<<FOC2) | (1<<CS20) | (1<<CS21);
		break;
		case P_64:
		TCCR2 = (1<<FOC2) | (1<<CS22);
		break;
		case P_128:
		TCCR2 = (1<<FOC2) | (1<<CS20) | (1<<CS22);
		break;
		case P_256:
		TCCR2 = (1<<FOC2) | (1<<CS21) | (1<<CS22);
		break;
		case P_1024:
		TCCR2 = (1<<FOC2) | (1<<CS20) | (1<<CS21)| (1<<CS22);
		break;
		default:
		TCCR2 = FALSE;
		break;
	}
}