/**
  ******************************************************************************
  * @file           : func.h
  * @brief          : Plik naglowkowy biblioteki z pozostala funkcjonalnoscia
  ******************************************************************************
  * @attention
  *	Autorzy - Michal Soldacki, Patryk Prusinowski
  ******************************************************************************
  */

#include "stm32l4xx_hal.h"

#ifndef INC_FUNC_H_
#define INC_FUNC_H_

void Delay(__IO uint32_t nTime);

void TimingDelay_Decrement(void);

//void prepareParameters(float temperature, float humidity);

void prepareRain(uint16_t rainMeas, char *rain);

void charToUint(char *text, uint8_t *newtext);

void showTemperatureLCD(float temperature);

void showHumidityLCD(float humidity);

void showRainLCD(char *rain);

void sendTemperatureUART(float temperature);

void sendHumidityUART(float humidity);

void sendRainUART(char *rain);

#endif /* INC_FUNC_H_ */
