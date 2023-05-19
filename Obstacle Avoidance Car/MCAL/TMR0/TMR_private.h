/*
 * TMR_private.h
 *
 * Created: 2023-05-16 6:57:41 PM
 *  Author: HAZEM-PC
 */ 


#ifndef TMR_PRIVATE_H_
#define TMR_PRIVATE_H_

/*============= MACRO DEFINITION =============*/
//timer common macros
#define MAX_COUNT 256
#define MIN_COUNT 1
#define INIT_VALUE(T_max,T_delay,tick)  (((f32)T_max-T_delay)/tick)
#define MAX_DELAY_MS(prescaler) ((((f32)prescaler/F_CPU)*MAX_COUNT)*1000UL) 
#define MIN_DELAY_MS(prescaler) ((((f32)prescaler/F_CPU)*MIN_COUNT)*1000UL) 
#define MAX_DELAY_US(prescaler) ((((f32)prescaler/F_CPU)*MAX_COUNT)*1000000UL) 
#define MIN_DELAY_US(prescaler) ((((f32)prescaler/F_CPU)*MIN_COUNT)*1000000UL)


//pre_scaler values for TIMER0
#define P_0    0
#define P_1    1
#define P_8    8
#define P_64   64
#define P_256  256
#define P_1024 1024

/*======timer registers ====== */
//timer 0 registers
#define TCCR0	(*((volatile u8*)0x53))
#define TCNT0	(*((volatile u8*)0x52))
#define OCR0	(*((volatile u8*)0x5C))
#define TIMSK	(*((volatile u8*)0x59))	//Timer/Counter Interrupt Mask Register – TIMSK
#define TIFR	(*((volatile u8*)0x58))	//Timer/Counter Interrupt Flag Register – TIFR
//timer 1 registers
#define TCCR1A	(*((volatile u8*)0x4F))
#define TCCR1B	(*((volatile u8*)0x4E))
#define TCNT1H	(*((volatile u8*)0x4D))
#define TCNT1L	(*((volatile u8*)0x4C))
#define OCR1AH	(*((volatile u8*)0x4B))
#define OCR1AL	(*((volatile u8*)0x4A))
#define OCR1BH	(*((volatile u8*)0x49))
#define OCR1BL	(*((volatile u8*)0x48))
#define ICR1H	(*((volatile u8*)0x47))
#define ICR1L	(*((volatile u8*)0x46))
//timer 2 registers
#define TCCR2	(*((volatile u8*)0x45))
#define TCNT2	(*((volatile u8*)0x44))
#define OCR2	(*((volatile u8*)0x43))
#define ASSR	(*((volatile u8*)0x42))
#define SFIOR	(*((volatile u8*)0x50))

/*======timer0 pin names ====== */
//TCCR0 timer counter control register
#define CS00  0
#define CS01  1
#define CS02  2
#define WGM01 3
#define COM00 4
#define COM01 5
#define WGM00 6
#define FOC0  7
//TIMSK interrupt mask register
#define TOIE0  0
#define OCIE0  1
#define TOIE1  2
#define OCIE1B 3
#define OCIE1A 4
#define TICIE1 5
#define TOIE2  6
#define OCIE2  7
//TIFR interrupt flag register
#define TOV0  0
#define OCF0  1
#define TOV1  2
#define OCF1B 3
#define OCF1A 4
#define ICF1  5
#define TOV2  6
#define OCF2  7

/*======timer1 pin names ====== */
//Timer/Counter1 Control Register A – TCCR1A
#define WGM10	0
#define WGM11	1
#define FOC1B	2
#define FOC1A	3
#define COM1B0	4
#define COM1B1	5
#define COM1A0	6
#define COM1A1	7
//Timer/Counter1 Control Register B – TCCR1B
#define CS10	0
#define CS11	1
#define CS12	2
#define WGM12	3
#define WGM13	4
#define ICES1	6
#define ICNC1	7

/*======timer2 pin names ====== */
//Timer/Counter Control Register – TCCR2
#define CS20	0
#define CS21	1
#define CS22	2
#define WGM21	3
#define COM20	4
#define COM21	5
#define WGM20	6
#define FOC2	7
//Asynchronous Status Register – ASSR
#define TCR2UB	0
#define OCR2UB	1
#define TCN2UB	2
#define AS2		3

//timer ISR definition
#define TMR2_CMP	__vector_4
#define TMR2_OVF	__vector_5
#define TMR1_CPT	__vector_6
#define TMR1_CMP_A	__vector_7
#define TMR1_CMP_B	__vector_8
#define TMR1_OVF	__vector_9
#define TMR0_CMP	__vector_10
#define TMR0_OVF	__vector_11

//Macro defines the ISR
#define ISR_HANDLER(INT_VECT) void INT_VECT(void) __attribute__ ((signal,used));\
void INT_VECT(void)

#endif /* TMR_PRIVATE_H_ */