/*buzzer.c

*/
 
#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h>
#include "buzzer.h"

/*Function Name: buzzer_pin_config
Input: None
Output: Function to initialize buzzer
Logic: Setting PORTC3 as output which is the pin connected to the buzzer
Example Call: buzzer_pin_config()
*/
void buzzer_pin_config(void)
{
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

/*Function Name:buzzer_on
Input: None
Output: Buzzer turns on
Logic: Turns the buzzer on by writing 1 to PC3
Example Call: buzzer_on ()
*/
void buzzer_on(void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

/*Function Name:buzzer_off
Input: None
Output: Buzzer turns off
Logic: Turns the buzzer off by writing 0 to PC3
Example Call: buzzer_off()
*/
void buzzer_off(void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}

/*Function Name:buzzer_beep
Output: Buzzer beeps 1 time
Logic: Turns the buzzer on and off by writing 1 & 0 to PC3
Example Call: buzzer_beep()
*/
void buzzer_beep()
{
	buzzer_on();
	_delay_ms(200);
	buzzer_off();
	_delay_ms(200);
}
