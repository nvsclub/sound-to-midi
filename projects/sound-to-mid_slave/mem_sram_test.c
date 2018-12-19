/* This file has been prepared for Doxygen automatic documentation generation.*/
/**
 * \file
 *
 * \brief
 *		SRAM test based on the March X algorithm.
 *
 * \par Application note:
 *      AVR1610: Guide to IEC60730 Class B compliance with XMEGA
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "mem_sram_test.h"

static uint8_t classb_buffer[CLASSB_SEC_SIZE+CLASSB_OVERLAP_SIZE] __attribute__ ((section (".classb_sram_buffer")));

#define DEBUG_PRINTF 0
#define DODGE_STACK_SECTION 1
#define SRAM_FAIL_SIMULATION 0

uint8_t sram_test(){
	for(uint8_t i = 0; i < CLASSB_NSEC_TOTAL; i++)
	{
		if(classb_sram_test())
		{
			#if(1==DEBUG_PRINTF)
				printf("SRAM ERROR @ Block %d\n",i);
			#endif
			return i+1;
		}
		#if(1==DEBUG_PRINTF)
			printf("SRAM OK @ Block %d\n",i);
		#endif

	}
	return 0;
}

uint8_t classb_sram_test(){

	static uint8_t current_section = 0;
	uint8_t error = 0;

#if(1 == DODGE_STACK_SECTION)
	switch (current_section){
		case 0:
			/* Buffer test */
			error = classb_marchX((uint8_t *)INTERNAL_SRAM_START, classb_buffer, CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE);
			break;
		case 1:
			/* Test the first section (without overlaps) */
			error = classb_marchX((uint8_t *)INTERNAL_SRAM_START + CLASSB_SEC_SIZE, classb_buffer, CLASSB_SEC_SIZE);
			break;
		case CLASSB_NSECS:
			/* This cases avoids testing the stack area of the sram which leads to bad behaviour */
			break;
		case CLASSB_NSECS-1:
			break;
		case CLASSB_NSECS-2:
			break;
		default:
			/* Test all the sections in the middle */
			error = classb_marchX((uint8_t *)INTERNAL_SRAM_START + current_section * CLASSB_SEC_SIZE - CLASSB_OVERLAP_SIZE, classb_buffer, CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE);
			break;
		}
#endif

#if(0 == DODGE_STACK_SECTION)
	switch (current_section){
	case 0:
		/* Buffer test */
		error = classb_marchX((uint8_t *)INTERNAL_SRAM_START, classb_buffer, CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE);
		break;
	case 1:
		/* Test the first section (without overlaps) */
		error = classb_marchX((uint8_t *)INTERNAL_SRAM_START + CLASSB_SEC_SIZE, classb_buffer, CLASSB_SEC_SIZE);
		break;
	case CLASSB_NSECS:
		/* Test the last section with variable remaining addresses */
		error = classb_marchX((uint8_t *)INTERNAL_SRAM_START + CLASSB_NSECS * CLASSB_SEC_SIZE - CLASSB_OVERLAP_SIZE, classb_buffer, CLASSB_SEC_REM + CLASSB_OVERLAP_SIZE);
		break;
	default:
		/* Test all the sections in the middle */
		error = classb_marchX((uint8_t *)INTERNAL_SRAM_START + current_section * CLASSB_SEC_SIZE - CLASSB_OVERLAP_SIZE, classb_buffer, CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE);
		break;
	}
#endif

	/* Pass over to a new section */
	current_section++;
	if (current_section > CLASSB_NSEC_TOTAL-1)
		current_section = 0;

	/* Returns 0 if there are no errors, or the number of sections with errors if they exist */
	return error;

}

