#include "i2c_library.h"

/* INITIALIZES MASTER FOR I2C CONNECTION */
void i2c_init_master(void){

  TWSR = 0x00;                /* SETS REGISTER TO 0 */
  TWBR = (uint8_t) TWBR_val;  /* CHECK DEFINES */

}

/* INITIALIZES MASTER FOR I2C CONNECTION */
void i2c_init_slave(void){

  TWAR = I2C_ADDRESS;     /* SETS SLAVE ADDRESS */
  TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWINT)|(1<<TWIE);

}


/* STOPS MASTER CONNECTION */
void i2c_stop_master(void){
  
  /* STOPS TRANSMISSION */
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

}

/* STOPS SLAVE CONNECTION */
void i2c_stop_slave(void){

  TWCR &= ~((1<<TWEA)|(1<<TWEN));

}

/* START I2C COMMUNICATION BETWEEN MODULES - MASTER ONLY */
uint8_t i2c_start(uint8_t address, uint8_t mode){

  /* START CONDITION TO INITIALIZE COMUNICATION
   * MODE DEFINES REPEATED START OR NORMAL
   * MODE == 0x00 - First MAIN Start
   * MODE == 0x01 - Use between MAIN Start and Stop
   * RETURNS 0 - BAD or 1 - GOOD */

  /* Sends Start Condition */
  TWCR = 0;
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  while(!(TWCR & (1<<TWINT)));

  /* Return 0 if START Condition not Transmitted */
  if (mode == 0x00){
    if((TWSR & TW_NO_INFO) != TW_START){
      return 0x00;
    }
  }

  /* Return 0 if START Condition not Transmitted */
  if (mode == 0x01){
    if((TWSR & TW_NO_INFO) != TW_REP_START){
      return 0x00;
    }
  }

  /* Sends Address to Communicate*/
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while(!(TWCR & (1<<TWINT)));

  /* Return 1 if Address + W/R Condition Transmitted and Acknowledge */
  if((TWSR & TW_NO_INFO) == TW_MT_SLA_ACK){
    return 0x01;
  }

  /* Return 2 if Address + W/R Condition Transmitted and not Acknowledge */
  if((TWSR & TW_NO_INFO) == TW_MT_SLA_NACK){
    return 0x02;
  }

  else  return 0x03;
}

/* WRITES DATA TO SLAVE - MASTER ONLY */
uint8_t i2c_write(uint8_t data){

  /* Sends Data to Register */
  TWDR = data;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while(!(TWCR & (1<<TWINT)));

  /* Return 1 if Address + W/R Condition Transmitted and Acknowledge */
  if((TWSR & TW_NO_INFO) == TW_MT_DATA_ACK){  /* Wait for End */
    return 0x01;
  }

  /* Return 2 if Address + W/R Condition Transmitted and not Acknowledge */
  if((TWSR & TW_NO_INFO) == TW_MT_SLA_NACK){
    return 0x02;
  }

  return 0x03;
}

/* TRASMITTS DATA FROM BUFFER - SLAVE ONLY */
void i2c_slave_trans(uint8_t data, uint8_t i2c_address_send, uint8_t i2c_databuffer[255]){

  /* CHECKS IF BUFFER IS EMPTY */
  if (i2c_address_send == 0x00){
    TWDR = 0xFF;
  }

  /* SENDS DATA IN BUFFER WITH DEFINED ADDRESS*/
  else{
    TWDR = i2c_databuffer[i2c_address_send];
    i2c_address_send++;
  }

  /* CHECKS IF BUFFER CAN ACCEPT MORE DATA */
  if(i2c_address_send < 0xFF){
    TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
  }

  /* IF BUFFER FULL SENDS NO ACKNOWLEDGE MESSAGE */
  else{
    TWCR &= ~(1<<TWEA);
    TWCR |= (1<<TWIE)|(1<<TWINT)|(1<<TWEN);
  }
}

/* READS DATA FROM SLAVE WITH ACKNOWLEDGE - MASTER ONLY */
uint8_t i2c_read_w_ack(void){

  /* Reads data requested from slave */
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
  while( !(TWCR & (1<<TWINT)));
  return TWDR;
}

/* READS DATA FROM SLAVE WITHOUT ACKNOWLEDGE - MASTER ONLY */
uint8_t i2c_read_w_nack(void){

  /* READ data requested from slave */
  TWCR = (1<<TWINT) | (1<<TWEN);
  while( !(TWCR & (1<<TWINT)));
  return TWDR;
}

/* READS DATA FROM SLAVE WITH ACKNOWLEDGE - SLAVE ONLY */
void i2c_slave_receive(uint8_t data, uint8_t i2c_address_receiv, uint8_t i2c_databuffer[255]){

  /* CHECKS IF BUFFER IS EMPTY */
  if (i2c_address_receiv < 0xFF){
    i2c_databuffer[i2c_address_receiv] = data;
    i2c_address_receiv++;
    TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
  }

  else{
    i2c_databuffer[i2c_address_receiv] = data;
    i2c_address_receiv = 0;
    TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
  }
}

void i2c_handler(uint16_t midi_msg){
  /* Start i2c protocol to send 1st byte of MIDI message */
  i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
  uint8_t split = ((midi_msg & 0xFF00) >> 8);

  /* Send 1st byte of MIDI message */
  i2c_write(split);
  i2c_stop_master();

  _delay_ms(5);

  i2c_start(I2C_ADDRESS & I2C_WRITE, 0x00);
  split = (midi_msg & 0x00FF);
  /* Send 1st byte of MIDI message */
  i2c_write(split);
  i2c_stop_master();
}
