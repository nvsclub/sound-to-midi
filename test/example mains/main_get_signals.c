#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "printf_tools.h"
#include "microphone.h"

#define	baud 9600UL
#define baud_gen ((F_CPU/(16*baud))-1)

#define T1COUNT 65536-25
// 0.1s = 25000 clock cycles @ 16MHz/64

uint16_t volatile timer = 0;
uint8_t volatile toggle = 0;

void setup_timer(){
  // Disable Interrupts
	cli();

  // Normal Mode
	TCCR1A=0x00;
  // Stop timer
	TCCR1B=0x00;
  // Setup timer counter
	TCNT1 = T1COUNT;

  // Activate interrupts
	TIMSK1=0x01;
	CLKPR=0x00;

  // Enable interrupts
	sei();

  // Start the timer counter
	TCCR1B = 0b00000011;

}

ISR(TIMER1_OVF_vect){
	// Reload the number of cycles needed
  TCNT1=T1COUNT;
	if (65535!=timer) {
		timer++;
    if(toggle == 0)
      toggle = 1;
    else
      toggle = 0;

	}

	else timer=0;

}

int main(){

	UBRR0 = baud_gen;

  UCSR0A = 0;
  UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);

  _delay_ms(500);

  init_printf_tools();
  setup_timer();
  init_adc();

  uint8_t microphone_note_samples[256];
  uint16_t i = 1;
  uint8_t pretoggle = toggle;
  
  while(i){
    if(read_adc(0) > 170)
      i = 0;
  }

  for(i = 0; i < 255; i++){
    while(pretoggle == toggle);
    pretoggle = toggle;
    microphone_note_samples[i] = read_adc(0);

  }

  i = 0;
  while(i < 255){
    printf("%d\n", microphone_note_samples[i]);
    i++;
  }

  printf("\n\n\n\n\n\n\n");

}
