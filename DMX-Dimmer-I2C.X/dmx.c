#include <xc.h>
#include "dmx.h"
#include "setup.h"
#include "channels.h"

/*
** States for DMX Statemachine
*/ 
#define DMX_NOTSYNCED 0x00
#define DMX_WAITSTART 0x01
#define DMX_PAYLOAD	  0x02
 
/*
** Baudrate
** Datasheet Page 280
*/

//#define BAUDCLOCK_8MHZ 1 
#define BAUDCLOCK_16MHZ 1 
//#define BAUDCLOCK_32MHZ 1 
//#define BAUDCLOCK_64MHZ 1 

/*'
 * Variables for Statemachine
 */

unsigned char dmxState; 
unsigned char currentAdress;

unsigned char baseAddress; 
unsigned char topAddress; 

/*
 * Initialize DMX Hardware and internal statemachine 
 * 
 * 250000  8N2 
 * 
 */
void initDMX()  {
    
    baseAddress       = getDMXBaseAddress(); 
    topAddress        = baseAddress + MAXCHANNELS;      // To save some cycles in the irq

    currentAdress     = 0;
    dmxState          = DMX_NOTSYNCED;
    
    BAUDCONbits.BRG16 = 1;     // Baudratengenerator 16-Bit

    SPBRG1            = 3;     // Baudrate 250000 @ 16MHz
    SPBRGH            = 0;     //
    
    BAUDCONbits.CKTXP = 1;      // polaritaet verdrehen
    
    RCSTA1bits.CREN   = 1;
    RCSTA1bits.SPEN   = 1;
    SYNC   = 0;                 // 2.01 - Review of Datasheet
 
    PIE1bits.RCIE     = 1;
    IPR1bits.RCIP     = 1;      // HIgh Priority

}

void resetDMX()  {
    
    RCSTA1bits.CREN = 0; 
    currentAdress   = 0;
    dmxState        = DMX_NOTSYNCED;
    RCSTA1bits.CREN = 1; 
    
}

/*
 *  DMX Basisadresse von ausen einstellbar
 *  Beachten: 
 *     * Die PIn reihenfolge ist gemischt.
 *     * Eingestellte Adresse mal 4 
 */
unsigned char getDMXBaseAddress(void)  {
    
    return (unsigned char)(PORTC >> 1U) & 0b01111100U;
}

void handleDMX()  {
    
    unsigned char eFraming    = (unsigned char)FERR;             // Read framing error
    unsigned char received    = RCREG;                           // Read SerialIn register
        
    resetSignalWatchdog();                      // Received data, therefore at least a "signal" is available
   
    if ( eFraming == 1)  {                      // Framing Error 
        
        if ( dmxState == DMX_NOTSYNCED )  {     // expected
            currentAdress = 0;                  // new receive cycle
            dmxState      = DMX_WAITSTART;
        }
        else  {
            resetFramingWatchdog();             // This is the case if e.g. d+ and d- are interchanged 
        }
        return; 
    }

    if ( (dmxState == DMX_WAITSTART) && (received == 0x00) )  {

        dmxState = DMX_PAYLOAD;
        return;
    }

    if ( dmxState == DMX_PAYLOAD)  {
      
        if ( (currentAdress >= baseAddress) &&  (currentAdress < topAddress))  {
  
            if (received > 0)  {
               resetDataWatchdog(); 
            }
            setChannelLevel((unsigned char)(currentAdress - baseAddress), received); 
        }
        
        if ( ++currentAdress >= 128)
           dmxState = DMX_NOTSYNCED;
    }
}

/*
 * DMX Error Monitoring
 * 
 *  [[ Editors Note: Diese Methoden sind doch sehr ähnlich; hier wäre eine generifizierung 
 *     Denkbar ]]
 */

/*
 *  Daten vorhanden? 
 */


volatile unsigned int dataWatchdog = 0; 

inline void resetDataWatchdog()  {
    dataWatchdog = 100; 
}

void decrementDataWatchdog()  {
    
    if ( dataWatchdog > 0)  {
        dataWatchdog--; 
        LED_DATAVALID = 1; 
    }
    else   { 
        LED_DATAVALID = 0;
    }    
}

/*
 * Monitor: Liegt ein serielles Signal an? 
 */

volatile unsigned int signalWatchdog = 0; 

inline void resetSignalWatchdog()  {

     signalWatchdog = 1000;   // Changed from 1000 to 10000. 
}

unsigned char decrementSignalWatchdog()  {
    
    if ( signalWatchdog > 0)  {
        signalWatchdog--; 
        LED_NOSIGNALERROR = 0; 
        return 0;
    }
    else   { 
        LED_NOSIGNALERROR = 1;
        return 1; 
    }    
}

/*
 * Framing errors. 
 * Framing Errors kommen auch im Normalbetrieb vor ( Sync) 
 * Wenn sie gehäuft vorkommen ist das ein Zeichen für Eine Verpolung

  */

volatile unsigned int framingErrorWatchdog = 0; 

inline void resetFramingWatchdog()  {
    LED_FRAMINGERROR     = 1; 
    framingErrorWatchdog = 1000; 
} 

void decrementFramingWatchdog()  {
    
    if (framingErrorWatchdog > 0)  {
       framingErrorWatchdog--;
    }
    else  {
        LED_FRAMINGERROR = 0; 
    }
}


  
    

