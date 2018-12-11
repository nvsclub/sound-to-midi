#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

#define sram_starting_address (0x0100)
#define sram_ending_address (0x08FF)
#define sram_size 1024U    //in number of bytes
#define sram_divisions 8U
#define sram_block_size sram_size/sram_divisions

// Defining global variables
static uint8_t sram_buffer[sram_block_size];


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
uint8_t test_march_b(uint8_t sram_access[]){
  uint8_t no_errors = 0;
  uint8_t i;

  for(uint8_t j=0; j < sram_block_size; j++){
      sram_buffer[j] = sram_access[j];
      printf("copyinn %d %x \n", sram_access[j], &sram_access[j]);
  }


  printf("%x, %d \n", SP, SP);
  // Move the stack pointer
  SPL = ((uint16_t) &sram_buffer[0]);
  SPH = ((uint16_t) &sram_buffer[0]) >> 8;
  printf("%x, %d \n", &sram_buffer[0], &sram_buffer[0]);
  printf("%x, %d \n", SP, SP);

  // increase address: w0
  for(i = 0; i < sram_block_size; i++){
    printf("mem %x %x", sram_access[i], &sram_access[i]);
    sram_access[i] = 0x00;
    printf(" %x \n", sram_access[i]);
  }

  printf("loop 1 \n");

  // increase address: r0,w1,r1,w0,r0,w1
  for(i = 0; i < sram_block_size; i++){
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w1
    sram_access[i] = 0xFF;
  
    // r1
    if (sram_access[i] != 0xFF){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w0
    sram_access[i] = 0x00;
  
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }
  
    // w1
    sram_access[i] = 0xFF;
  
  }

  printf("loop 2 \n");

  // increase address: r1,w0,w1
  for(i = 0; i < sram_block_size; i++){
    // r1
    if (sram_access[i] != 0xFF){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w0
    sram_access[i] = 0x00;
  
    // w1
    sram_access[i] = 0xFF;
  
  }

  printf("loop 3 \n");

  // decrease address: r1,w0,w1,w0
  for(i = sram_block_size - 1; i > 0; i--){
    // r1
    if (sram_access[i] != 0xFF){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w0
    sram_access[i] = 0x00;
  
    // w1
    sram_access[i] = 0xFF;
  
    // w0
    sram_access[i] = 0x00;
    
  }

  printf("loop 4 \n");

  // decrease address: r0,w1,w0
  for(i = sram_block_size - 1; i > 0; i--){
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w1
    sram_access[i] = 0xFF;
  
    // w0
    sram_access[i] = 0x00;

  }

  printf("loop 5 \n");

  for(uint8_t j=0; j < sram_block_size; j++){
    sram_access[j] = sram_buffer[j];
    printf("copinnn baaaaaaaack");
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
uint8_t test_march_c(uint8_t sram_access[]){
  uint8_t no_errors = 0;
  uint8_t i;

  for(uint8_t j=0; j < sram_block_size; j++){
    sram_buffer[j] = sram_access[j]; 
  }

  // increase address: w0
  for(i = 0; i < sram_block_size; i++){
    sram_access[i] = 0x00;
  }

  printf("loop 6 \n");

  // increase address: r0,w1
  for(i = 0; i < sram_block_size; i++){
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w1
    sram_access[i] = 0xFF;
  
  }

  printf("loop 7 \n");

  // increase address: r1,w0
  for(i = 0; i < sram_block_size; i++){
    // r1
    if (sram_access[i] != 0xFF){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w0
    sram_access[i] = 0x00;
  
  }

  printf("loop 8 \n");

  // decrease address: r0,w1
  for(i = sram_block_size - 1; i > 0; i--){
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w1
    sram_access[i] = 0xFF;

  }

  printf("loop 9 \n");

  // decrease address: r1,w0
  for(i = sram_block_size - 1; i > 0; i--){
    // r1
    if (sram_access[i] != 0xFF){
      no_errors++;
      printf("%d \n", no_errors);
    }

    // w0
    sram_access[i] = 0x00;
  
  }

  printf("loop 10 \n");

  // decrease address: r1,w0
  for(i = sram_block_size - 1; i > 0; i--){
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }
  
  }

  printf("loop 11 \n");

  for(uint8_t j=0; j < sram_block_size; j++){
    sram_access[j] = sram_buffer[j]; 
  }


  // returns 0 if no errors were found
  // else returns the number of errors found
  return no_errors;
}


/* running all memory tests */
uint8_t test_sram_memory(){
  uint8_t no_errors = 0;
  
  for(uint8_t i=0; i < sram_divisions; i++){
    no_errors += test_march_b((uint8_t *) (sram_starting_address + i*(sram_block_size)));
    no_errors += test_march_c((uint8_t *) (sram_starting_address + i*(sram_block_size)));

  }
  
  return no_errors;

}