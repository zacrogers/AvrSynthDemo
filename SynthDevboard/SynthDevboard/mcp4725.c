/*
 * mcp4725.c
 *
 * Created: 15-Jan-20 9:50:07 AM
 *  Author: Zac
 */ 
#include "mcp4725.h"


void mcp4725_init(uint8_t address)
{
	i2c_init();
	i2c_start(address << 1);
}

void mcp4725_write(uint16_t data)
{
	/*
	i2c_transmit(MCP4725_ADDR << 1, (uint8_t*)WRITE_DAC, 8);
	i2c_transmit(MCP4725_ADDR << 1, (uint8_t*)(data >> 4), 8);
	i2c_transmit(MCP4725_ADDR << 1, (uint8_t*)((data & 0xF) << 4), 8);
	*/
	//i2c_start(MCP4725_ADDR << 1);
	i2c_write(WRITE_DAC);
	i2c_write(data >> 4);	        /* msb */
	i2c_write((data & 0xF) << 4);	/* lsb */
	//i2c_stop();
}