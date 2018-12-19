#include <avr/eeprom.h>

#define SIGNATURE (0xAA)

uint8_t eeprom_pointer[2];


/* Sets the signature for the uC
 * Returns 1 if the signature is different from the previous
 * Returns 0 if the signature was already there */
uint8_t sign_uc(){
  if(SIGNATURE != eeprom_read_byte(&eeprom_pointer[0])){
    eeprom_update_byte(&eeprom_pointer[0], SIGNATURE);
    return 1;

  }

  return 0;

}

/* Returns the signature on the chip */
uint8_t check_sign(){
  return eeprom_read_byte(&eeprom_pointer[0]);

}

/* Gets the hash save in the eeprom memory for the flash test */
uint8_t get_og_flash_hash(){
  return eeprom_read_byte(&eeprom_pointer[1]);

}


/* Use to update flash hash */
void update_og_flash_hash(uint8_t og_hash){
  eeprom_update_word(&eeprom_pointer[1], og_hash);
  return;

}
