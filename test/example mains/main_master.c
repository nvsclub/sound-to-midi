#include "general_lib.h"
#include "spi_library.h"
#include "i2c_library.h"

ISR(TWI_vect){

  uint8_t data; // Temporary variable to store data

  // REQUEST TO ACKNOWLEDGE
  if (((TWSR & TW_NO_INFO) == TW_SR_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    TWCR |= (1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
  }
  // REQUEST TO TRANSMITT DATA
  else if (((TWSR & TW_NO_INFO) == TW_ST_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    data = TWDR;
    i2c_slave_trans(data);
  }

  // REQUEST TO RECEIVE DATA
  else if ((TWSR & TW_NO_INFO) == TW_SR_DATA_ACK){
    data = TWDR;
    turnLeds(data);
  }

  else{
    TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
  }
}

//  MODULE FUNCTION
//  This Module will collect data from microphone
//  and then send it using spi protocol
//  When called (interrupted) it will receve a MIDI message by
//  I2C protocol and update LEDS's according to message

//  THIS MODULE IS:
//  SPI Master
//  I2C Slave

// MAIN FUNCTION
int main(void){

  // INITIALIZE SERIAL PRINT REQUEIREMENTS
  UBRR0 = baudgen;
  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
  UCSR0C = (3<<UCSZ00);
  _delay_ms(500);

  // MAIN INITIALIZATIONS
  spi_init_master();    // SPI Initialization as master
  i2c_init_slave();     // I2C Initialization as slave
  init_printf_tools();  // Initialize Prints
  sei();                // Initializes Interrupt

  uint8_t microphone_msg;

  while(1){

    // Get message from microphone


    // Transfer microphone message using SPI
    spi_trans(microphone_msg);

  }


}
