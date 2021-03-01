/**
  ******************************************************************************
  * @file           : func.c
  * @brief          : Biblioteka z pozostala funkcjonalnoscia
  ******************************************************************************
  * @attention
  *	Autorzy - Michal Soldacki, Patryk Prusinowski
  ******************************************************************************
  */

#include "func.h"
#include "dht11.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
extern uint8_t rxdata;
//extern uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
//extern uint16_t SUM, RH, TEMP;
//extern uint8_t Presence;

static __IO uint32_t TimingDelay;

/**
* Funkcja wywolujaca opoznienie
* @param nTime czas opoznienia w milisekundach
*/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
* Funkcja dekrementujaca TimingDelay do momentu wyzerowania licznika
*/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/*void prepareParameters(float temperature, float humidity){
	DHT11_Start();
	Presence = DHT11_Check_Response();
	Rh_byte1 = DHT11_Read ();
	Rh_byte2 = DHT11_Read ();
	Temp_byte1 = DHT11_Read ();
	Temp_byte2 = DHT11_Read ();
	SUM = DHT11_Read();

	TEMP = Temp_byte1;
	RH = Rh_byte1;

	temperature = (float) TEMP;
	humidity = (float) RH;

	temperature = temperatureCorrection(temperature);
	humidity = humidityCorrection(humidity);
}*/

/**
* Funkcja przetwarzajaca wartosc liczbowa z czujnika na stan opisowy deszczu
* @param rainMeas wartosc liczbowa z czujnika
* @param rain opis tekstowy stanu deszczu
*/

void prepareRain(uint16_t rainMeas, char *rain){
	if(rainMeas < 1500) strcpy(rain, "Leje! ");
	else if(rainMeas >= 1500 && rainMeas < 3000) strcpy(rain, "Kropi.");
	else if(rainMeas >= 3000 && rainMeas <= 4095) strcpy(rain, "Czysto");
	else {};
}

/**
* Funkcja przeksztalcajaca tablice char'ow na tablice uint'ow
* @param text tablica char'ow
* @param newtext tablica uint'ow
*/
void charToUint(char *text, uint8_t *newtext){
	for(int i=0; i<strlen(text); i++){
		newtext[i] = (uint8_t)text[i];
	}
}

/**
* Funkcja wyswietlajaca temperature na LCD
* @param temperature temperatura
*/
void showTemperatureLCD(float temperature){
	Display_Temp(temperature);
}

/**
* Funkcja wyswietlajaca wilgotnosc na LCD
* @param humidity wilgotnosc
*/
void showHumidityLCD(float humidity){
	Display_Rh(humidity);
}

/**
* Funkcja wyswietlajaca stan deszczu na LCD
* @param rain stan deszczu
*/
void showRainLCD(char *rain){
	Display_Rain(rain);
}

/**
* Funkcja przesylajaca temperature przez UART do modulu BT
* @param temperature temperatura
*/
void sendTemperatureUART(float temperature){
	char text[20] = "Temperatura [*C]: ";
	char buffer[5];
	uint8_t text2[20];

	sprintf(buffer, "%d", (int)temperature);
	strcat (text, buffer);
	charToUint(text,text2);

	HAL_UART_Transmit_IT(&huart1, text2, strlen(text2));
	HAL_Delay(19);
	HAL_UART_AbortTransmit_IT(&huart1);
}

/**
* Funkcja przesylajaca wilgotnosc przez UART do modulu BT
* @param humidity wilgotnosc
*/
void sendHumidityUART(float humidity){
	char text[19] = "Wilgotnosc [%]: ";
	char buffer[5];
	uint8_t text2[19];

	sprintf(buffer, "%d", (int)humidity);
	strcat (text, buffer);
	charToUint(text,text2);

	HAL_UART_Transmit_IT(&huart1, text2, strlen(text2));
	HAL_Delay(17);
	HAL_UART_AbortTransmit_IT(&huart1);
}

/**
* Funkcja przesylajaca stan deszczu przez UART do modulu BT
* @param rain stan deszczu
*/
void sendRainUART(char *rain){
	char text[18] = "Stan nieba: ";
	uint8_t text2[18];

	strcat (text, rain);
	charToUint(text,text2);

	HAL_UART_Transmit_IT(&huart1, text2, strlen(text2));
	HAL_Delay(16);
	HAL_UART_AbortTransmit_IT(&huart1);
}
