#include <xc.h>
#include "setup.h"
#include "channels.h"
#include "pca9685.h"
#include "lookuptable.h"

/*
 * DMX ./. I2C Converter zum Treiben des PCA9585
 * Version 1.0 Februar 2018
 */

volatile CHANNELINFO channels[MAXCHANNELS];

/*
 * Initializing of channeldata
 */
void initChannels()  {
    
    unsigned char i; 
    
    for (i = 0; i < MAXCHANNELS; i++)  {
        channels[i].bChanged = 0;
        channels[i].level    = 0;
    }
}

/*
 * Sets a channel to a level and marks the channel as changed. 
 * This allows to identify chenged channels to reduce traffic on i2c
 */

void setChannelLevel(unsigned char channel, unsigned char level)   {
    
    if ( channel >= MAXCHANNELS )  {                // Memory safety
        return;
    }
    
    if ( channels[channel].level != level)  {  
        channels[channel].level    = level;
        channels[channel].bChanged = 1; 
    }
}

/*
 * Transfers all changed channels via i2c.
 */

void sendChangedChannels(void)  {
    
    unsigned char i; 
    unsigned char device ; 
    unsigned char channel; 
    
    for (i = 0; i < MAXCHANNELS; i++ )  {
        
        if ( channels[i].bChanged == 1)  {
            channels[i].bChanged = 0; 
           
            if ( i > 15)  {
               device  = 1;
               channel = i - 16U;  
            }
            else  {
               device  = 0;
               channel = i;  
            }
            
            pcaSetChannelLog(device, channel, channels[i].level);
        }
    }
}

/*
 * Testet ob wir den pca in sleep schicken koennen
 *  
 *  !!! --- !!!
 *  Die Verwendung dieser Methode führt zu irritationen. 
 *  Es werden weitere kanäle mitgeführt. Beim aufruf von lichtstimmungen fällt das wenig auf, 
 *  wenn einzelne kanäle gefadet werden, laufen ander kanäle mit. 
 *  Das ist dann der fall wenn als startbedingung alle känäle == 0 sind. 
 *  
 *  Diese Methode wird also nicht verwendet.
 */

static unsigned char bAllChannelsOff = 1; 

void testChannelLevels(void)  {
    
    unsigned char aChannelIsOn = 0; 
    unsigned char i; 
    
    for (i = 0; i < MAXCHANNELS; i++ )  {
    
        if ( channels[i].level > 0)  {
            aChannelIsOn = 1; 
            break; 
        }    
    }
    
    if ( bAllChannelsOff == 1 )  {
        
        if (aChannelIsOn == 1)  {
            pcaWakeUp(0);
            pcaWakeUp(1);
            bAllChannelsOff = 0; 
        }
    }
    else  {
       if (aChannelIsOn == 0)  {
           pcaSleep(0); 
           pcaSleep(1); 
           bAllChannelsOff = 1;
       }  
    }
}

