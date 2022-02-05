/* 
 * File:   rs485.h
 * Author: marcus
 *
 * Created on 31. Juli 2014, 21:33
 */

#ifndef __DMX_H__
#define	__DMX_H__

#define LED_FRAMINGERROR  LATCbits.LATC2
#define LED_NOSIGNALERROR LATCbits.LATC1
#define LED_DATAVALID     LATCbits.LATC0



extern void initDMX(void);
extern void resetDMX(void);

extern void handleDMX(void);
//extern void setDMXNotSynced(void);
extern unsigned char getDMXBaseAddress(void); 

inline void resetDataWatchdog(void);
extern void decrementDataWatchdog(void);

inline void resetSignalWatchdog(void);
extern unsigned char decrementSignalWatchdog(void);

inline void resetFramingWatchdog() ;
extern void decrementFramingWatchdog(void);


#endif	

