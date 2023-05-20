/*
 * TMR0_interface.c
 *
 * Created: 2023-05-16 6:54:02 PM
 *  Author: HAZEM-PC
 */ 

/*============= FILE INCLUSION =============*/
#include "TMR0_interface.h"

/*============= global variables =============*/
static volatile void (*g_callBackPtr_0)(void) = NULL;
static volatile u8 g_overFlows=0;
static volatile u8 g_tick=0;
static u8 g_initialValue=0;
volatile u8 g_timeoutFlag=0;

/*============= TYPE DEFINITION =============*/
typedef struct{
	u16 prescaler;
	u8 initial_value;
	f32 NO_OF_OV;
}ST_timer0_config;

/*============= PRIVATE FUNCTIONS =============*/
static inline u8 calc_prescaler(f32 delay,u16* prescale);
static inline void calc_initialValue(u16 prescaler,u8* initial_value,f32 delay);
static inline void set_prescale(u16 prescaler);

/*============= FUNCTION DEFINITIONS =============*/
/*
 * Description:used to calculate timer settings
 * calculate pre_scale value
 * calculate number of overflows
 * calculate timer initial value
 */

void TMR0_DelayMS(f32 delay)
{
	ST_timer0_config st_time;
	u8 real_part,reminder,count;
	if(calc_prescaler(delay, &st_time.prescaler))
	{
		calc_initialValue(st_time.prescaler, &st_time.initial_value,delay);
		st_time.NO_OF_OV=0;
	}
	else
	{
		st_time.prescaler=P_1024;
		st_time.NO_OF_OV=(delay/MAX_DELAY_MS(P_1024));
		st_time.initial_value=0;
	}
	real_part=st_time.NO_OF_OV;
	reminder=(st_time.NO_OF_OV-real_part)*(MAX_DELAY_MS(P_1024)); //reminder delay to complete required delay period
	count=real_part;
	TCNT0=st_time.initial_value;
	if(count==0)
	{
		set_prescale(st_time.prescaler);
		while(!(TIFR & (1<<TOV0)));
	}
	else
	{
		TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);		//use clock pre_scale 1024
		while(count != 0)								//run delay of real part
		{
			while(!(TIFR & (1<<TOV0)));
			count--;
			TIFR |= (1<<TOV0);
		}
		if(reminder > 0)								//run remaining delay period
		{
			if(reminder <= MAX_DELAY_MS(P_1))						//find best range that contain the required remaining delay
				st_time.prescaler=P_1;
			else if(reminder <= MAX_DELAY_MS(P_8))
				st_time.prescaler=P_8;
			else if(reminder <= MAX_DELAY_MS(P_64))
				st_time.prescaler=P_64;
			else if(reminder <= MAX_DELAY_MS(P_256))
				st_time.prescaler=P_256;
			else if(reminder <= MAX_DELAY_MS(P_1024))
				st_time.prescaler=P_1024;
			else
				st_time.prescaler = P_0;
		}
		switch(st_time.prescaler)												//according to selected pre_scaler value
		{
		case P_1:
			TCNT0=INIT_VALUE(MAX_DELAY_MS(P_1),reminder,MIN_DELAY_MS(P_1));	//set TCNT0 register with the initial value
			TCCR0 = (1<<FOC0) | (1<<CS00);									//set TCCR0 register with clock source
			break;
		case P_8:
			TCNT0=INIT_VALUE(MAX_DELAY_MS(P_8),reminder,MIN_DELAY_MS(P_8));
			TCCR0 = (1<<FOC0) | (1<<CS01);
			break;
		case P_64:
			TCNT0=INIT_VALUE(MAX_DELAY_MS(P_64),reminder,MIN_DELAY_MS(P_64));
			TCCR0 = (1<<FOC0) | (1<<CS01) | (1<<CS00);
			break;
		case P_256:
			TCNT0=INIT_VALUE(MAX_DELAY_MS(P_256),reminder,MIN_DELAY_MS(P_256));
			TCCR0 = (1<<FOC0) | (1<<CS02);
			break;
		case P_1024:
			TCNT0=INIT_VALUE(MAX_DELAY_MS(P_1024),reminder,MIN_DELAY_MS(P_1024));
			TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
			break;
		default:
			TCNT0=0;
			TCCR0 = 0;
			break;
		}
		while(!(TIFR & (1<<TOV0)));			//Wait until the Timer0 Overflow occurs (wait until TOV0 = 1)
	}
	TIFR |= (1<<TOV0);						//Clear TOV0 bit by set its value
	TCCR0 = FALSE;
}

