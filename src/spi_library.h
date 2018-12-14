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

volatile uint8_t spi_address;
volatile uint8_t spi_databuffer[0xFF] = {0};

// SETS INITIALIZATION FOR MASTER DEVICE
void spi_init_master (void){

  // SETS MOSI, SS, SCK as OUTPUT
  DDRB = (1<<DDB2)|(1<<DDB3)|(1<<DDB5);
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
  SPDR = 0;
}

// SETS INITIALIZATION FOR SLAVE DEVICE
void spi_init_slave (void){

  // SET MISO AS OUTPUT
  DDRB = (1<<DDB4);
  SPCR = (1<<SPE)|(1<<SPIE)|(1<<SPR0)|(1<<SPR1);
  SPDR = 0;

  //SPCR ENABLES SPI COMMUNICATION
  //SPE   - SPI enable
}

// SENDS DATA TO REGISTER - MASTER AND SLAVE
void spi_trans (uint8_t data){

  SPDR = data;                  // Puts data on SPI register
  while(!(SPSR & (1<<SPIF)));   // Waits for Data Transfer
}

// RECEIVES DATA FROM REGISTER - MASTER AND SLAVE
unsigned char spi_receiv(void){
  return SPDR;
}

// MANAGE DATA RECEIVED FROM SPI CONNECTION
void spi_manage(uint8_t data){

  // Stores data in buffer
  if (spi_address < 0xFF){
    spi_databuffer[spi_address] = data;
    spi_address++;
  }
  else{
    spi_databuffer[spi_address] = data;
    spi_address = 0;
  }
}
