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

  // Microphone Value Between 0 and 100
  if (mic_input > 0x0000 && mic_input <= 0x0064){
    midi_msg = (0x90 << 8) | 0x00;
    return midi_msg;
  }

  // Microphone Value Between 100 and 200
  else if (mic_input > 0x0064 && mic_input <= 0x00C8){
    midi_msg = (0x90 << 8) | 0x01;
    return midi_msg;
  }

  // Microphone Value Between 200 and 300
  else if (mic_input > 0x00C8 && mic_input <= 0x012C){
    midi_msg = (0x90 << 8) | 0x02;
    return midi_msg;
  }

  // Microphone Value Between 300 and 400
  else if (mic_input > 0x012C && mic_input <= 0x0190){
    midi_msg = (0x90 << 8) | 0x03;
    return midi_msg;
  }

  // Microphone Value Between 400 and 500
  else if (mic_input > 0x0190 && mic_input <= 0x01F4){
    midi_msg = (0x90 << 8) | 0x04;
    return midi_msg;
  }

  // Microphone Value Between 500 and 600
  else if (mic_input > 0x01F4 && mic_input <= 0x0258){
    midi_msg = (0x90 << 8) | 0x05;
    return midi_msg;
  }

  // Microphone Value Between 600 and 700
  else if (mic_input > 0x0258 && mic_input <= 0x02BC){
    midi_msg = (0x90 << 8) | 0x06;
    return midi_msg;
  }

  // Microphone Value Between 700 and 800
  else if (mic_input > 0x02BC && mic_input <= 0x0320){
    midi_msg = (0x90 << 8) | 0x07;
    return midi_msg;
  }

  // Microphone Value Between 800 and 900
  else if (mic_input > 0x0320 && mic_input <= 0x0384){
    midi_msg = (0x90 << 8) | 0x08;
    return midi_msg;
  }

  // Between 900 and 1000
  else if (mic_input > 0x0384 && mic_input <= 0x03E8){
    midi_msg = (0x90 << 8) | 0x09;
    return midi_msg;
  }

  // Microphone Value Between 1000 and 1023
  else if (mic_input > 0x03E8 && mic_input <= 0x03FF){
    midi_msg = (0x90 << 8) | 0x0A;
    return midi_msg;
  }

  else{
    midi_msg = 0x0000;
    return midi_msg;
  }

}


// TURN ON LEDS ACCORDING TO MIDI MESSAGE
void turnLeds(uint16_t midi_msg){

  // Control defines if Led turns Off or On
  uint8_t control = (uint8_t)((midi_msg & 0xFF00) >> 8);

  if (control == 0x90){
    PORTD |= (uint8_t)((midi_msg & 0x00FF));
    return;
  }

  else if (control == 0x80){
    PORTD &= ~((uint8_t)(midi_msg & 0x00FF));
    return;
  }









}
