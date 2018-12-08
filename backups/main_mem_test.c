#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
//#include "mem_sram_test.h"
#include "mem_flash_test.h"
#include "printf_tools.h"

#define	baud 9600UL  // baud rate
#define baud_gen ((F_CPU/(16*baud))-1)  //baud divider

int main (void){

  printf("\n INITIALIZING \n");

	UBRR0 = baud_gen;

  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);

  _delay_ms(500);

  init_printf_tools();
  init_mili_timers();

  _delay_ms(500);

  printf(" INITIALIZED \n");

	// set PORT B as output
  DDRB = 0xFF;

  // turn all ports on
  PORTB = 0xFF;

  printf("Started ram test \n");

  //uint8_t no_errors = 0;
  //uint8_t no_errors = test_sram_memory();
  uint8_t no_errors = test_flash_memory();

  printf("End_ram-test %d \n", no_errors);

  if (no_errors != 0){
    // if there are errors disable all leds
    PORTB = 0x00;
    printf("\n Erros: %d \n", no_errors);
  }
  
  printf("Test succeded \n");


}