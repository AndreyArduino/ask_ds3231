/*********************************************************************
Библиотека дла работы с часами реального времени на чипсете DS3231
ВерсиЯ 1.00

Функции библиотеки
begin()                            - инициализация часов

Внутренние функции библиотеки

Все мои проекты собраны здесь
http://xn--b1adda5bp2i.xn--g1achdice.xn--p1ai/avtomatizatsiya
*********************************************************************/

#ifndef ask_ds3231_h
#define ask_ds3231_h

#include "Arduino.h"

#define I2C_ADDR_RTC   0x68

typedef struct 
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t dayOfWeek;
} RTCDateTime;

class ask_ds3231 {
public:		
	ask_ds3231();														
	void setUp();														
	void begin();														
	void setDateTime(uint16_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMinute, uint8_t aSecond); 
	void setDateTime(RTCDateTime aDT);									
	RTCDateTime getDateTime(void);										
	int16_t getTemperature(void);										
	uint8_t dayOfWeak(uint16_t aYear, uint8_t aMonth, uint8_t aDay);	
	uint8_t getSecond(void);											
	uint8_t getMinute(void);											
	uint8_t getHour(void);												
	uint32_t getMinuteFrom18Year(void);									
	uint32_t getMinuteFrom18Year(RTCDateTime aDT);						
	uint32_t getSecondFrom18Year(void);									
	uint32_t getSecondFrom18Year(RTCDateTime aDT);						
	bool sensorActivated();												

private:    
	bool mSensorActivated;
	uint8_t decToBcd(const uint8_t aVal);							
	uint8_t bcdToDec(const uint8_t aVal);							
	RTCDateTime mDT;												
};
#endif 