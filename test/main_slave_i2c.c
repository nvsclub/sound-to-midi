#include "general_lib.h"
#include "i2c_library.h"

ISR(TWI_vect){

  uint8_t data;

  // REQUEST TO RECEIVE DATA
  if (((TWSR & TW_NO_INFO) == TW_SR_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    TWCR |= (1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
  }

  // REQUEST TO TRANSMITT DATA
  else if (((TWSR & TW_NO_INFO) == TW_ST_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    data = TWDR;
    i2c_slave_trans(data);
  }

  else if ((TWSR & TW_NO_INFO) == TW_SR_DATA_ACK){
    data = TWDR;
    i2c_slave_receive(data);
  }
  else{
    TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
  }
}

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
  i2c_init_slave();
  sei();

  uint8_t data;

  i2c_address_send = 0x00;
  i2c_address_receiv = 0x00;

  uint8_t counter;
  counter = 0x00;

  while(1){

    if (i2c_databuffer[counter] != 0){
      printf("RESULTS_%d: ", counter);
      data = i2c_databuffer[counter];
      printBits(sizeof(char),&data);
      counter++;
    }

    _delay_ms(2000);
  }
}
