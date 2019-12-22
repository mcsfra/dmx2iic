/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
  
#ifndef PCA9685_H
#define	PCA9685_H

#define PCA9685_I2C_BASE_ADDRESS    (unsigned char)0x80

// Register addresses from data sheet

#define PCA9685_MODE1_REG           (unsigned char)0x00
#define PCA9685_MODE2_REG           (unsigned char)0x01
#define PCA9685_SUBADR1_REG         (unsigned char)0x02
#define PCA9685_SUBADR2_REG         (unsigned char)0x03
#define PCA9685_SUBADR3_REG         (unsigned char)0x04
#define PCA9685_ALLCALL_REG         (unsigned char)0x05
#define PCA9685_LED0_REG            (unsigned char)0x06 // Start of LEDx regs, 4B per reg, 2B on phase, 2B off phase, little-endian
#define PCA9685_PRESCALE_REG        (unsigned char)0xFE
#define PCA9685_ALLLED_REG          (unsigned char)0xFA

// Mode1 register pin layout

#define PCA9685_MODE_RESTART        (unsigned char)0x80
#define PCA9685_MODE_EXTCLK         (unsigned char)0x40
#define PCA9685_MODE_AUTOINC        (unsigned char)0x20
#define PCA9685_MODE_SLEEP          (unsigned char)0x10
#define PCA9685_MODE_SUBADR1        (unsigned char)0x08
#define PCA9685_MODE_SUBADR2        (unsigned char)0x04
#define PCA9685_MODE_SUBADR3        (unsigned char)0x02
#define PCA9685_MODE_ALLCALL        (unsigned char)0x01

#define PCA9685_SW_RESET            (unsigned char)0x06    // Sent to address 0x00 to reset all devices on Wire line
#define PCA9685_PWM_FULL            (unsigned int) 0x01000 // Special value for full on/full off LEDx modes
 
extern void pcaInit(unsigned char which);
extern unsigned char pcaGetAddress(unsigned char which); 
extern void pcaSetChannelRaw(unsigned char which, unsigned char channel, unsigned int level);
extern void pcaSetChannelLin(unsigned char which, unsigned char channel, unsigned char value);
extern void pcaSetChannelLog(unsigned char which, unsigned char channel, unsigned char value);
extern void pcaSleep(unsigned char which );  
extern void pcaWakeUp(unsigned char which );

extern void pcaSetSpeed(unsigned char which, unsigned int hertz );

extern void pcaSweepTest(void);

#endif	

