#include <avr/eeprom.h>

#define SIGNATURE 0x7B

uint8_t eeprom_pointer[2];


// Sets the signature for the uC
/* Returns 1 if the signature is different from the previous
   Returns 0 if the signature was already there */
uint8_t sign_uc(){
  if(SIGNATURE != eeprom_read_byte(&eeprom_pointer)){
    eeprom_update_byte(&eeprom_pointer[0], SIGNATURE);
    return 1;

  }

  return 0;

}

uint8_t check_sign(){
  return eeprom_read_byte(&eeprom_pointer[0]);
  
}

// Gets the hash save in the eeprom memory for the flash test
uint8_t get_og_flash_hash(){
  return eeprom_read_byte(&eeprom_pointer[1]);

}


// Use to update flash hash
/* The flash hash default value is 0x00, so change it back
after updating the hash in eeprom */
void update_og_flash_hash(){
  eeprom_update_byte(&eeprom_pointer[1], 0x00);
  return;

}
