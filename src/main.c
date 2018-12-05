#include <stdio.h>
#include <stdlib.h>

/*

DDDDDDDD

*/

#include <avr/io.h>
#include <util/delay.h>
#include "ram_test.h"
#include "printf_tools.h"



#define cpu_freq 12000000UL
#define	baud 9600  // baud rate
#define baud_gen ((cpu_freq/(16*baud))-1)  //baud divider

#define sram_starting_address (0x0100)

int main (void){

	UBRR0 = baud_gen;

  UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);

  init_printf_tools();

  printf("\n INICIALIZADO \n");

	// set PORT B as output
  DDRB = 0xFF;

  // turn all ports on
  PORTB = 0xFF;

  uint8_t no_errors = run_memory_test((uint8_t *) sram_starting_address);

  if (no_errors != 0){
    // if there are errors disable all leds
    PORTB = 0x00;
  }

  printf("\n Erros: %d \n", no_errors);

}