/*
 * i2c_master.c
 *
 * Created: 14-Jan-20 5:24:41 PM
 *  Author: Zac
 */ 
#include "i2c_master.h"

void i2c_init(void)
{
	TWBR = (uint8_t)TWBR_val;
}

uint8_t i2c_start(uint8_t address)
{
	/* Reset control resister*/
	TWCR = 0;
	
	/* Transmit start condition */
	TWCR = TRANS_START | START_COND;
	
	/* Wait for transmission end */
	while(!TRANS_FINISH);
	
	/* Check if start transmit was successful */
	if((TWSR & 0xF8) != TW_START)
	{
		return 1;
	}
	
	/* Load slave address to data register */
	DATA_REG = address;
	
	/* Start address transmission */
	TWCR = TRANS_START;

	/* Wait for transmission end*/	
	while(!TRANS_FINISH);
	
	/* Check if slave acknowledged read/write mode */
	uint8_t twst = TW_STATUS & 0xF8;
	if((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
	{
		return 1;
	}
	
	return 0;
}

uint8_t i2c_write(uint8_t data)
{
	/* Load data into register */
	DATA_REG = data;
	
	/* Start data transmission */
	TWCR = TRANS_START;
	
	/* Wait for transmission end */
	while(!TRANS_FINISH);
	
	/* Check acknowledge */
	if((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		return 1;
	}
	
	return 0;
}

uint8_t i2c_read_ack(void)
{
	/* Start read and acknowledge */
	TWCR = TRANS_START | (1<<TWEA); 
	
	/* Wait for end */
	while(!TRANS_FINISH);	
	
	/* Return data */
	return DATA_REG;
}

uint8_t i2c_read_nack(void)
{
	/* Start read and not acknowledge */
	TWCR = TRANS_START;
	
	/* Wait for end */
	while(!TRANS_FINISH);
	
	/* Return data */
	return DATA_REG;
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
	if(i2c_start(address | I2C_WRITE))
	{
		return 1;
	}
	
	for(uint16_t i = 0; i < length; ++i)
	{
		if (i2c_write(data[i]))
		{
			return 1;
		}
	}
	
	i2c_stop();

	return 0;
}

uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
	if (i2c_start(address | I2C_READ))
	{
		return 1;
	}
	
	for (uint16_t i = 0; i < (length-1); ++i)
	{
		data[i] = i2c_read_ack();
	}
	
	data[(length - 1)] = i2c_read_nack();
	
	i2c_stop();
	
	return 0;
}

uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
	if (i2c_start(devaddr | 0x00))
	{
		 return 1;
	}
	
	i2c_write(regaddr);

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i])) 
		{
			return 1;
		}
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
	if (i2c_start(devaddr)) 
	{
		return 1;
	}
	
	i2c_write(regaddr);

	if (i2c_start(devaddr | 0x01))
	{
		 return 1;
	}
	
	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = i2c_read_ack();
	}
	data[(length-1)] = i2c_read_nack();

	i2c_stop();

	return 0;
}

void i2c_stop(void)
{
	/* Transmit STOP condition */
 	TWCR = TRANS_START | STOP_COND;
}