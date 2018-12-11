#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "timer_tools.h"
#include "printf_tools.h"

// DEFINE BAUD RATE FOR SERIAL COMMUNICATION
#define	baud 57600
#define baudgen ((F_CPU/(16*baud))-1)

// DEFINE CPU FREQUENCY
#ifndef F_CPU
#define F_CPU 160000000UL //11059200UL
#endif

// GENERAL CONFIGURATION DATA
#define ACK 0x7E
#define true 0x01
#define false 0x00

// DATA TO PRINT VALUES IN BITS
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

// PRINTS VARIABLE IN BITS
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

// CONVERT MICROPHONE INPUT INTO MIDI MESSAGE
uint16_t MIDI_interp(uint16_t mic_input){

  uint16_t midi_msg;
  // Between 0 and 100
  if (mic_input > 0x0000 && mic_input < 0x0064){

  }








}
