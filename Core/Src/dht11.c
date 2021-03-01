/**
  ******************************************************************************
  * @file           : dht11.c
  * @brief          : Interfejs czujnika DHT11
  ******************************************************************************
  * @attention
  *	Autor biblioteki - Controllers Tech<br>
  *	Uzupelnienie funkcjonalnosci - Michal Soldacki, Patryk Prusinowski<br>
  * Film autora - www.youtube.com/watch?v=09C1dyXvSbg (DHT11+LCD)
  ******************************************************************************
  */

#include "dht11.h"
#include "i2c-lcd.h"
#include "stdio.h"
extern TIM_HandleTypeDef htim6;

/**
* Funkcja opoznienia w mikrosekundach
* @param time czas opoznienia
*/
void delay(uint16_t time){
	__HAL_TIM_SET_COUNTER(&htim6,0);
	while((__HAL_TIM_GET_COUNTER(&htim6))<time);
}

/**
* Funkcja wyswietlajaca temperature oraz instrukcje na LCD
* @param Temp temperatura
*/
void Display_Temp (float Temp)
{
	lcd_clear();
	char str1[20] = {0};
	lcd_put_cur(0, 0);

	sprintf (str1, " H-Hum   R-Rain");
	lcd_send_string(str1);

	char str2[20] = {0};
	lcd_put_cur(1, 0);

	sprintf (str2, "TEMP: %.2d ", (int)Temp);
	lcd_send_string(str2);
	lcd_send_data('C');
}

/**
* Funkcja wyswietlajaca wilgotnosc oraz instrukcje na LCD
* @param Rh wilgotnosc
*/
void Display_Rh (float Rh)
{
	lcd_clear();
	char str1[20] = {0};
	lcd_put_cur(0, 0);

	sprintf (str1, " T-Temp  R-Rain");
	lcd_send_string(str1);

	char str2[20] = {0};
	lcd_put_cur(1, 0);

	sprintf (str2, "HUM: %.2d ", (int)Rh);
	lcd_send_string(str2);
	lcd_send_data('%');
}

/**
* Funkcja wyswietlajaca stan deszczu oraz instrukcje na LCD
* @param Rain stan deszczu
*/
void Display_Rain (char *Rain)
{
	lcd_clear();
	char str1[20] = {0};
	lcd_put_cur(0, 0);

	sprintf (str1, " T-Temp   H-Hum");
	lcd_send_string(str1);

	char str2[20] = {0};
	lcd_put_cur(1, 0);

	sprintf (str2, "Rain: %s", Rain);
	lcd_send_string(str2);
}

/**
* Funkcja przygotowujaca wyjscie pinu
* @param GPIOx struktura pinu
* @param GPIO_Pin wykorzystywany pin
*/
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
* Funkcja przygotowujaca wejscie pinu
* @param GPIOx struktura pinu
* @param GPIO_Pin wykorzystywany pin
*/
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
* Funkcja incjalizujaca dzialanie czujnika
*/
void DHT11_Start (void)
{
	Set_Pin_Output(DHT11_PORT, DHT11_PIN);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 1);
	delay(1000);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 0);
	delay (18000);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 1);
	delay(20);
	Set_Pin_Input(DHT11_PORT, DHT11_PIN);
}

/**
* Funkcja sprawdzajaca odpowiedz czujnika
* @return stan odpowiedzi czujnika
*/
uint8_t DHT11_Check_Response (void)
{
	uint8_t Response = 0;
	delay (40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay (80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
		else Response = -1; // 255
	}
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go low

	return Response;
}

/**
* Funkcja odczytujaca dane z czujnika
* @return odczytana wartosc z czujnika
*/
uint8_t DHT11_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go high
		delay (40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for the pin to go low
	}
	return i;
}

/**
* Funkcja kalibrujaca wartosc temperatury
* @param temperature temperatura
* @return skalibrowana temperatura
*/
float temperatureCorrection(float temperature){
	return temperature + 10.0;
}

/**
* Funkcja kalibrujaca wartosc wilgotnosci
* @param humidity wilgotnosc
* @return skalibrowana wilgotnosc
*/
float humidityCorrection(float humidity){
	return humidity - 100.0;
}
