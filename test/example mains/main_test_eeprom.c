/*
 * Test project
 *
 *
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "sram_test.h"
#include "mem_eeprom_test.h"
#include "printf_tools.h"

#define	baud 57600UL
#define baud_gen ((F_CPU/(16*baud))-1)

int main(void){

	UBRR0 = baud_gen;

  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);

  _delay_ms(500);

	init_printf_tools();

	printf("Test started.\n");

	printf("%x\n", sign_uc());
	printf(" %x\n", check_sign());

	printf("  %x\n", get_og_flash_hash());

	update_og_flash_hash();

	printf("   %x\n", get_og_flash_hash());

	printf("Test done.\n");

	_delay_ms(5000);

}
