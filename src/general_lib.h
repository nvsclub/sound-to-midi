#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "timer_tools.h"
#include "printf_tools.h"
#include "i2c_library.h"
#include "spi_library.h"
#include "microphone.h"

// DEFINE BAUD RATE FOR SERIAL COMMUNICATION
#define	baud 57600
#define baudgen ((F_CPU/(16*baud))-1)

// DEFINE CPU FREQUENCY
#ifndef F_CPU
#define F_CPU 160000000UL //11059200UL
#endif

// GENERAL CONFIGURATION DATA
#define ACK 0x7E
#define BUFFER_SIZE 0xFF

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
uint16_t MIDI_interp(uint8_t mic_input){

  uint16_t midi_msg = 0x0000;
  midi_msg = (0x90 << 8) | 0x00;
  return midi_msg;

  /*
  // Microphone Value Between 0 and 35
  if (mic_input > 0x70){
    midi_msg = (0x90 << 8) | 0x00;
    return midi_msg;
  }
  return midi_msg;
  */

}

/*
// CONVERT MICROPHONE INPUT INTO MIDI MESSAGE
uint16_t MIDI_interp(uint8_t mic_input){

  uint16_t midi_msg;

  // Microphone Value Between 0 and 35
  if (mic_input > 0x00 && mic_input <= 0x23){
    midi_msg = (0x90 << 8) | 0x00;
    return midi_msg;
  }

  // Microphone Value Between 35 and 70
  else if (mic_input > 0x23 && mic_input <= 0x46){
    midi_msg = (0x90 << 8) | 0x01;
    return midi_msg;
  }

  // Microphone Value Between 50 and 105
  else if (mic_input > 0x46 && mic_input <= 0x69){
    midi_msg = (0x90 << 8) | 0x02;
    return midi_msg;
  }

  // Microphone Value Between 105 and 140
  else if (mic_input > 0x69 && mic_input <= 0x8C){
    midi_msg = (0x90 << 8) | 0x03;
    return midi_msg;
  }

  // Microphone Value Between 140 and 175
  else if (mic_input > 0x8C && mic_input <= 0xAF){
    midi_msg = (0x90 << 8) | 0x04;
    return midi_msg;
  }

  // Microphone Value Between 175 and 210
  else if (mic_input > 0xAF && mic_input <= 0xD2){
    midi_msg = (0x90 << 8) | 0x05;
    return midi_msg;
  }

  // Microphone Value Between 210 and 245 (256)
  else if (mic_input > 0xD2 && mic_input <= 0xFF){
    midi_msg = (0x90 << 8) | 0x06;
    return midi_msg;
  }

  else{
    midi_msg = 0x0000;
    return midi_msg;
  }
}
*/

// TURN ON LEDS ACCORDING TO MIDI MESSAGE
void turnLeds(uint8_t data){

  if ((data == 0x90) || (data == 0x08)){
    i2c_databuffer[0] = data;
    //printf("nig %x \n", i2c_databuffer[0]);
    return;
  }

  else{
    if (i2c_databuffer[0] == 0x90){
      //printf("nigga\n");
      PORTD ^= 0xFF;
      i2c_databuffer[0] = 0;
      return;
    }
  }
  return;
}
  /*

    else if (i2c_databuffer[0] == 0x80){
      PORTD &= ~data;
      i2c_databuffer[0] = 0;
      return;
    }
    else{
      printf("ERROR in TURN LEDS");
      return;
    }
  }
}
*/
