#include "pca9685.h"
#include "i2c.h"
#include "lookuptable.h"


unsigned char pcaGetAddress(unsigned char which)  {
    
    return (unsigned char)(PCA9685_I2C_BASE_ADDRESS + ( which == 0 ? 0 : 2)); 
}

void pcaInit(unsigned char which)  {
  
    // Es hat sich gezeigt, dass die 50 Hz a) nichts bringen und b) noch schlimmeres Flackern produzieren. 
    // Also nicht verwenden.
    // pcaSleep(which);
    // pcaSetSpeed(which, 50);
    pcaWakeUp(which);
    
}
  

void pcaSetChannelRaw(unsigned char which, unsigned char channel, unsigned int level)  {
        
    i2cStart();
    i2cSend(pcaGetAddress(which) );
    i2cSend(( channel * 4U ) + 0x06U);
    i2cSend(0x00);
    i2cSend(0x00);
    i2cSend((unsigned char)(level & 0xFF));
    i2cSend((unsigned char)((level >> 8 ) & 0x0F));
    i2cStop();
}
     
void pcaSetChannelLin(unsigned char which, unsigned char channel, unsigned char value)   {
        
    unsigned int v = value; 
    pcaSetChannelRaw(which, channel, v << 4); 
}
 
void pcaSetChannelLog(unsigned char which, unsigned char channel, unsigned char value)  {
       
    pcaSetChannelRaw(which, channel, getLookup(value)); 
}

void pcaSleep(unsigned char which)    {
    
    i2cStart();
    i2cSend(pcaGetAddress(which) );
    i2cSend(0x00);
    i2cSend(0x10);
    i2cStop();
}

void pcaWakeUp(unsigned char which)   {
        
    i2cStart();
    i2cSend(pcaGetAddress(which));
    i2cSend(0x00);
    i2cSend(0x80 | 0x20);
    i2cStop();
}

void pcaSetSpeed(unsigned char which, unsigned int hertz)   {
        
    unsigned long data = (unsigned long)(25000000L / ( 4096L * hertz)) - 1; 
    
    i2cStart();
    i2cSend(pcaGetAddress(which));
    i2cSend(0xFE);
    i2cSend((unsigned char) data);
    i2cStop();
    
}

/*
 * Testrun
 */

void pcaSweepTest(void)  {
    
    unsigned char i, j;
   
    for(;;)   {
        for (j = 0; j < 255; j++ )  {
            for ( i = 0; i < 16; i++)  {        
               pcaSetChannelLog(0,i,j);
            }
        }
    }
}    
 





        