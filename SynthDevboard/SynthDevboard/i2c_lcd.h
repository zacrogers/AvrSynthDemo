/*
 * i2c_lcd.h
 *
 * Library for hitachi lcd display with i2c backpack
 * Must be connected to i2c bus on - PC4 : SDA, PC : SCL
 *
 * Created: 14-Jan-20 7:51:12 PM
 *  Author: Zac
 */ 

#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <util/delay.h>
#include "i2c_master.h"

/* LCD instructions */
#define LCD_CLEAR              0x01
#define LCD_HOME               0x02
#define LCD_CURSOR_DECR        0x10
#define LCD_CURSOR_INCR        0x14

#define LCD_SHIFT_DISP_R       0x05
#define LCD_SHIFT_DISP_L       0x07

#define LCD_DISP_OFF_CUR_OFF   0x08
#define LCD_DISP_OFF_CUR_ON    0x0A
#define LCD_DISP_ON_CUR_OFF    0x0C
#define LCD_CUR_BLINK_OFF      0x0E
#define LCD_CUR_BLINK_ON       0x0F

#define LCD_CUR_HOME_1         0x80
#define LCD_CUR_HOME_2         0xC0

#define LCD_MODE_4BIT          0x28


typedef struct
{
	uint8_t  address; 
	uint8_t  lines;
	uint8_t  columns;
	//uint8_t  mode;
}I2C_LCD;


void LCD_Init                (I2C_LCD *lcd);             // initialize lcd
void LCD_SendCmd             (I2C_LCD *lcd, char cmd);   // send command to the lcd
void LCD_SendData            (I2C_LCD *lcd, char data);  // send data to the lcd
void LCD_SendString          (I2C_LCD *lcd, const char *str);  // send string to the lcd
void LCD_Clear               (I2C_LCD *lcd);
void LCD_EnableCursorBlink   (I2C_LCD *lcd);
void LCD_DisableCursorBlink  (I2C_LCD *lcd);
void LCD_EnableCursor        (I2C_LCD *lcd);
void LCD_DisableCursor       (I2C_LCD *lcd);
void LCD_SetCursor           (I2C_LCD *lcd, uint8_t line, uint8_t col);

#ifdef __cplusplus
}
#endif

#endif /*I2C_LCD_H_ */

