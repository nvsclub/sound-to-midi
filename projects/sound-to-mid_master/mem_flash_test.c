#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#define no_bytes_flashed 9684U

/* Calculates an hash of the program binary
 * by doing the bitwise XOR between every byte~
 */
uint8_t test_flash_memory(uint8_t og_code_hash){
  uint8_t code_hash = 0;
  uint8_t line;


  for(uint16_t i = 0; i < no_bytes_flashed; i++){
    /* Access binary in the flash */
    line = pgm_read_byte_near(i);
    code_hash ^= line;
  }

  /* Compares the hashes and returns the result */
  /* If everything is ok */
  if(code_hash == og_code_hash){
    return 0;
  }
  /* If something is wrong */
  else{
    return 1;
  }

}
