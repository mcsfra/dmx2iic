#include <xc.h>
#include "i2c.h"


unsigned char bI2CTimeout;

void i2cInit(void)  {
      
    TRISBbits.RB4 = 1; 
    TRISBbits.RB6 = 1; 
   
    SSPCON1bits.SSPEN = 1;
    SSPADD  = 0x08;   // das stimmt doch nicht, oder? 
    SSPCON1 = 0x28;
    SSPSTAT = 0x00;
    
    bI2CTimeout = 0; 
    
}

/*
 * Von Microchip gemopst
 */

/*
 *  i2c startcondition 
 */
void i2cStart()  {

   SSPCON2bits.SEN = 1;      // set start condition and wait  
   while(SSPCON2bits.SEN)    // until hardware reseets it
       ;
}

/*
 *  i2c stop.
 */
void i2cStop()   {
  
   SSPCON2bits.PEN = 1;    
   while(SSPCON2bits.PEN)  
       ;
}

/*
 * i2c restart
 */
void i2cRestart()   {
    
   SSPCON2bits.RSEN = 1;    
   while(SSPCON2bits.RSEN) 
      ;
}

/*
Function: I2CAck
Return:
Arguments:
Description: Generates acknowledge for a transfer
*/
void i2cAck()  {

   unsigned char timeout = 0xFFU; 
    
   SSPCON2bits.ACKDT = 0;         // Acknowledge data bit, 0 = ACK 
   SSPCON2bits.ACKEN = 1;         // Ack data enabled 
   while(SSPCON2bits.ACKEN)   {   // wait for ack data to send on bus 
      if ( timeout-- == 0)  {
          bI2CTimeout = 1; 
          return; 
      }
   }
}

/*
Function: I2CNck
Return:
Arguments:
Description: Generates Not-acknowledge for a transfer
*/
void i2cNak()  {
    
   unsigned char timeout = 0xFFU;  
    
   SSPCON2bits.ACKDT = 1; // Acknowledge data bit, 1 = NAK 
   SSPCON2bits.ACKEN = 1; // Ack data enabled 
  
   while(SSPCON2bits.ACKEN)   { // wait for ack data to send on bus 
      if ( timeout-- == 0)  {
          bI2CTimeout = 1;
          return; 
      }
   }
}

/*
Function: I2CWait
Return:
Arguments:
Description: wait for transfer to finish
*/
void i2cWait()  {

     while ((SSPCON2 == 0x1F ) || ( SSPSTAT == 0x04 ) )
         ;
}

/*
Function: I2CSend
Return:
Arguments: dat - 8-bit data to be sent on bus
data can be either address/data byte
Description: Send 8-bit data on I2C bus
*/
void i2cSend(unsigned char dat)  {

   SSPBUF = dat;    // Move data to SSPBUF 
   while(BF)        // wait till complete data is sent from buffer 
      ; 
   i2cWait(); 
}

/*
Function: I2CRead
Return: 8-bit data read from I2C bus
Arguments:
Description: read 8-bit data from I2C bus
*/
unsigned char i2cRead(void)  {

    unsigned char timeout = 0xFFU; 
                                
    SSPCON2bits.RCEN = 1;        //  enable data reception... 
    while(!SSPSTATbits.BF)  {    // ...and wait for buffer full 
     
        if ( timeout-- == 0)  {  // data should be available within a reasonable time  
            bI2CTimeout = 1;     // if not, set timeout and return
            return 0;            // and return 
        }
    }
    
    unsigned char temp = SSPBUF;    // read buffer 
    i2cWait();                       
    return temp;                     
}

