
/* Sets the signature for the uC
 * Returns 1 if the signature is different from the previous
 * Returns 0 if the signature was already there */
uint8_t sign_uc();

/* Returns the signature on the chip */
uint8_t check_sign();

/* Gets the hash save in the eeprom memory for the flash test */
uint8_t get_og_flash_hash();

/* Use to update flash hash */
void update_og_flash_hash();
