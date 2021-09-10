/*
 * mcp4725.h
 *
 * Created: 15-Jan-20 9:49:49 AM
 *  Author: Zac
 */ 


#ifndef MCP4725_H_
#define MCP4725_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_master.h"
#include <stdint.h>

#define WRITE_DAC    0x40
#define WRITE_EEPROM 0x60

#define MCP4725_MAX  0xFFF

#define MCP4725_ADDR 0x60

void mcp4725_init(uint8_t address);
void mcp4725_write(uint16_t data);
void mcp4725_read_dac(void);
void mcp4725_read_eeprom(void);

#ifdef __cplusplus
}
#endif

#endif /* MCP4725_H_ */