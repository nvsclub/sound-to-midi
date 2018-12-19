/* This file has been prepared for Doxygen automatic documentation generation.*/
/**
 * \file
 *
 * \brief Settings for the SRAM test.
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

#ifndef _SRAM_H_
#define _SRAM_H_

#include <avr/io.h>
#define CLASSB_NSECS 8

#define INTERNAL_SRAM_START (0x0100)
#define INTERNAL_SRAM_SIZE (2048U)

#define CLASSB_OVERLAP 25UL


#define CLASSB_SEC_SIZE (INTERNAL_SRAM_SIZE / CLASSB_NSECS)

#define CLASSB_SEC_REM (INTERNAL_SRAM_SIZE % CLASSB_NSECS)

#define CLASSB_OVERLAP_SIZE (CLASSB_SEC_SIZE*CLASSB_OVERLAP)/100

#if (CLASSB_SEC_REM == 0)
#  define CLASSB_NSEC_TOTAL CLASSB_NSECS
#else
#  define CLASSB_NSEC_TOTAL CLASSB_NSECS + 1
#endif

uint8_t sram_test();

uint8_t classb_sram_test( void );

uint8_t classb_marchX(register volatile uint8_t * p_sram, register volatile uint8_t * p_buffer, register uint16_t size);

#endif
