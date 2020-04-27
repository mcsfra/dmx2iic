#include <xc.h>         
#include "dmx.h"

/* 
** High-priority service
*/


/*
void interrupt high_isr(void)  {
}
*/

void high_priority interrupt high_isr(void)  {

    /*
     * DMX- Eingangsdatenstrom
     */
    if (PIR1bits.RCIF == 1)  {
        handleDMX();
        PIR1bits.RCIF = 0;
    }
}




/*
** Low prio Interrupt
*/

void low_priority interrupt low_isr(void)  {

    /*
     * DMX- Eingangsdatenstrom
     */
   // if (PIR1bits.RCIF == 1)  {
   //     handleDMX();
    //    PIR1bits.RCIF = 0;
   // }
}