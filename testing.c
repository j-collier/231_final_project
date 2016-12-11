#include "lab3.h"
#include <unistd.h>
#include <sys/alt_alarm.h>
#include <alt_types.h>

#define CALLBACK_TIME 5


/* The function below takes two arguments: value which is the value in decimal
 * to be displayed by the seven-segment displays and a character array that have the encoding
 * to display the provided value in decimal on the seven-segment displays. The provided value
 * can range from 0 - 999. The seven-segment displays must output the entire range correctly.
 */

alt_alarm alt_alarm_int;

alt_u32 scheduler_callback(void * context);



int HEX3to0_display (int value, unsigned char * table);

int deposit_sum = 0;
int global_flag = 0;
int main(void)
{

	while(1)
	{
	/* Declare volatile pointers to the necessary I/O registers (volatile means that IO load
	 * and store instructions will be used to access these pointer locations,
	 * instead of regular memory loads and stores)
	*/

	alt_alarm_start(&alt_alarm_int, alt_ticks_per_second() * CALLBACK_TIME,scheduler_callback, NULL);

	/********** Your declarations should go below! ***********/

	volatile int * pushButtons =  PUSHBUTTON_BASE;

		volatile int * hex3_hex0 = HEX3_HEX0_BASE;

		volatile int * greenLED = GREEN_LED_BASE;

		volatile int * switches = SLIDER_SWITCH_BASE;

		volatile int * redLED = RED_LED_BASE;
	// An array containing values to convert a single digit decimal to the corresponding
	// seven_segment value. The first three elements (table[0] to table[2]) are correct;
	// the rest are not. You need to complete the array below with all the correct conversion values.

	unsigned char table[10] = { 0x3f, 0x06, 0x5b, 0xcf, 0xe6, 0xed, 0xfd, 0x07, 0xff, 0xef};

	/* Declare your local variables that you may need. */
	/********** Your declarations should go below! ***********/

	//valueHex - holds the same number as "value" but converted to hex
	int displayed = 0;
	int price = 0;
	int price2 = 0;
	int price3 = 0;
	int products = 0;
	int products2 = 0;
	int products3 = 0;

	int sum = 0;

	InitialState:
	while((*switches != 0) || (price == 0) || *switches == 512)
	{
		if(*pushButtons == 8)
		{
			price = *switches-(512);
			printf("this is a test for price %d\n",price);
			displayed = HEX3to0_display(price,table);
			 *hex3_hex0 = displayed;
			 usleep(2000000);
		}
		if(*pushButtons == 4)
				{
					price2 = *switches-(512);
					printf("this is a test for price %d\n",price);
					displayed = HEX3to0_display(price,table);
					 *hex3_hex0 = displayed;
					 usleep(2000000);
				}
		if(*pushButtons == 2)
				{
					price3 = *switches-(512);
					printf("this is a test for price %d\n",price);
					displayed = HEX3to0_display(price,table);
					 *hex3_hex0 = displayed;
					 usleep(2000000);
				}
		*hex3_hex0 = NULL;    //change this to blank
		if(*switches == 256)
			goto Setup;

	}

	Setup:
	while(products == 0 || *switches != 0 || *switches == 0)
	{
		if(*pushButtons == 8)
		{
			products = *switches-256;
			printf("this is a test for products %d\n",products);
			usleep(50000);
			displayed = HEX3to0_display(products,table);
			*hex3_hex0 = displayed;

		}
		if(products >= 0 && *switches == 0)
			goto Start;


	}
	Start:
	*greenLED = 0;
	*redLED = 0;
	while(products >= 0)
	{
		printf("number of products %d\n",products);
		if(products == 0)
		{
			goto Setup;
		}
		/* YOU NEED TO ADD CODE IN THIS WHILE LOOP TO PERFORM THE
		 * SPECIFIED FUNCTIONALITIES.
		 */

		displayed = HEX3to0_display(products,table);
		*hex3_hex0 = displayed;

		 if(*pushButtons == 2){
			 displayed = HEX3to0_display(5,table);
			 *hex3_hex0 = displayed;
			 deposit_sum += 5;
			 global_flag =1;
			 usleep(1000000);
		 }
		 else if(*pushButtons == 4){
			displayed = HEX3to0_display(10,table);
			 *hex3_hex0 = displayed;
			deposit_sum += 10;
			global_flag = 1;
			usleep(1000000);
		 }
		 else if(*pushButtons == 8){
			displayed = HEX3to0_display(25,table);
			 *hex3_hex0 = displayed;
			deposit_sum += 25;
			global_flag = 1;
			usleep(1000000);
		 }
		while(*switches == 1)
		{
			if( deposit_sum < price )

			{

				*redLED = 1;
				displayed = HEX3to0_display(0,table);
				*hex3_hex0 = displayed;

				if(*pushButtons == 8 )
				{
					deposit_sum = 0;
					displayed = HEX3to0_display(0,table);
					*hex3_hex0 = displayed;
					*redLED = 0;
					goto Start;
				}
				usleep(1000000);

			}

			if(deposit_sum >= price)
				{
				*greenLED = 128;
				if(*pushButtons == 8 )
				{
				deposit_sum = 0;
				products = products-1;
				displayed = HEX3to0_display(deposit_sum,table);
				*hex3_hex0 = displayed;


						if(products > 0)
						{
							*greenLED = 0;
							goto Start;
						}
						else
						{
							goto Setup;
						}
					}
				}

		}
		displayed = HEX3to0_display(deposit_sum,table);
		*hex3_hex0 = displayed;

		usleep(150000);

	}
	}
}

alt_u32 scheduler_callback(void * context) {
	alt_printf("Scheduler being called.\n");

	if(global_flag == 0)
	{
		deposit_sum = 0;
	}

	global_flag = 0;
	return alt_ticks_per_second() * CALLBACK_TIME;
}

/* The function below takes two arguments: value which is the value in decimal
 * to be displayed by the seven-segment displays and a character array that have the encoding
 * to display the provided value in decimal on the seven-segment displays. The provided value
 * can range from 0 - 999. The seven-segment displays must output the entire range correctly.
 */



int HEX3to0_display (int value, unsigned char * table)
{

	int ones = 0;
	int tens = 0;
	int hundreds = 0;
	int hexOnes = 0;
	int hexTens = 0;
	int hexHundreds = 0;
	int return_val = 0;
	/* YOU NEED TO COMPLETE THIS CONVERSION FUNCTION. */

	//valueHex holds the hex representation of the value input
	//it must be a concatenation of value grabbed from table[]
	//the values passed into table[] need to be taken from the individual digits of value
	//once the values have been concatenated they can be directed to the output pointer so it is displayed on the seven segment display
	//this allows any number from 000 to 999 to be displayed
	//we don't hard code the values in the other part of the program because it can all be gotten from this function


	hundreds = value/100;
	tens = (value - hundreds * 100 )/10;
	ones = (value % 10);

	hexOnes = table[ones];
	hexTens = table[tens];
	hexHundreds = table[hundreds];

	if(hexOnes > 0){
		return_val |= hexOnes;
	}
	if(value > 99){
		return_val |= (hexHundreds << 16);
	}
	if(value > 0){
		return_val |= (hexTens << 8);
	}


	return return_val;
}


