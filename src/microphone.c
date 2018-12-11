#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>


///Initializing ADC so we can read the digitalized analog inputs
void init_adc(){
	//Defining reference value as AVcc
	ADMUX = ADMUX | (1<<REFS0);
	//Turns off the digital buffer in the used ports for energy saving
	DIDR0 = DIDR0 | (1<<PC0);
	//Activating ADC and set pre-divisor to 128
	ADCSRA = ADCSRA | (1<<ADEN) | (7<<ADPS0);
}

//Reads the value of the indicated channel
uint8_t read_adc(uint8_t channel){
	//Choosing channel. CAREFULL DO NOT interfere with the reference values
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	//Conversion (manual mode since ADATE=0 in ADCSRA)
	ADCSRA |= (1<<ADSC);
	//Wait for conversion to end (ADSC to return to value 0)
	while(ADCSRA & (1<<ADSC));
	//Returns the value
	return ADC >> 2;
}
