#include "general_lib.h"

////////////////////////////////
//  SPI DESCRIPTION VARIABLES //
////////////////////////////////

// Any Variable related to the Spi SPI library
// can be check below for meaning

//SPE   - SPI Enable
//MSTR  - MASTER SELECT
//SPR0  - SPI Clock Rate Selection
//SPDR  - SPI Data Register
//SPCR  - SPI Control Register
//SPIF  - SPI Interrupt Flag
//SPSR  - SPI Status Register
//SPIE  - SPI INTERRUPT ENABLE


// SETS INITIALIZATION FOR MASTER DEVICE
void spi_init_master (void){

  // SETS MOSI, SS, SCK as OUTPUT
  DDRB = (1<<DDB2)|(1<<DDB3)|(1<<DDB5);
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
  SPDR = 0;
}

// SETS INITIALIZATION FOR SLAVE DEVICE
void spi_init_slave (void){

  // SET MISO AS OUTPUT
  DDRB = (1<<DDB4);
  SPCR = (1<<SPE)|(1<<SPIE);
  SPDR = 0;

  //SPCR ENABLES SPI COMMUNICATION
  //SPE   - SPI enable
}

// SENDS DATA TO REGISTER - MASTER AND SLAVE
void spi_trans (unsigned char data){

  SPDR = data;                  // Puts data on SPI register
  while(!(SPSR & (1<<SPIF)));   // Waits for Data Transfer
}

// RECEIVES DATA FROM REGISTER - MASTER AND SLAVE
unsigned char spi_receiv(void){
  return SPDR;
}
