/*
 * Test project
 *
 *
 */

#include <avr/io.h>
#include "sram_test.h"
#include "printf_tools.h"


int main(void){

	init_printf_tools();

	printf("Finished sram test with %d defective sections\n", sram_test());

}
