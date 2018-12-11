#include "general_lib.h"
#include "i2c_library.h"

int main(void){

  //INIT MASTER DEVICE

  // SET PORTD as OUTPUT and ALL Ports to LOW
  DDRD=0xFF;
  PORTD=0x00;

  UBRR0 = baudgen;
  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
  UCSR0C = (3<<UCSZ00);
  _delay_ms(500);

  init_printf_tools();
  i2c_init_master();

  uint8_t results;
  uint8_t data;

  while(1){

    results = i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
    printf("RESULTS_1 START ");
    printBits(sizeof(char), &results);
    results = i2c_write(0xFF);
    printf("RESULTS_1 WRITE ");
    printBits(sizeof(char), &results);
    i2c_stop_master();

    _delay_ms(4000);

    results = i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
    printf("RESULTS_2 START ");
    printBits(sizeof(char), &results);
    results = i2c_write(0x55);
    printf("RESULTS_2 WRITE ");
    printBits(sizeof(char), &results);
    results = i2c_start(I2C_ADDRESS | I2C_READ, 0x01);
    printf("RESULTS_2 START_REP ");
    printBits(sizeof(char), &results);
    data = i2c_read_w_ack();
    printf("RESULTS_2 READ ");
    printBits(sizeof(char), &data);
    i2c_stop_master();

    _delay_ms(10000);
  }
}
