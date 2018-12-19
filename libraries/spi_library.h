#include <avr/io.h>

/*
 *
 * SPI DESCRIPTION VARIABLES
 *
 *
 * Any Variable related to the Spi SPI library
 * can be check below for meaning
 *
 * MSTR  - MASTER SELECT
 * SPR0  - SPI Clock Rate Selection
 * SPDR  - SPI Data Register
 * SPCR  - SPI Control Register
 * SPIF  - SPI Interrupt Flag
 * SPSR  - SPI Status Register
 * SPIE  - SPI INTERRUPT ENABLE
 * */

/* SETS INITIALIZATION FOR MASTER DEVICE */
void spi_init_master (void);

/* SETS INITIALIZATION FOR SLAVE DEVICE */
void spi_init_slave (void);

/* SENDS DATA TO REGISTER - MASTER AND SLAVE */
void spi_trans (uint8_t data);

/* RECEIVES DATA FROM REGISTER - MASTER AND SLAVE */
unsigned char spi_receiv(void);

/* MANAGE DATA RECEIVED FROM SPI CONNECTION */
void spi_manage(uint8_t data, uint8_t spi_address, uint8_t spi_databuffer[254]);
