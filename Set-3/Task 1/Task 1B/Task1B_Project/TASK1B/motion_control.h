/*********************************************** motion_control.h *****************************************************/

#ifndef Header_files

#define Header_files
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

#endif

#include "config_motion.h"

#include <math.h> //included to support power function



unsigned char flag = 0;
unsigned char Left_black_line = 0;
unsigned char Center_black_line = 0;
unsigned char Right_black_line = 0;
unsigned char LFR_Threshhold = 20;

const int maxSpeed = 255;
signed int speed_Difference = 0;
int lastError = 0, integral=0;
float Kp = 0, Kd = 0, Ki = 0;
// unsigned char lastValue;



// Function Headings
void port_init();
void init_devices (void);
int getSpeedDiff(unsigned char,unsigned char,unsigned char);
void set_PID_consts(float, float, float);
long pid(int , int);
void start_motion(struct Direction);


//Function to Initialize PORTS
void port_init()
{
	buzzer_pin_config();
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();	
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config

}


// Function to initialise all devices and sensors
void init_devices (void)
{
 	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer5_init();
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	sei();   //Enables the global interrupts
}


//
void start_motion(struct Direction turn_array)
{
	lcd_cursor(2,1);
	lcd_string("Entering start_motion");

	back_mm(10);
	_delay_ms(500);

	int i=0,turn;
	
	while(1)
	{

		Left_black_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_black_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_black_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

		flag=0;

		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		
		

		if(Center_black_line>LFR_Threshhold)
		{
			flag=1;
			forward();
			velocity(100,100);
		}

		if((Left_black_line<LFR_Threshhold) && (flag==0))
		{
			flag=1;
			forward();
			velocity(130,70);
		}

		else if((Right_black_line<LFR_Threshhold) && (flag==0))
		{
			flag=1;
			forward();
			velocity(70,130);
		}

		if(Center_black_line<LFR_Threshhold && Left_black_line<LFR_Threshhold && Right_black_line<LFR_Threshhold)
		{
			forward();
			velocity(0,0);
		}

		if((Center_black_line>LFR_Threshhold && Left_black_line>LFR_Threshhold)||(Center_black_line>LFR_Threshhold && Right_black_line>LFR_Threshhold))
		{
			turn = turn_array.directions[i++];
			lcd_cursor(1,14);
			if(turn == -1)
				lcd_string("Lef");
			else if (turn == 0)
				lcd_string("str");
			else if (turn == 1)
				lcd_string("Rig");
			if(turn == 2)
			{
				stop();
				return;
			}
			else if(turn == -1)
			{
				lcd_cursor(2,1);
				lcd_string("Calling left_degrees");
				left_degrees(30);
				lcd_cursor(2,1);
				lcd_string("Calling soft_left_degrees");
				soft_left_degrees(40);
				stop();
				_delay_ms(500);
				lcd_cursor(2,1);
				lcd_string("Exit joint detect");
			}
			else if(turn == 1)
			{
				lcd_cursor(2,1);
				lcd_string("Calling right_degrees");
				right_degrees(30);
				lcd_cursor(2,1);
				lcd_string("Calling soft_right_degrees");
				soft_right_degrees(40);
				stop();
				_delay_ms(500);
				lcd_cursor(2,1);
				lcd_string("Exit joint detect");
			}
			else if(turn == 0)
			{
				forward();
				velocity(100,100);
				_delay_ms(500);
			}
		}
	}		
}
