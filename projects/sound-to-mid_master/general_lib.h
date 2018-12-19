#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include "timer_tools.h"
#include "printf_tools.h"
#include "i2c_library.h"
#include "spi_library.h"
#include "microphone.h"
#include "mem_eeprom_test.h"
#include "mem_flash_test.h"
#include "mem_sram_test.h"


#define DEBUG_PRINTF 0

/* DEFINE BAUD RATE FOR SERIAL COMMUNICATION */
#define	baud 57600
#define baudgen ((F_CPU/(16*baud))-1)

/* DEFINE CPU FREQUENCY */
#ifndef F_CPU
#define F_CPU 160000000UL
#endif

/* GENERAL CONFIGURATION DATA */
#define ACK 0x7E

/* DATA TO PRINT VALUES IN BITS */
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

volatile uint16_t timer = 0;

/* I2C VARIABLES */
volatile uint8_t i2c_address_send;
volatile uint8_t i2c_address_receiv;
volatile uint8_t i2c_databuffer[254] = {0};
volatile uint8_t I2C_flag = 0;
/* DEFINES BUFFER VARIABLES TO READ AND WRITE DATA */

/* DEFINE SPI LIBRARIES */
volatile uint8_t spi_address;
volatile uint8_t spi_databuffer[254] = {0};

/* PRINTS VARIABLE IN BITS */
void printBits(size_t const size, void const * const ptr){

  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i=size-1;i>=0;i--){
    for (j=7;j>=0;j--)
    {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  puts("");
}


/* TURN ON LEDS ACCORDING TO MIDI MESSAGE */
void turnLeds(uint8_t data){

  if (data == 0x90){
    PORTD = 0x00;
  }

  else{

#if(1 == DEBUG_PRINTF)
  printf("GOT DATA I2C: ");
  printBits(sizeof(uint8_t), &data);
  printf("\n");
#endif

  if (data == 0xFF){
    PORTD = 0xFC;
  }
  else{
    PORTD = data;
  }
  I2C_flag = 0;

}

  return;
}
