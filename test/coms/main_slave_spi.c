
#include "general_lib.h"
#include "spi_library.h"

ISR(SPI_STC_vect){
  unsigned char data;
  data = spi_receiv();
  spi_trans(ACK);
  printBits(sizeof(char), &data);
}


// MAIN FUNCTION
int main (void){

  UBRR0 = baudgen;
  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
  UCSR0C = (3<<UCSZ00);

  _delay_ms(500);

  spi_init_slave();    // Initializes SPI
  init_printf_tools();  // Initialize Prints
  sei();

  printf("STARTING TRANSMISSION");
  while(1);

}
