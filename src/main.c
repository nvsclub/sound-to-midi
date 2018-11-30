/*
 *  pisca.c
 *  A blink led demo written in plain C
 *  Created on: 1/09/2014
 *      Author: jpsousa@fe.up.pt
 */

/*

DDDDDDDD

*/

#include <avr/io.h>
#include <util/delay.h>

int main (void){

  DDRB = 0xFF;	/* set PORT B as output */

  while(1) {
    PORTB = 0xFF;		/* Turn led on  */
    _delay_ms(500);		/* Wait 500ms   */
    PORTB = 0x00;		/* Turn led off */
    _delay_ms(500);		/* Wait 500ms   */
  }
}
