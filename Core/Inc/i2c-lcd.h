/**
  ******************************************************************************
  * @file           : i2c-lcd.h
  * @brief          : Plik naglowkowy interfejsu LCD
  ******************************************************************************
  * @attention
  *	Autor biblioteki - Controllers Tech<br>
  * Film autora - www.youtube.com/watch?v=09C1dyXvSbg (DHT11+LCD)
  ******************************************************************************
  */

#include "stm32l4xx_hal.h"

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);
