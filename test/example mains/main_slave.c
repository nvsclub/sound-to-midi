#include "general_lib.h"

ISR(SPI_STC_vect){
  unsigned char data;
  data = spi_receiv();
  spi_manage(data);
  printBits(sizeof(uint8_t), &data);
}

//  MODULE FUNCTION
//  This module will receive data from a SPI protocol
//  then will convert the data to a MIDI message
//  and finally send the command to turn on the LEDS using I2C protocol

//  THIS MODULE IS:
//  SPI Slave
//  I2C Master

// MAIN FUNCTION
int main (void){

  // INITIALIZE SERIAL PRINT REQUEIREMENTS
  UBRR0 = baudgen;
  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
  UCSR0C = (3<<UCSZ00);
  _delay_ms(500);

  // MAIN INITIALIZATIONS
  spi_init_slave();     // SPI Initialization as slave
  i2c_init_master();    // I2C Initialization as master
  init_printf_tools();  // Initialize Prints
  sei();                // Initializes Interrupt

  // GENERAL VARIABLES
  spi_address = 0;      // Sets initial address for receiving buffer
  uint16_t midi_msg;    // Will contain MIDI Message to Send
  uint8_t results;      // Results from communication protocols
  uint8_t count;
  uint8_t split;

  count = 0;
  spi_address = 0;
  split = 0;

  while(1){

    for(uint8_t i = 0; i < 0xFF; i++){

      if (count < 40){
        if (spi_databuffer[i] != 0){

          results = spi_databuffer[i];

          // Get MIDI MESSAGE
          midi_msg = MIDI_interp(results);

          // Start i2c protocol to send 1st byte of MIDI message
          results = i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
          printf("RESULTS_START: ");
          printBits(sizeof(char), &results);

          // Send 1st byte of MIDI message
          split = ((midi_msg & 0xFF00) >> 8);
          results = i2c_write(split);
          printf("RESULTS_WRITE_1: ");
          printBits(sizeof(char), &split);
          i2c_stop_master();

          // (Re)Start i2c protocol to send 2nd byte of MIDI message
          results = i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
          printf("RESULTS_START_2: ");
          printBits(sizeof(char), &results);

          // Send 2nd byte of MIDI message
          split = (midi_msg & 0x00FF);
          results = i2c_write(((midi_msg & 0x00FF) >> 8));
          printf("RESULTS_WRITE_2: ");
          printBits(sizeof(char), &split);

          // Stop Transmission
          i2c_stop_master();
          _delay_ms(2000);
          count++;

        }
      }
    }
  }
}
