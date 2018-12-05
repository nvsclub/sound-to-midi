#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//#define sram_starting_address (0x0100)
#define sram_starting_address (0x0200)
#define sram_ending_address (0x08FF)
#define sram_size 856 //in number of bytes
//#define sram_size 1024 //in number of bytes

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
uint8_t test_march_b(uint16_t sram_access[]){
  uint8_t no_errors = 0;
  uint16_t i;


  // increase address: w0
  for(i = 0; i < sram_size; i++){
    sram_access[i] = 0x00;
  }

  printf("loop 1 \n");

  // increase address: r0,w1,r1,w0,r0,w1
  for(i = 0; i < sram_size; i++){
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
  for(i = 0; i < sram_size; i++){
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
  for(i = sram_size - 1; i > 0; i--){
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
  for(i = sram_size - 1; i > 0; i--){
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
uint8_t test_march_c(uint16_t sram_access[]){
  uint8_t no_errors = 0;
  uint16_t i;

  // increase address: w0
  for(i = 0; i < sram_size; i++){
    sram_access[i] = 0x00;
  }

  printf("loop 6 \n");

  // increase address: r0,w1
  for(i = 0; i < sram_size; i++){
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
  for(i = 0; i < sram_size; i++){
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
  for(i = sram_size - 1; i > 0; i--){
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
  for(i = sram_size - 1; i > 0; i--){
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
  for(i = sram_size - 1; i > 0; i--){
    // r0
    if (sram_access[i] != 0x00){
      no_errors++;
      printf("%d \n", no_errors);
    }
  
  }

  printf("loop 11 \n");


  // returns 0 if no errors were found
  // else returns the number of errors found
  return no_errors;
}


/* running all memory tests */
uint8_t run_memory_test(){

  // the typecast (uint8_t *) converts the integer into an address
  return test_march_b((uint16_t *) sram_starting_address) + test_march_c((uint16_t *) sram_starting_address);

}