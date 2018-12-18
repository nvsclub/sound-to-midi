#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>


#include <util/delay.h>


#define sram_starting_address (0x0100)
#define sram_safe_starting_address (0x04FF)
#define sram_ending_address (0x08FF)
#define sram_size 2048U
#define sram_safe_size 1024U
#define sram_divisions 8U
#define sram_overlap 25U
#define sram_block_size sram_size/sram_divisions
#define sram_block_rest sram_size%sram_divisions
#define sram_block_overlap sram_block_size*sram_overlap/100
#define sram_safe_block_size sram_safe_size/sram_divisions
#define sram_safe_block_rest sram_safe_size%sram_divisions
#define sram_safe_block_overlap sram_safe_block_size*sram_overlap/100

#if(sram_block_rest == 0)
# define sram_block_total sram_divisions
# define sram_safe_block_total sram_divisions
#else
# define sram_block_total sram_divisions + 1
# define sram_safe_block_total sram_divisions + 1
#endif

// Defining global variables
static uint8_t sram_buffer[sram_block_size+sram_block_overlap]; 


/*
The combination of the march b and march c- test cover all errors in memory
*/

/*
Sequence:
any order:        w0
increase address: r0,w1,r1,w0,r0,w1
increase address: r1,w0,w1
decrease address: r1,w0,w1,w0
decrease address: r0,w1,w0
*/
uint8_t test_march_b(register volatile uint8_t *sram_access, register volatile uint8_t *sram_buffer, register uint16_t size){
  register uint8_t no_errors = 0;
  register uint16_t i;

  if(sram_buffer != sram_access){
    for(uint16_t i=0; i < size; i++){
        *(sram_buffer+i) = *(sram_access+i);
  
    }
  }

  //printf(" Starting test B from %d to %d  ", (sram_access), (sram_access+size));

  // increase address: w0
  for(i = 0; i < size; i++){
    *(sram_access+i) = 0x00;

  }

  // increase address: r0,w1,r1,w0,r0,w1
  for(i = 0; i < size; i++){
    // r0
    if (*(sram_access+i) != 0x00)
      no_errors++;

    // w1
    else
      *(sram_access+i) = 0xFF;
  
    // r1
    if (*(sram_access+i) != 0xFF)
      no_errors++;

    // w0
    else
      *(sram_access+i) = 0x00;
  
    // r0
    if (*(sram_access+i) != 0x00)
      no_errors++;
  
    // w1
    else
      *(sram_access+i) = 0xFF;
  
  }

  // increase address: r1,w0,w1
  for(i = 0; i < size; i++){
    // r1
    if (*(sram_access+i) != 0xFF)
      no_errors++;

    // w0
    // w1
    else{
      *(sram_access+i) = 0x00;
      *(sram_access+i) = 0xFF;
    }
  
  
  }

  // decrease address: r1,w0,w1,w0
  for(i = size - 1; i > 0; i--){
    // r1
    if (*(sram_access+i) != 0xFF)
      no_errors++;

    // w0
    // w1
    // w0
    else{
      *(sram_access+i) = 0x00;
      *(sram_access+i) = 0xFF;
      *(sram_access+i) = 0x00;
    }
  
    
  }

  // decrease address: r0,w1,w0
  for(i = size - 1; i > 0; i--){
    // r0
    if (*(sram_access+i) != 0x00)
      no_errors++;

    // w1
    // w0
    else{
      *(sram_access+i) = 0xFF;
      *(sram_access+i) = 0x00;
    }

  }

  //printf(" Ending test B from %d to %d  \n", (sram_access), (sram_access+size));

  if(sram_buffer != sram_access){
    for(i=0; i < size; i++){
        *(sram_access+i) = *(sram_buffer+i);
  
    }

  }

  // returns 0 if no errors were found
  // else returns the number of errors found
  return no_errors;
}

