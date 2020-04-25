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

#define MEGAHERTZ 1000000
#define FOSC (8 * MEGAHERTZ)


/*'
 * Variables for Statemachine
 */

unsigned char dmxState; 
unsigned char currentAdress;

/*
 * Initialize DMX Hardware and internal statemachine 
 */
void initDMX(void)  {
    
    BAUDCONbits.BRG16 = 1;     // Baudratengenerator 8-Bit
    BAUDCONbits.CKTXP = 1;     // polaritaet verdrehen
    SPBRG1            = 1;     // Baudrate einstellen
    SPBRGH            = 0;     //

    RCSTA1bits.CREN   = 1;
    RCSTA1bits.SPEN   = 1;

    PIE1bits.RCIE     = 1;
    IPR1bits.RCIP     = 0;     // Low Priority

    currentAdress     = 0;
    dmxState          = DMX_NOTSYNCED;
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
    
    unsigned char baseAddress = getDMXBaseAddress();             
    unsigned char eOverflow   = (unsigned char)OERR;             // Read overflow error?
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
      
        if ( (currentAdress >= baseAddress) &&  (currentAdress < baseAddress + MAXCHANNELS))  {
  
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
 *
 */

/*
 *  Daten vorhanden? 
 */


volatile unsigned int dataWatchdog = 0; 

void resetDataWatchdog()  {
    dataWatchdog = 100; 
}

void decrementDataWatchdog()  {
  
  //  This codeblock should avoid   
  //  if ( LATCbits.LATC2  == 1)   {    // in case of Framing Errrors, data cannot be good. We will override therefore the siganling of ghood data.
  //   if (PORTCbits.RC2 == 1 )  {
  //       LATCbits.LATC0 = 0; 
  //       return;        
  //   }
    
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

void resetSignalWatchdog()  {

    signalWatchdog = 1000; 
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

void resetFramingWatchdog()  {
    LED_FRAMINGERROR  = 1; 
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


  
    

