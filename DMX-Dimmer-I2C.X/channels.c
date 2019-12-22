#include <xc.h>
#include "setup.h"
#include "channels.h"
#include "pca9685.h"
#include "lookuptable.h"

/*
 * DMX ./. I2C Converter zum Treiben des PCA9585
 * Version 1.0 Februar 2018
 */


CHANNELINFO channels[MAXCHANNELS];

void initChannels()  {
    
    unsigned char i; 
    
    for (i = 0; i < MAXCHANNELS; i++)  {
        channels[i].bChanged = 0;
        channels[i].level    = 0;
    }
}

void setChannelLevel(unsigned char channel, unsigned char level)   {
    
    if ( channels[channel].level != level)  {  
        channels[channel].level = level;
        channels[channel].bChanged = 1; 
    }
}


void sendChangedChannels(void)  {
    
    unsigned char i; 
    unsigned int  lookup; 
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
 *  Testet ob wir den pca in sleep schicken koennen
 *
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

