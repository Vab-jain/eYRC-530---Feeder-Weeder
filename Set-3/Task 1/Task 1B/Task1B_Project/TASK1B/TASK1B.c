/*
 * TASK1B.c
 *
 * Created: 04/11/2017 22:58:28
 *  Author: Avinash Dubey
 */ 


#include <avr/io.h>
#define F_CPU 14745600
#include <util/delay.h>
#include "buzzer.h"

void ReachDestinationAvoidingNode(unsigned char Xd,unsigned char Yd,unsigned char Xn,unsigned char Yn)
{
	//TODO:: Please write your application code. You can make your own functions & header files
}


//Do not make changes in main function

int main(void)
{
       ReachDestinationAvoidingNode(5,'D',6,'D');
	   buzzer_on();
	   _delay_ms(500);
	   buzzer_off();
	   ReachDestinationAvoidingNode(2,'F',2,'D');
	   buzzer_on();
	   _delay_ms(500);
	   buzzer_off();
	   ReachDestinationAvoidingNode(2,'A',2,'C');
	   buzzer_on();
	   
}