/*
Sequence:
any order:        w0
increase address: r0,w1
increase address: r1,w0
decrease address: r0,w1
decrease address: r1,w0
any order:        r0
*/
uint8_t test_march_c(register volatile uint8_t *sram_access, register volatile uint8_t *sram_buffer, register uint16_t size){
  register uint8_t no_errors = 0;
  register uint8_t i;

  if(sram_buffer != sram_access){
    for(i=0; i < size; i++){
        *(sram_buffer+i) = *(sram_access+i);
  
    }

  }

  ////printf(" Starting test C from %d to %d  ", *(sram_access), *(sram_access+size));

  // increase address: w0
  for(i = 0; i < size; i++){
    *(sram_access+i) = 0x00;
  
  }


  // increase address: r0,w1
  for(i = 0; i < size; i++){
    // r0
    if (*(sram_access+i) != 0x00)
      no_errors++;

    // w1
    else
      *(sram_access+i) = 0xFF;
  
  }

  // increase address: r1,w0
  for(i = 0; i < size; i++){
    // r1
    if (*(sram_access+i) != 0xFF)
      no_errors++;

    // w0
    else
      *(sram_access+i) = 0x00;
  
  }

  // decrease address: r0,w1
  for(i = size - 1; i > 0; i--){
    // r0
    if (*(sram_access+i) != 0x00)
      no_errors++;

    // w1
    else
      *(sram_access+i) = 0xFF;

  }

  // decrease address: r1,w0
  for(i = size - 1; i > 0; i--){
    // r1
    if (*(sram_access+i) != 0xFF)
      no_errors++;

    // w0
    else
      *(sram_access+i) = 0x00;
  
  }

  // decrease address: r0
  for(i = size - 1; i > 0; i--){
    // r0
    if (*(sram_access+i) != 0x00)
      no_errors++;
  
  }

  ////printf(" Ending test C from %d to %d  ", *(sram_access), *(sram_access+size));

  if(sram_buffer != sram_access){
    for(i=0; i < size; i++){
        *(sram_access+i) = *(sram_buffer+i);
  
    }

  }

  // returns 0 if no errors were found
  // else returns the number of errors found
  return no_errors;
}


/* running all memory tests */
void test_sram_memory(){
  register uint8_t current_section;
  register uint8_t no_errors = 0;
  

  // Testing the safe zone
  /*for(current_section = 0; current_section < sram_block_total; current_section++){
    ////printf("\nCurrent section: %x \n", current_section);
    switch(current_section){
      case sram_safe_block_total:
        no_errors += test_march_b((uint8_t *) sram_safe_starting_address + sram_divisions * sram_safe_block_size - sram_safe_block_overlap, sram_buffer, sram_safe_block_rest + sram_safe_block_overlap);
        no_errors += test_march_c((uint8_t *) sram_safe_starting_address + sram_divisions * sram_safe_block_size - sram_safe_block_overlap, sram_buffer, sram_safe_block_rest + sram_safe_block_overlap);
        break;
      default:
        no_errors += test_march_b((uint8_t *) sram_safe_starting_address + current_section * sram_safe_block_size - sram_safe_block_overlap, sram_buffer, sram_safe_block_size + sram_safe_block_overlap);
        no_errors += test_march_c((uint8_t *) sram_safe_starting_address + current_section * sram_safe_block_size - sram_safe_block_overlap, sram_buffer, sram_safe_block_size + sram_safe_block_overlap);
        break;
      
    }

  }*/

  for(current_section = 0; current_section < sram_block_total; current_section++){
    ////printf("\nCurrent section: %x \n", current_section);

    switch(current_section){
      // test the buffer
      case 0:
        no_errors += test_march_b((uint8_t *) sram_starting_address, sram_buffer, sram_block_size + sram_block_overlap);
        no_errors += test_march_c((uint8_t *) sram_starting_address, sram_buffer, sram_block_size + sram_block_overlap);
        break;
      case 1:
        no_errors += test_march_b((uint8_t *) sram_starting_address + sram_block_size, sram_buffer, sram_block_size);
        no_errors += test_march_c((uint8_t *) sram_starting_address + sram_block_size, sram_buffer, sram_block_size);
        break;
      case sram_block_total:
        no_errors += test_march_b((uint8_t *) sram_starting_address + sram_divisions * sram_block_size - sram_block_overlap, sram_buffer, sram_block_rest + sram_block_overlap);
        no_errors += test_march_c((uint8_t *) sram_starting_address + sram_divisions * sram_block_size - sram_block_overlap, sram_buffer, sram_block_rest + sram_block_overlap);
        break;
      default:
        no_errors += test_march_b((uint8_t *) sram_starting_address + current_section * sram_block_size - sram_block_overlap, sram_buffer, sram_block_size + sram_block_overlap);
        no_errors += test_march_c((uint8_t *) sram_starting_address + current_section * sram_block_size - sram_block_overlap, sram_buffer, sram_block_size + sram_block_overlap);
        break;
      
    }
  
  return;
  }
}