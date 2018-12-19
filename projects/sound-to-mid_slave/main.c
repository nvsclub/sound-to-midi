#include "general_lib.h"

#define RECOGNITION_THRESHOLD 10000


#define DEBUG_PRINTF 0
#define SETUP_HASH 0
/* only enable flash test when both DEBUG_PRINTF and SETUP_HASH are 0
 * and in mem_sram_test DEBUG_PRINTF is 0 and DODGE_STACK_SECTION is 1
 */
#define ENABLE_FLASH_TEST 1

ISR(SPI_STC_vect){
  unsigned char data;
  data = spi_receiv();
  spi_manage(data, spi_address, spi_databuffer);
}

/* MODULE FUNCTION
 * This module will receive data from a SPI protocol
 * then will convert the data to a MIDI message
 * and finally send the command to turn on the LEDS using I2C protocol
 *
 * THIS MODULE IS:
 * SPI Slave
 * I2C Master */

/* MAIN FUNCTION */
int main (void){

#if(1 == SETUP_HASH)
  update_og_flash_hash(0x2C);
  sign_uc();
#endif

  /* INITIALIZE SERIAL PRINT REQUEIREMENTS */
#if(1 == DEBUG_PRINTF)
  UBRR0 = baudgen;
  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
  UCSR0C = (3<<UCSZ00);
  init_printf_tools();  /* Initialize Prints */
  printf("Serial interface initialized\n");
  printf("Initializing memory tests: \n");
  if(test_flash_memory(get_og_flash_hash()))
	  printf("Flash test without anomalies\n");
  else
	  printf("Flash test found errors in binary\n");
  if(sram_test())
	  printf("SRAM test found defects in section %d\n", sram_test());
  else
	  printf("SRAM test without anomalies");

#endif

#if(1 == ENABLE_FLASH_TEST)
  /* Do memory tests */
  if(sram_test() != 0 || test_flash_memory(get_og_flash_hash()) != 0){
    /* If any error occurred, then go into defective state and blink the led13 */
	DDRB = 0x20;

    while(1) {
      PORTB = 0x20;
      _delay_ms(1000);
      PORTB = 0x00;
      _delay_ms(1000);
    }
  }

#else
  /* Do memory tests */
  if(sram_test() != 0){
    /* If any error occurred, then go into defective state and blink the led13 */
	DDRB = 0x20;

    while(1) {
      PORTB = 0x20;
      _delay_ms(1000);
      PORTB = 0x00;
      _delay_ms(1000);
    }
  }
#endif

  /* MAIN INITIALIZATIONS */
  spi_init_slave();     /* SPI Initialization as slave */
  i2c_init_master();    /* I2C Initialization as master */
  sei();                /* Initializes Interrupt */

  /* GENERAL VARIABLES INITIALIZATION */
  uint8_t results;      /* Results from communication protocols */
  spi_address = 0;      /* Sets initial address for receiving buffer */

  /* Signals for comparison */
  int8_t og_samples_a4[24] = {-86, -74, -4, 36, 41, -2, -29, -14, 19, 5, -3, 30, 40, 43, 54, 56, 14, -25, -84, -75, -17, 29, 42, -2};
  int8_t og_samples_b4[24] = {-102, -97, -38, 13, 24, 10, 26, 59, 63, 79, 75, 21, 4, 21, 3, -70, -103, -103, -46, 4, 23, 17, 46, 63};
  int8_t og_samples_c4[24] = {-66, -60, -8, 57, 80, 27, -55, -91, 33, 86, 86, 12, -44, -55, -29, 5, -7, -43, -61, -51, -6, 56, 81, 56};
  int8_t og_samples_d4[24] = {-72, -17, 45, 81, 61, -28, -37, -11, 15, 4, -42, -77, -52, 78, 39, -28, -79, -34, 41, 92, 69, -22, -23, -5};
  int8_t og_samples_e4[24] = {-21, -20, -8, 14, 41, 60, 46, -11, -39, -49, -34, -20, -20, -23, 10, 66, 40, 7, -14, -32, -32, -2, 35, 3};
  int8_t og_samples_f4[24] = {-60, -15, 41, 63, 33, -8, -38, -44, 22, 22, 4, -6, -10, 5, 37, 43, 11, 23, 31, 19, -5, -29, -59, -68};


  int8_t samples_fifo[24];
  uint8_t samples_pointer = 23;
  uint8_t array_pointer;
  uint8_t k;
  int32_t correlation_a = 0, correlation_b = 0, correlation_c = 0, correlation_d = 0, correlation_e = 0, correlation_f = 0;

  /* CLEANS SPI BUFFER */
  for(uint8_t i = 0; i < 255; i++){
    spi_databuffer[i] = 0;
  }

  while(1){

    /* IF ALL SAMPLES HAVE BEEN SENT BY MASTER */
    if (spi_databuffer[254] != 0){

#if(1 == DEBUG_PRINTF)

      printf("BEGIN SAMPLE PROCESSING. \n");

#endif

      /* PROCESSNG SAMPLES */
      for(uint8_t i = 0; i < 255; i++){

        if(samples_pointer == 23)
          samples_pointer = 0;
        else
          samples_pointer++;

        samples_fifo[samples_pointer] = spi_databuffer[i];
        spi_databuffer[i] = 0;

        correlation_a = 0;
        correlation_b = 0;
        correlation_c = 0;
        correlation_d = 0;
        correlation_e = 0;
        correlation_f = 0;

        for(k = 0; k < 24; k++){
          array_pointer = (samples_pointer + k) % 24;
          correlation_a += og_samples_a4[k] * samples_fifo[array_pointer];
          correlation_b += og_samples_b4[k] * samples_fifo[array_pointer];
          correlation_c += og_samples_c4[k] * samples_fifo[array_pointer];
          correlation_d += og_samples_d4[k] * samples_fifo[array_pointer];
          correlation_e += og_samples_e4[k] * samples_fifo[array_pointer];
          correlation_f += og_samples_f4[k] * samples_fifo[array_pointer];
        }

        /* BREAK IF GOOD CORRELATION */
        if ((correlation_a > RECOGNITION_THRESHOLD) || (correlation_b > RECOGNITION_THRESHOLD) || (correlation_c > RECOGNITION_THRESHOLD) || (correlation_d > RECOGNITION_THRESHOLD) || (correlation_e > RECOGNITION_THRESHOLD) || (correlation_f > RECOGNITION_THRESHOLD)){
          break;
        }
      }

#if(1 == DEBUG_PRINTF)

      printf("A: %d \n", correlation_a);
      printf("B: %d \n", correlation_b);
      printf("C: %d \n", correlation_c);
      printf("D: %d \n", correlation_d);
      printf("E: %d \n", correlation_e);
      printf("F: %d \n", correlation_f);

#endif

      /* SEND LED MSGS */
      if((correlation_a > RECOGNITION_THRESHOLD) && (correlation_a > correlation_b) && (correlation_a > correlation_c) && (correlation_a > correlation_d) && (correlation_a > correlation_e) && (correlation_a > correlation_f)){
        i2c_handler(0x9004);
      }
      else if((correlation_b > RECOGNITION_THRESHOLD) && (correlation_b > correlation_c) && (correlation_b > correlation_d) && (correlation_b > correlation_e) && (correlation_b > correlation_f)){
        i2c_handler(0x9008);
      }
      else if((correlation_c > RECOGNITION_THRESHOLD) && (correlation_c > correlation_d) && (correlation_c > correlation_e) && (correlation_c > correlation_f)){
        i2c_handler(0x9010);
      }
      else if((correlation_d > RECOGNITION_THRESHOLD) && (correlation_d > correlation_e) && (correlation_d > correlation_f)){
        i2c_handler(0x9020);
      }
      else if((correlation_e > RECOGNITION_THRESHOLD) && (correlation_e > correlation_f)){
        i2c_handler(0x9040);
      }
      else if((correlation_f > RECOGNITION_THRESHOLD)){
        i2c_handler(0x9080);
      }
      else{
        i2c_handler(0x90FF);
      }

      correlation_a = 0;
      correlation_b = 0;
      correlation_c = 0;
      correlation_d = 0;
      correlation_e = 0;
      correlation_f = 0;

      for(uint8_t i = 0; i < 255; i++){
        spi_databuffer[i] = 0;
      }

#if(1 == DEBUG_PRINTF)

      printf("END OF PROCESSING SAMPLE.\n\n");

#endif

    }
  }
}
