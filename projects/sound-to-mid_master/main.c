#include "general_lib.h"

#define T1COUNT 65536-2500
/* 0.1s = 25000 clock cycles @ 16MHz/64 */

#define DEBUG_PRINTF 0
#define SETUP_HASH 0
/* only enable flash test when both DEBUG_PRINTF and SETUP_HASH are 0
 * and in mem_sram_test DEBUG_PRINTF is 0 and DODGE_STACK_SECTION is 1
 */
#define ENABLE_FLASH_TEST 1

uint8_t volatile toggle = 0;

void init_timer(){
  /*   Disable Interrupts */
  cli();

  /*   Normal Mode */
  TCCR1A=0x00;
  /*   Stop timer */
  TCCR1B=0x00;
  /*   Setup timer counter */
  TCNT1 = T1COUNT;

  /*   Activate interrupts */
  TIMSK1=0x01;
  CLKPR=0x00;

  /*   Enable interrupts */
  sei();

  /*   Start the timer counter */
  TCCR1B = 0b00000011;

}

ISR(TIMER1_OVF_vect){
  /*   Reload the number of cycles needed */
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


ISR(TWI_vect){

  uint8_t data; /*   Temporary variable to store data */
  ATOMIC_BLOCK(ATOMIC_FORCEON){

  /*   REQUEST TO ACKNOWLEDGE */
  if (((TWSR & TW_NO_INFO) == TW_SR_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    TWCR |= (1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
  }
  /*   REQUEST TO TRANSMITT DATA */
  else if (((TWSR & TW_NO_INFO) == TW_ST_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    data = TWDR;
    i2c_slave_trans(data, i2c_address_send, i2c_databuffer);
  }

  /*   REQUEST TO RECEIVE DATA */
  else if ((TWSR & TW_NO_INFO) == TW_SR_DATA_ACK){
    data = TWDR;
    i2c_slave_receive(data, i2c_address_receiv, i2c_databuffer);
    if (I2C_flag == 1){
      turnLeds(data);
    }
  }

  else{
    TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
  }
  }
}

/*  MODULE FUNCTION
 * This Module will collect data from microphone
 * and then send it using spi protocol
 * When called (interrupted) it will receve a MIDI message by
 * I2C protocol and update LEDS's according to message
 *
 * THIS MODULE IS:
 * SPI Master
 * I2C Slave */

/* MAIN FUNCTION */
int main(void){

#if(1 == SETUP_HASH)
  update_og_flash_hash(0x0E);
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

  _delay_ms(500);

  /* MAIN INITIALIZATIONS */
  spi_init_master();    /* SPI Initialization as master */
  i2c_init_slave();     /* I2C Initialization as slave */
  init_adc();           /* Initialize ADC */
  init_timer();         /* Initialize timer */

  /* GENERAL VARIABLES INITIALIZATION */
  uint8_t microphone_msg[254];
  uint8_t pr_toggle = 0;
  uint8_t counter = 0;
  uint8_t sample_counter = 1;
  i2c_address_receiv = 0;

  /* LED PORT INITIALIZATION */
  DDRD = 0xFF;
  PORTD = 0x00;

  _delay_ms(500);

  /* MAIN CODE */
  while(1){

    /* COLLECT DATA FROM MICROPHONE */
    if(pr_toggle != toggle){
      pr_toggle = toggle;

      if ((read_adc(0) > 170) && I2C_flag == 0){
#if(1 == DEBUG_PRINTF)
        printf("COLLECTING SAMPLE: %d\n", sample_counter);
#endif
        while(counter < 255){
          if(pr_toggle != toggle){
            pr_toggle = toggle;
            microphone_msg[counter] = read_adc(0);
            counter++;
          }
        }
      }
    }

    if (counter == 255){

      /* SEND COLLECTED DATA FROM MICROPHONE */
#if(1 == DEBUG_PRINTF)
      printf("SENDING SAMPLE: %d\n", sample_counter);
#endif
      for (uint8_t i = 0; i < 255; i++){
        spi_trans(microphone_msg[i]);
      }
      I2C_flag = 1;
      /* WAIT FOR NEW CYCLE */
      counter = 0;
      sample_counter++;
    }
  }

}
