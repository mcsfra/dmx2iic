#include <xc.h>
#include "dmx.h"
#include "setup.h"
#include "channels.h"

/*
** Baudrate
** Datasheet Page 280
*/

#define MEGAHERTZ 1000000
#define FOSC (8 * MEGAHERTZ)


/*
 *  Debug Buffer
 */

unsigned char dmxDebugBuffer[128];


unsigned char dmxState; 
unsigned int  currentAdress;

void initDebugBuffer()  {
    unsigned char i; 

    for (i = 0; i < 128;i++)  {
        dmxDebugBuffer[i] = 0;
    }
 
}


void initDMX(void)  {
    
    BAUDCONbits.BRG16 = 1;     // Baudratengenerator 8-Bit
    BAUDCONbits.CKTXP = 1;     // polaritaet verdrehen
    SPBRG1            = 1;     // Baudrate einstellen
    SPBRGH            = 0;     //

    RCSTA1bits.CREN   = 1;
    RCSTA1bits.SPEN   = 1;

    PIE1bits.RCIE     = 1;
    IPR1bits.RCIP     = 0;  // Low Priority

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



unsigned char errflag;
unsigned char received;

unsigned char dummy;

void handleDMX()  {
    
   unsigned char baseAddress = getDMXBaseAddress();      

   /*
   ** error-flag holen ( muss vor dem lesen der eigentlichen date gemacht werden )
   */
   errflag = (unsigned char)FERR;

  /*
   ** Das Empfangene Byte holen
   */
   received = RCREG;

   if ( (errflag == 1) && ( dmxState == DMX_NOTSYNCED)  )  {
       currentAdress = 0;
       dmxState = DMX_WAITSTART;
       return;
   }

   if ( (dmxState == DMX_WAITSTART) && (received == 0x00) )  {

      dmxState = DMX_PAYLOAD;
      return;
   }

   if ( dmxState == DMX_PAYLOAD)  {

      if ( currentAdress < 128)  {
          dmxDebugBuffer[currentAdress] = received;
      }  
       
      if ( (currentAdress >= baseAddress) &&  (currentAdress < baseAddress + MAXCHANNELS))  {

          /* Dieses Konstrukt verstehe ich nicht
           deswegen auskommentiert. Februar 2018
           if ( channelLevel[currentAdress-BASEADDRESS] == 0 && received != 0)  {
              dummy = LATA;  // zum Setzen des Breakdowns
          }
          */ 
          setChannelLevel(currentAdress - baseAddress, received); 
      }

      currentAdress++;

      if ( currentAdress >= 128)
         dmxState = DMX_NOTSYNCED;

   }
}


