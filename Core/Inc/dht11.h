/**
  ******************************************************************************
  * @file           : dht11.h
  * @brief          : Plik naglowkowy interfejsu czujnika DHT11
  ******************************************************************************
  * @attention
  *	Autor biblioteki - Controllers Tech<br>
  *	Uzupelnienie funkcjonalnosci - Michal Soldacki, Patryk Prusinowski<br>
  * Film autora - www.youtube.com/watch?v=09C1dyXvSbg (DHT11+LCD)
  ******************************************************************************
  */
#include "stm32l4xx_hal.h"

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

void delay(uint16_t time);

void Display_Temp (float Temp);

void Display_Rh (float Rh);

void Display_Rain (char *Rain);

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void DHT11_Start (void);

uint8_t DHT11_Check_Response (void);

uint8_t DHT11_Read (void);

float temperatureCorrection(float temperature);

float humidityCorrection(float humidity);



#endif /* INC_DHT11_H_ */
