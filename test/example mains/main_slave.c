#include "general_lib.h"

#define RECOGNITION_THRESHOLD

ISR(SPI_STC_vect){
  unsigned char data;
  data = spi_receiv();
  spi_manage(data);
  printBits(sizeof(uint8_t), &data);
}

void i2c_handler(uint8_t midi_msg){
  // Start i2c protocol to send 1st byte of MIDI message
  i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
  // Send 1st byte of MIDI message
  uint8_t split = ((midi_msg & 0xFF00) >> 8);
  i2c_write(split);
  i2c_stop_master();

  return;
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
  uint8_t split;

  spi_address = 0;
  split = 0;

  int8_t og_samples_a4[24] = {-86, -74, -4, 36, 41, -2, -29, -14, 19, 5, -3, 30, 40, 43, 54, 56, 14, -25, -84, -75, -17, 29, 42, -2};
  int8_t og_samples_b4[24] = {-102, -97, -38, 13, 24, 10, 26, 59, 63, 79, 75, 21, 4, 21, 3, -70, -103, -103, -46, 4, 23, 17, 46, 63};
  int8_t og_samples_c4[24] = {-66, -60, -8, 57, 80, 27, -55, -91, 33, 86, 86, 12, -44, -55, -29, 5, -7, -43, -61, -51, -6, 56, 81, 56};
  int8_t og_samples_d4[24] = {-72, -17, 45, 81, 61, -28, -37, -11, 15, 4, -42, -77, -52, 78, 39, -28, -79, -34, 41, 92, 69, -22, -23, -5};
  int8_t og_samples_e4[24] = {-21, -20, -8, 14, 41, 60, 46, -11, -39, -49, -34, -20, -20, -23, 10, 66, 40, 7, -14, -32, -32, -2, 35, 3};
  int8_t og_samples_f4[24] = {-60, -15, 41, 63, 33, -8, -38, -44, 22, 22, 4, -6, -10, 5, 37, 43, 11, 23, 31, 19, -5, -29, -59, -68};
  int8_t og_samples_g4[24] = {-42, -82, -87, -34, 64, 104, 57, -7, -47, 3, 25, 13, 3, -4, 18, 62, 47, 63, 71, 39, -15, -61, -95, -85};
  int8_t samples_fifo[24];
  uint8_t samples_pointer = 23;
  uint8_t array_pointer;
  uint8_t k;
  int32_t correlation_a = 0, correlation_b = 0, correlation_c = 0, correlation_d = 0, correlation_e = 0, correlation_f = 0, correlation_g = 0;


  while(1){

    for(uint8_t i = 0; i < 0xFF; i++){

      if (spi_databuffer[i] != 0){

        results = spi_databuffer[i];
        spi_databuffer[i] = 0;

        if(samples_pointer == 23) 
          samples_pointer = 0;
        else
          samples_pointer++;

        samples_fifo[samples_pointer] = read_adc(0) - 114;

        correlation_a = 0;
        correlation_b = 0;
        correlation_c = 0;
        correlation_d = 0;
        correlation_e = 0;
        correlation_f = 0;
        correlation_g = 0;
        for(k = 0; k < 24; k++){
          array_pointer = (samples_pointer + k) % 24;
          correlation_a += og_samples_a4[k] * samples_fifo[array_pointer];
          correlation_b += og_samples_b4[k] * samples_fifo[array_pointer];
          correlation_c += og_samples_c4[k] * samples_fifo[array_pointer];
          correlation_d += og_samples_d4[k] * samples_fifo[array_pointer];
          correlation_e += og_samples_e4[k] * samples_fifo[array_pointer];
          correlation_f += og_samples_f4[k] * samples_fifo[array_pointer];
          correlation_g += og_samples_g4[k] * samples_fifo[array_pointer];

        }
      
        if((correlation_a > RECOGNITION_THRESHOLD) && (correlation_a > correlation_b) && (correlation_a > correlation_c) && (correlation_a > correlation_d) && (correlation_a > correlation_e) && (correlation_a > correlation_f) && (correlation_a > correlation_g)){
          i2c_handler(0x80);
        }
        else if((correlation_b > RECOGNITION_THRESHOLD) && (correlation_b > correlation_c) && (correlation_b > correlation_d) && (correlation_b > correlation_e) && (correlation_b > correlation_f) && (correlation_b > correlation_g)){
          i2c_handler(0x40);
        }
        else if((correlation_c > RECOGNITION_THRESHOLD) && (correlation_c > correlation_d) && (correlation_c > correlation_e) && (correlation_c > correlation_f) && (correlation_c > correlation_g)){
          i2c_handler(0x20);
        }
        else if((correlation_d > RECOGNITION_THRESHOLD) && (correlation_d > correlation_e) && (correlation_d > correlation_f) && (correlation_d > correlation_g)){
          i2c_handler(0x10);
        }
        else if((correlation_e > RECOGNITION_THRESHOLD) && (correlation_e > correlation_f) && (correlation_e > correlation_g)){
          i2c_handler(0x08);
        }
        else if((correlation_f > RECOGNITION_THRESHOLD) && (correlation_f > correlation_g)){
          i2c_handler(0x04);
        }
        else if((correlation_g > RECOGNITION_THRESHOLD)){
          i2c_handler(0x02);
        }
        else
          i2c_handler(0x01);
     }

    }
    
  }

}
