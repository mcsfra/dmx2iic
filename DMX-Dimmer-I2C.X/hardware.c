#include <xc.h>
#include "hardware.h"



void initHardware(void)  {

  // 8MHz internal OscillatorBlock

  OSCCONbits.IDLEN = 0;

  OSCCONbits.IRCF2 = 1;
  OSCCONbits.IRCF1 = 1;
  OSCCONbits.IRCF0 = 0;

  OSCCONbits.SCS1  = 1;
  OSCCONbits.SCS0  = 0;

  /*
  ** I/O
  */

  CM1CON0  = 0;
  CM2CON0  = 0;

  ANSEL   = 0x00;
  ANSELH  = 0x00;

  LATA    = 0x00;
  PORTA   = 0x00;
  TRISA   = 0b00000011;              

  PORTB   = 0x00;
  LATB    = 0x00;
  TRISB   = 0b00100111;

  LATC    = 0x00;
  PORTC   = 0x00;
  TRISC   = 0b11111000;  // für DMX-Basisadresse  
  
            // 2^0 - RC3 - Pin7
            // 2^1 - RC4 - Pin6
            // 2^2 - RC5 - Pin5
            // 2^3 - RC6 - Pin8
            // 2^4 - RC7 - Pin9
}
