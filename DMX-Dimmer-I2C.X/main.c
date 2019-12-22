#include <xc.h>        /* XC8 General Include File */

#include "hardware.h"
#include "dmx.h"
#include "i2c.h"
#include "pca9685.h"
#include "setup.h"
#include "channels.h"

unsigned char errorCounter = 0;

unsigned char baseAddress; 

void generateValues(void); 

void main(void)  {
 
    initHardware();
    initChannels();
    
    i2cInit(); 
    
    pcaInit(0);
    pcaInit(1);
    
    initDMX();
  
    RCONbits.IPEN = 1;

    /*
     * Hier habe ich ein ToDo. Eigentlich dachte ich, dass ich auf den LoeInte4rrupt reagiere
     * Dennoch müssen wie es scheint BEIDE Bits gesetzt sein. Nanu?
     */   

    INTCONbits.GIEH = 1; 
    INTCONbits.GIEL = 1;
 
    pcaWakeUp(0);
    pcaWakeUp(1);
        
    for (;;)  {

     //   baseAddress = getDMXBaseAddress(); 
       // generateValues();
        
        
        if (OERR == 1)  {
             errorCounter++;
             SPEN = 0;
             RCIE = 0;
             initDMX();
        }

        sendChangedChannels(); 

        testChannelLevels(); 
    }


}



