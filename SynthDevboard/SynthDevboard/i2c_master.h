#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#ifdef __cplusplus
extern "C"{
#endif


#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h>

/* I2c speed defines */
#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#define F_SCL         100000UL // SCL frequency
#define Prescaler     1
#define TWBR_val      ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)


/* Read/write defines */
#define I2C_READ  0x01
#define I2C_WRITE 0x00


/* Register value macros */
#define DATA_REG      TWDR
#define TRANS_START   (1U << TWINT) | (1U << TWEN)
#define TRANS_FINISH  (TWCR & (1U << TWINT))
#define START_COND    (1U << TWSTA)
#define STOP_COND     (1<<TWSTO)


/* Function defines */
void    i2c_init       (void);
uint8_t i2c_start      (uint8_t address);
uint8_t i2c_write      (uint8_t data);
uint8_t i2c_read_ack   (void);
uint8_t i2c_read_nack  (void);
uint8_t i2c_transmit   (uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_receive    (uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_writeReg   (uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
uint8_t i2c_readReg    (uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
void    i2c_stop       (void);

#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_ */