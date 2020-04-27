#include <xc.h>        /* XC8 General Include File */

#include "hardware.h"
#include "dmx.h"
#include "i2c.h"
#include "pca9685.h"
#include "setup.h"
#include "channels.h"

unsigned char errorCounter = 0;

unsigned char baseAddress; 

/*
 * Pinout
 *  
 *  1 --  VDD  
 *  2 -- 
 *  3 -- 
 *  4 --  /Reset
 *  5 --  DMX Address 16 
 *  6 --  DMX Address  8
 *  7 --  DMX Address  4
 *  8 --  DMX Address 32 
 *  9 --  DMX Address 64
 * 10 --
 * 11 --  I2C - Clock
 * 12 --  DMX IN 
 * 13 --  I2C - Data
 * 14 --  RC2 - DMX State - Framing Error
 * 15 --  RC1 - DMX State - No Signal 
 * 16 --  RC0 - DMX State - IncomingData > 0
 * 17 --   
 * 18 --  RA1 - Programming DATA 
 * 19 --  RA0 - Programming CLOCK
 * 20 --  GND
 *  
 */

void main(void)  {
 
    initHardware();             // I/O Registers
    initChannels();             // Internal Data Stuctures
    i2cInit();                  
    pcaInit(0);                 // Enable PCA #0
    pcaInit(1);                 // Enable PCA #1
    initDMX();                  // And finally dmx. 
  
    RCONbits.IPEN = 1;          // Peripheral Interrupts enable 
    
    /*
     * Hier habe ich ein ToDo. Eigentlich dachte ich, dass ich auf den LoeInte4rrupt reagiere
     * Dennoch müssen wie es scheint BEIDE Bits gesetzt sein. Nanu?
     */   

    INTCONbits.GIEH = 1; 
    INTCONbits.GIEL = 1;
    PEIE = 1; 
    
    /*
     *  Initialize connected PCAs
     */
    
    pcaWakeUp(0);
    pcaWakeUp(1);
    
    for (;;)  {
        
        if ( OERR )  {
           CREN = 0; 
           resetDMX(); 
           
           __nop(); __nop(); __nop(); __nop(); __nop();
           OERR = 0;
           CREN = 1; 
        }
        
        
         // Work
        sendChangedChannels(); 

        // Cable Monitoring         
        decrementDataWatchdog();            // Handle Watchdog for Channeldata within Addressrange > 0
        decrementFramingWatchdog();         // Watchdog for detection and reporting of framing errors ( cable copnnection interchanged )
     
        
        if ( decrementSignalWatchdog() == 1 )  {  
        //     SPEN = 0;
        //     RCIE = 0;
        //     initDMX();
        }
#ifdef ADVANCED_POWER_SAVE    
            testChannelLevels();
#endif            
    }
}



