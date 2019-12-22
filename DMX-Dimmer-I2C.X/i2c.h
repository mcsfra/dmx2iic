#ifndef I2C_H
#define I2C_H


extern void i2cInit(void);
extern void i2cStart(void);
extern void i2cStop(void);
extern void i2cRestart(void);
extern void i2cAck(void);
extern void i2cNak(void);
extern void i2cWait(void);
extern void i2cSend(unsigned char dat);
extern unsigned char i2cRead(void);  

#endif