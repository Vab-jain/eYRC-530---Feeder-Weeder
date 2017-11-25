/*********************************************** motion_control_pid.h *****************************************************/

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

int getSpeedDiff(unsigned char L,unsigned char C,unsigned char R)
{
	unsigned char onLine = 0;
	int SpeedDiff;
	// lcd_cursor(2,1);
	// lcd_string("       ");
	if(L >= LFR_Threshhold || C >= LFR_Threshhold || R >= LFR_Threshhold)
	{
		lcd_cursor(2,1);
		lcd_string("onLine  ");
		onLine = 1;
	}
	unsigned int avg, sum;
	sum = L + C + R;
	avg = (int)((1000*L + (1000*2)*C + (1000*3)*R)/sum);

	if(!onLine)
	{
		lcd_cursor(2,1);
		lcd_string("Stopping");
		forward();
		velocity(0,0);
	}

	SpeedDiff = pid(avg,2000);
	return SpeedDiff;
}

void set_PID_consts(float pro,float inte, float deri)
{
	Kp = pro;
	Ki = inte;
	Kd = deri;
}

long pid(int value, int origin)
{
	float pidResult;
	
	int error = value - origin;

	int derivative = error - lastError;
	pidResult = Kp*error + Kd*derivative + Ki*integral;
	error = lastError;
	return (int)pidResult;
}

void start_motion(struct Direction turn_array)
{
	lcd_cursor(2,1);
	lcd_string("Entering start_motion");

	back_mm(10);
	_delay_ms(500);

	int i=0,turn;
	
	set_PID_consts(0.1,0,0); // set Kp, Ki and Kd respectively
			
	while(1)
	{

		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

		flag=0;

		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		

		speed_Difference = getSpeedDiff(Left_white_line,Center_white_line,Right_white_line);
		forward();

		// speed_Difference = 65475 - speed_Difference;

		lcd_print(1,14,maxSpeed,3);
		if (speed_Difference > 0)
		{
			lcd_cursor(2,9);
			lcd_string("P");
		}
		if (speed_Difference < 0)
		{
			lcd_cursor(2,9);
			lcd_string("N");
		}
		//Checking if value of speed_Difference is larger than maxSpeed
		if(speed_Difference > maxSpeed)
		{
			speed_Difference =  maxSpeed;
		}
		else if(speed_Difference < (-maxSpeed))
		{
			speed_Difference = (-maxSpeed);
			lcd_cursor(2,9);
			lcd_string("N");
		}
		
		lcd_print(2,10,speed_Difference,5);

		if(speed_Difference < 0)
			velocity(100 + speed_Difference, 100);
		else
			velocity(100, 100 - speed_Difference);
	}
}