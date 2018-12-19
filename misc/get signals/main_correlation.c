#include "general_lib.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "printf_tools.h"
#include "microphone.h"

#define	baud 9600UL
#define baud_gen ((F_CPU/(16*baud))-1)

#define RECOGNITION_THRESHOLD 10000

#define T1COUNT 65536-25
// 0.0001s = 25 clock cycles @ 16MHz/64
// 10kHz

uint16_t volatile timer = 0;
uint8_t volatile toggle = 0;

void init_timer(){
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

  DDRB = 0xFF;	/* set PORT B as output */
  PORTB = 0x00;		/* Turn led off */

  _delay_ms(500);

  // MAIN INITIALIZATIONS
  spi_init_master();    // SPI Initialization as master
  i2c_init_slave();     // I2C Initialization as slave
  init_printf_tools();  // Initialize Prints
  init_adc();           // Initialize ADC
  init_timer();         // Initialize timers
  // REDUNDANT IN THE TIMERS - sei();                // Initializes Interrupt

  int8_t og_samples_a4[24] = {-86, -74, -4, 36, 41, -2, -29, -14, 19, 5, -3, 30, 40, 43, 54, 56, 14, -25, -84, -75, -17, 29, 42, -2};
  int8_t og_samples_b4[24] = {-102, -97, -38, 13, 24, 10, 26, 59, 63, 79, 75, 21, 4, 21, 3, -70, -103, -103, -46, 4, 23, 17, 46, 63};
  int8_t og_samples_c4[24] = {-66, -60, -8, 57, 80, 27, -55, -91, 33, 86, 86, 12, -44, -55, -29, 5, -7, -43, -61, -51, -6, 56, 81, 56};
  int8_t og_samples_d4[24] = {-72, -17, 45, 81, 61, -28, -37, -11, 15, 4, -42, -77, -52, 78, 39, -28, -79, -34, 41, 92, 69, -22, -23, -5};
  int8_t og_samples_e4[24] = {-21, -20, -8, 14, 41, 60, 46, -11, -39, -49, -34, -20, -20, -23, 10, 66, 40, 7, -14, -32, -32, -2, 35, 3};
  int8_t og_samples_f4[24] = {-60, -15, 41, 63, 33, -8, -38, -44, 22, 22, 4, -6, -10, 5, 37, 43, 11, 23, 31, 19, -5, -29, -59, -68};
  int8_t og_samples_g4[24] = {-42, -82, -87, -34, 64, 104, 57, -7, -47, 3, 25, 13, 3, -4, 18, 62, 47, 63, 71, 39, -15, -61, -95, -85};
  int8_t samples_fifo[24];
  uint8_t samples_pointer = 23;
  uint8_t i;
  uint8_t pretoggle = toggle;
  uint8_t array_pointer;
  int32_t correlation_a = 0, correlation_b = 0, correlation_c = 0, correlation_d = 0, correlation_e = 0, correlation_f = 0, correlation_g = 0;
  
  while(1){
    if(toggle != pretoggle){
      pretoggle = toggle;

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
      for(i = 0; i < 24; i++){
        array_pointer = (samples_pointer + i) % 24;
        correlation_a += og_samples_a4[i] * samples_fifo[array_pointer];
        correlation_b += og_samples_b4[i] * samples_fifo[array_pointer];
        correlation_c += og_samples_c4[i] * samples_fifo[array_pointer];
        correlation_d += og_samples_d4[i] * samples_fifo[array_pointer];
        correlation_e += og_samples_e4[i] * samples_fifo[array_pointer];
        correlation_f += og_samples_f4[i] * samples_fifo[array_pointer];
        correlation_g += og_samples_g4[i] * samples_fifo[array_pointer];

      }
    
      if((correlation_a > RECOGNITION_THRESHOLD) && (correlation_a > correlation_b) && (correlation_a > correlation_c) && (correlation_a > correlation_d) && (correlation_a > correlation_e) && (correlation_a > correlation_f) && (correlation_a > correlation_g)){
        PORTB = 0x20;
        printf("A %" PRIi32 "\n", correlation_a);
      }
      else if((correlation_b > RECOGNITION_THRESHOLD) && (correlation_b > correlation_c) && (correlation_b > correlation_d) && (correlation_b > correlation_e) && (correlation_b > correlation_f) && (correlation_b > correlation_g)){
        PORTB = 0x10;
        printf("B %" PRIi32 "\n", correlation_b);
      }
      else if((correlation_c > RECOGNITION_THRESHOLD) && (correlation_c > correlation_d) && (correlation_c > correlation_e) && (correlation_c > correlation_f) && (correlation_c > correlation_g)){
        PORTB = 0x08;
        printf("C %" PRIi32 "\n", correlation_c);
      }
      else if((correlation_d > RECOGNITION_THRESHOLD) && (correlation_d > correlation_e) && (correlation_d > correlation_f) && (correlation_d > correlation_g)){
        PORTB = 0x00;
        printf("D %" PRIi32 "\n", correlation_d);
      }
      else if((correlation_e > RECOGNITION_THRESHOLD) && (correlation_e > correlation_f) && (correlation_e > correlation_g)){
        PORTB = 0x00;
        printf("E %" PRIi32 "\n", correlation_e);
      }
      else if((correlation_f > RECOGNITION_THRESHOLD) && (correlation_f > correlation_g)){
        PORTB = 0x00;
        printf("F %" PRIi32 "\n", correlation_f);
      }
      else if((correlation_g > RECOGNITION_THRESHOLD)){
        PORTB = 0x00;
        printf("G %" PRIi32 "\n", correlation_g);
      }
      else
        PORTB = 0x00;

    }
  }


  printf("\n\n\n\n\n\n\n");

}
