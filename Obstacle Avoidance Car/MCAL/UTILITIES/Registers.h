/*
 * Registers.h
 *
 *  Created on: Apr 4, 2023
 *      Author: HAZEM-PC
 */

#ifndef UTILITIES_REGISTERS_H_
#define UTILITIES_REGISTERS_H_

#include "STD_TYPES.h"

/*============= MACRO DEFINITION =============*/
#define SREG  (*((volatile u8*)0x5F))

/*============= DIO registers =============*/
#define PORTA (*((volatile u8*)0x3B))
#define DDRA  (*((volatile u8*)0x3A))
#define PINA  (*((volatile u8*)0x39))
#define PORTB (*((volatile u8*)0x38))
#define DDRB  (*((volatile u8*)0x37))
#define PINB  (*((volatile u8*)0x36))
#define PORTC (*((volatile u8*)0x35))
#define DDRC  (*((volatile u8*)0x34))
#define PINC  (*((volatile u8*)0x33))
#define PORTD (*((volatile u8*)0x32))
#define DDRD  (*((volatile u8*)0x31))
#define PIND  (*((volatile u8*)0x30))

#define PORTA_ID 	0
#define PORTB_ID 	1
#define PORTC_ID 	2
#define PORTD_ID 	3
#define MAX_PORT_ID 4

#define PIN0 	0
#define PIN1 	1
#define PIN2 	2
#define PIN3 	3
#define PIN4 	4
#define PIN5 	5
#define PIN6 	6
#define PIN7 	7
#define MAX_PIN 8

/*==========ADC registers===========*/
#define ADMUX  (*((volatile u8*)0x27))
#define ADCSRA (*((volatile u8*)0x26))
#define ADCH   (*((volatile u8*)0x25))
#define ADCL   (*((volatile u8*)0x24))
#define ADC    (*((volatile uint16*)0x24))
#define SFIOR  (*((volatile u8*)0x50))

//ADMUX register pins
#define MUX0 	0
#define MUX1	1
#define MUX2 	2
#define MUX3 	3
#define MUX4 	4
#define ADALR 	5
#define REFS0 	6
#define REFS1 	7
//ADCSRA register pins
#define ADPS0 	0
#define ADPS1 	1
#define ADPS2 	2
#define ADIE  	3
#define ADIF    4
#define ADATE 	5
#define ADSC  	6
#define ADEN 	7

//SFIRO register auto trigger source pins
#define ADTS0 	5
#define ADTS1 	6
#define ADTS2	7

/*============= TIMER0 registers =============*/
/*============= TIMER1 registers =============*/
/*============= TIMER2 registers =============*/



/*============= EXI registers =============*/
#define MCUCR	*((volatile u8*) 0x55)
#define MCUCSR	*((volatile u8*) 0x54)
#define GICR	*((volatile u8*) 0x5B)
#define GIFR	*((volatile u8*) 0x5A)

//MCUCR control register
#define ISC00   0
#define ISC01   1
#define ISC10   2
#define ISC11   3
#define SM0     4
#define SM1     5
#define SM2     6
#define SE      7
//MCUCSR interrupt flag register
#define ISC2    6
#define PORF    0
#define EXTRF   1
#define BORF    2
#define WDRF    3
#define JTRF    4
#define JTD     7
//GIFR general interrupt flag register
#define INTF2   5
#define INTF0   6
#define INTF1   7
//GICR general interrupt control register
#define IVCE    0
#define IVSEL   1
#define INT2    5
#define INT0    6
#define INT1    7

//timer ISR definition
#define EXI_INT0	__vector_1
#define EXI_INT1	__vector_2
#define EXI_INT2	__vector_3
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

/*============= UART registers =============*/

/*============= SPI registers =============*/

/*============= TWI registers =============*/

#endif /* UTILITIES_REGISTERS_H_ */
