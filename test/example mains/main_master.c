#include "general_lib.h"


#define T1COUNT 65536-2500
// 0.1s = 25000 clock cycles @ 16MHz/64

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


ISR(TWI_vect){

  uint8_t data; // Temporary variable to store data
  ATOMIC_BLOCK(ATOMIC_FORCEON){

  // REQUEST TO ACKNOWLEDGE
  if (((TWSR & TW_NO_INFO) == TW_SR_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    TWCR |= (1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
  }
  // REQUEST TO TRANSMITT DATA
  else if (((TWSR & TW_NO_INFO) == TW_ST_SLA_ACK) || ((TWSR & TW_NO_INFO) == TW_SR_ARB_LOST_SLA_ACK)){
    data = TWDR;
    i2c_slave_trans(data);
  }

  // REQUEST TO RECEIVE DATA
  else if ((TWSR & TW_NO_INFO) == TW_SR_DATA_ACK){
    data = TWDR;
    turnLeds(data);
    i2c_slave_receive(data);
  }

  else{
    TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
  }
  }
}

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

  _delay_ms(500);

  // MAIN INITIALIZATIONS
  spi_init_master();    // SPI Initialization as master
  i2c_init_slave();     // I2C Initialization as slave
  init_printf_tools();  // Initialize Prints
  init_adc();           // Initialize ADC
  init_timer();         // Initialize timers
  // REDUNDANT IN THE TIMERS - sei();                // Initializes Interrupt


  uint8_t microphone_msg;
  uint8_t pr_toggle = 0;
  i2c_address_receiv = 0;

  DDRD = 0xFF;

  _delay_ms(1000);
  while(1){
    if(pr_toggle != toggle){
      pr_toggle = toggle;

      // Get message from microphone
      microphone_msg = read_adc(0);
      // Transfer microphone message using SPI
      spi_trans(microphone_msg);

    }
  }

}
