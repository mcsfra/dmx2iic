/* 
 * File:   channels.h
 * Author: marcus
 *
 * Februar 2018
 */

#ifndef CHANNELS_H
#define	CHANNELS_H

typedef struct channelinfo  {

    unsigned char bChanged;
    unsigned char level;
   
} CHANNELINFO;


extern void initChannels(void);
extern void setChannelLevel(unsigned char channel, unsigned char value);
extern void sendChangedChannels(void); 
extern void testChannelLevels(void);

#endif	