uint8_t classb_marchX(register volatile uint8_t * p_sram, register volatile uint8_t * p_buffer, register uint16_t size){
	register uint16_t i = 0;
	register uint8_t error = 0;

	/* Copy the ram content into a buffer */
	if (p_buffer != p_sram)
		for (uint16_t i = 0; i < size; i++)
			*(p_buffer+i) = *(p_sram+i);


#if(1 == SRAM_FAIL_SIMULATION)
  error++;

#endif


	/*
	 *
	 * TEST MARCH B
	 *
	 *
	 * */

	/* increase address: w0 */
	  for(i = 0; i < size; i++){
	    *(p_sram+i) = 0x00;

	  }

	  /* increase address: r0,w1,r1,w0,r0,w1*/
	  for(i = 0; i < size; i++){
	    /* r0 */
	    if (*(p_sram+i) != 0x00)
	      error++;

	    /* w1 */
	    else
	      *(p_sram+i) = 0xFF;

	    /* r1 */
	    if (*(p_sram+i) != 0xFF)
	      error++;

	    /* w0 */
	    else
	      *(p_sram+i) = 0x00;

	    /* r0 */
	    if (*(p_sram+i) != 0x00)
	      error++;

	    /* w1 */
	    else
	      *(p_sram+i) = 0xFF;

	  }

	  /* increase address: r1,w0,w1 */
	  for(i = 0; i < size; i++){
	    /* r1 */
	    if (*(p_sram+i) != 0xFF)
	      error++;

	    /* w0
	     * w1 */
	    else{
	      *(p_sram+i) = 0x00;
	      *(p_sram+i) = 0xFF;
	    }


	  }

	  /* decrease address: r1,w0,w1,w0 */
	  for(i = size - 1; i > 0; i--){
	    /* r1 */
	    if (*(p_sram+i) != 0xFF)
	      error++;

	    /* w0
	     * w1
	     * w0 */
	    else{
	      *(p_sram+i) = 0x00;
	      *(p_sram+i) = 0xFF;
	      *(p_sram+i) = 0x00;
	    }


	  }

	  /* decrease address: r0,w1,w0 */
	  for(i = size - 1; i > 0; i--){
	    /* r0 */
	    if (*(p_sram+i) != 0x00)
	      error++;

	    /* w1
	     * w0 */
	    else{
	      *(p_sram+i) = 0xFF;
	      *(p_sram+i) = 0x00;
	    }

	  }

	/*
	 *
	 * TEST MARCH C-
	 *
	 *
	 * */

	  /* increase address: w0 */
	    for(i = 0; i < size; i++){
	      *(p_sram+i) = 0x00;

	    }


	    /* increase address: r0,w1 */
	    for(i = 0; i < size; i++){
	      /* r0 */
	      if (*(p_sram+i) != 0x00)
	        error++;

	      /* w1 */
	      else
	        *(p_sram+i) = 0xFF;

	    }

	    /* increase address: r1,w0 */
	    for(i = 0; i < size; i++){
	      /* r1 */
	      if (*(p_sram+i) != 0xFF)
	        error++;

	      /* w0 */
	      else
	        *(p_sram+i) = 0x00;

	    }

	    /* decrease address: r0,w1 */
	    for(i = size - 1; i > 0; i--){
	      /* r0 */
	      if (*(p_sram+i) != 0x00)
	        error++;

	      /* w1 */
	      else
	        *(p_sram+i) = 0xFF;

	    }

	    /* decrease address: r1,w0 */
	    for(i = size - 1; i > 0; i--){
	      /* r1 */
	      if (*(p_sram+i) != 0xFF)
	        error++;

	      /* w0 */
	      else
	        *(p_sram+i) = 0x00;

	    }

	    /* decrease address: r0 */
	    for(i = size - 1; i > 0; i--){
	      /* r0 */
	      if (*(p_sram+i) != 0x00)
	        error++;

	    }


	/* Restore the ram with the buffer content */
	if (p_buffer != p_sram)
		for (i = 0; i < size; i++)
			*(p_sram+i) = *(p_buffer+i);

	/* Return 1 if there was any error or 0 if none existed */
	if (error)
		return 1;
	else return 0;

}