void TMR0_Stop(void)
{
	TIMSK &= ~(1<<TOIE0);
}

void TMR0_CallEvent(f32 delay,void(*g_ptr)(void))
{	
	TIMSK |= (1<<TOIE0);
	if(delay < MAX_DELAY_MS(P_1024))
	{
		g_initialValue=ceil(((f32)(MAX_DELAY_MS(P_1024)-delay)/MIN_DELAY_MS(P_1024)));
		g_overFlows=0;
	}
	else
	{
		g_overFlows=delay/MAX_DELAY_MS(P_1024);
		g_initialValue=0;
	}
	TCNT0=g_initialValue;
	
	g_callBackPtr_0=g_ptr;
	TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
}

void TMR0_TimeOutMs(f32 delay)
{
	u8 divide_1=0;
	u32 divide_2=0;
	/*code to count time in milliseconds*/
		if(delay < MAX_DELAY_MS(P_1024))
		{
			g_initialValue=delay;
			g_overFlows=0;
		}
		else
		{
			for(u8 i=1;i< MAX_DELAY_COMP_MODE;i++)
			{
				divide_1=(f32)delay/i;
				divide_2=(f32)delay/i;
				if(divide_1 < MAX_COUNT)
				{
					if(divide_1==divide_2)
					{
						g_initialValue=divide_1;
						g_overFlows=i;
						break;
					}
				}
			}
		}
		TCNT0=0;
		OCR0=g_initialValue;
		TIMSK |= (1<<OCIE0);			//enable timer compare match interrupt
		g_timeoutFlag=0;				//set flag to default
		TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS02) | (1<<CS00);
}

ISR_HANDLER(TMR0_CMP)
{
	if(g_overFlows > 0)
	{
		g_tick++;
		if(g_tick==g_overFlows)
		{
			g_timeoutFlag++;
			g_tick = 0;			//clear the tick counter again to start new count				
		}
	}
	else
	{
		g_timeoutFlag++;
		TCNT0=0;
		OCR0=g_initialValue;
	}
}

ISR_HANDLER(TMR0_OVF)
{
	if(g_callBackPtr_0 != NULL)
	{
		if(g_overFlows > 0)
		{
			g_tick++;
			if(g_tick == g_overFlows)
			{
				g_callBackPtr_0();
				g_tick = 0; //clear the tick counter again to start new count
			}
		}
		else
		{
			g_callBackPtr_0();
			TCNT0=g_initialValue;
		}
	}
}

static inline u8 calc_prescaler(f32 delay,u16* prescale)
{
	if(delay <= MAX_DELAY_MS(P_1024))
	{
		if(delay <= MAX_DELAY_MS(P_1))
			*prescale=P_1;
		else if(delay <= MAX_DELAY_MS(P_8))
			*prescale=P_8;
		else if(delay <= MAX_DELAY_MS(P_64))
			*prescale=P_64;
		else if(delay <= MAX_DELAY_MS(P_256))
			*prescale=P_256;
		else if(delay <= MAX_DELAY_MS(P_1024))
			*prescale=P_1024;
		else
			*prescale = P_0;
		return TRUE;
	}
	else
		return FALSE;
}

static inline void calc_initialValue(u16 prescaler,u8* init_value,f32 delay)
{
	switch(prescaler)
	{
	case P_1:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_1),delay,MIN_DELAY_MS(P_1));
		break;
	case P_8:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_8),delay,MIN_DELAY_MS(P_8));
		break;
	case P_64:
		*init_value=INIT_VALUE(MAX_DELAY_MS(P_64),delay,MIN_DELAY_MS(P_64));
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

static inline void set_prescale(u16 prescaler)
{
	switch(prescaler)
	{
	case P_1:
		TCCR0 = (1<<FOC0) | (1<<CS00);
		break;
	case P_8:
		TCCR0 = (1<<FOC0) | (1<<CS01);
		break;
	case P_64:
		TCCR0 = (1<<FOC0) | (1<<CS01) | (1<<CS00);
		break;
	case P_256:
		TCCR0 = (1<<FOC0) | (1<<CS02);
		break;
	case P_1024:
		TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
		break;
	default:
		TCCR0 = FALSE;
		break;
	}
}

