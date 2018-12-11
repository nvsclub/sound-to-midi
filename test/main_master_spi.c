#include "general_lib.h"
#include "spi_library.h"

// MAIN FUNCTION
int main (void){

  unsigned char data;   // Data to be Sent and/or Received
  unsigned char data_of;
  DDRD = 0xFF;         // PD0 defined as Output

  UBRR0 = baudgen;
  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);

  _delay_ms(500);

  spi_init_master();    // Initializes SPI
  init_printf_tools();  // Initialize Prints

  while(1){
    data = true;
    spi_trans(data); // Sends data (in this case x)
    data_of = spi_receiv();
    printBits(sizeof(char), &data);
    printBits(sizeof(char), &data_of);
    _delay_ms(2000);
  }
}
