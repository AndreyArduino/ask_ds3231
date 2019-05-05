/*********************************************************************
Библиотека дла работы с часами реального времени на чипсете DS3231
ВерсиЯ 1.00

Функции библиотеки
begin()                            - инициализация часов

Внутренние функции библиотеки

Все мои проекты собраны здесь
http://xn--b1adda5bp2i.xn--g1achdice.xn--p1ai/avtomatizatsiya
*********************************************************************/

#include "ask_ds3231.h"
#include <Wire.h>

ask_ds3231::ask_ds3231() {

} 

uint8_t ask_ds3231::decToBcd(const uint8_t aVal) {
	return ((aVal / 10 * 16) + (aVal % 10));
} 

uint8_t ask_ds3231::bcdToDec(const uint8_t aVal) {
	return ((aVal / 16 * 10) + (aVal % 16));
} 

void ask_ds3231::setUp() {
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(0x0E);
	Wire.write(0x1C);
	Wire.write(0x88);
	Wire.endTransmission();

	// проверим если дата меньше чем 01.01.2018 то установим дату  01.01.2018 00:00:00
	mDT = getDateTime();

	if (mDT.year < 2018) {
		mDT.year = 2018;
		mDT.month = 01;
		mDT.day = 01;
		mDT.hour = 0;
		mDT.minute = 0;
		mDT.second = 0;
		mDT.dayOfWeek = dayOfWeak(2018, 01, 01);
		setDateTime(mDT);
	}
}

void ask_ds3231::begin() {
	mSensorActivated = true;
	Wire.begin();
	Wire.beginTransmission(I2C_ADDR_RTC);
	if (Wire.endTransmission() != 0) {
		mSensorActivated = false;
		return;
	}
	setUp();
} 

uint8_t ask_ds3231::dayOfWeak(uint16_t aYear, uint8_t aMonth, uint8_t aDay) {
	aYear = 2000 + aYear;
	uint16_t a = (14 - aMonth) / 12;
	uint16_t y = aYear - a;
	uint16_t m = aMonth + 12 * a - 2;
	return  ((aDay + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / 12) % 7) + 1;
} // dayOfWeak(uint16_t aYear, uint8_t aMonth, uint8_t aDay)

void ask_ds3231::setDateTime(uint16_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMinute, uint8_t aSecond)
{
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(uint8_t(0x00));
	Wire.write(decToBcd(aSecond));
	Wire.write(decToBcd(aMinute));
	Wire.write(decToBcd(aHour));
	Wire.write(decToBcd(dayOfWeak(aYear, aMonth, aDay)));
	Wire.write(decToBcd(aDay));
	Wire.write(decToBcd(aMonth));
	Wire.write(decToBcd(aYear - 2000));
	Wire.endTransmission();
} // ask_ds3231::setDateTime(uint16_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMinute, uint8_t aSecond)

void ask_ds3231::setDateTime(RTCDateTime aDT)
{
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(uint8_t(0x00));
	Wire.write(decToBcd(aDT.second));
	Wire.write(decToBcd(aDT.minute));
	Wire.write(decToBcd(aDT.hour));
	Wire.write(decToBcd(dayOfWeak(aDT.year, aDT.month, aDT.day)));
	Wire.write(decToBcd(aDT.day));
	Wire.write(decToBcd(aDT.month));
	Wire.write(decToBcd(aDT.year - 2000));
	Wire.endTransmission();
} // ask_ds3231::setDateTime(RTCDateTime aDT)

RTCDateTime ask_ds3231::getDateTime(void) {
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(uint8_t(0x00));
	Wire.endTransmission();
	Wire.requestFrom(I2C_ADDR_RTC, 7);
	while (!Wire.available()) {};
	for (uint8_t i = 0; i <= 6; i++)
	{
		switch (i) {
		case 0:
			mDT.second = bcdToDec(Wire.read());
			break;
		case 1:
			mDT.minute = bcdToDec(Wire.read());
			break;
		case 2:
			mDT.hour = bcdToDec(Wire.read());
			break;
		case 3:
			mDT.dayOfWeek = bcdToDec(Wire.read());
			break;
		case 4:
			mDT.day = bcdToDec(Wire.read());
			break;
		case 5:
			mDT.month = bcdToDec(Wire.read());
			break;
		case 6:
			mDT.year = bcdToDec(Wire.read()) + 2000;
			break;
		default:
			break;
		}
	}
	return mDT;
} // RTCDateTime ask_ds3231::getDateTime()

int16_t ask_ds3231::getTemperature(void) {
	uint8_t lMsb = 0, lLsb = 0;
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(uint8_t(0x11));
	Wire.endTransmission();
	Wire.requestFrom(I2C_ADDR_RTC, 2);
	while (!Wire.available()) {};
	lMsb = Wire.read();
	lLsb = Wire.read();
	return (int16_t)((float)((int16_t)lMsb << 8 | (int16_t)lLsb) / 2.56);
} // RTCDateTime ask_ds3231::getTemperature()

uint8_t ask_ds3231::getSecond(void)
{
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(uint8_t(0x00));
	Wire.endTransmission();
	Wire.requestFrom(I2C_ADDR_RTC, 1);
	return bcdToDec(Wire.read());
} // uint8_t ask_ds3231::getSecond(void)

uint8_t ask_ds3231::getMinute(void)
{
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(0x01);
	Wire.endTransmission();
	Wire.requestFrom(I2C_ADDR_RTC, 1);
	return bcdToDec(Wire.read());
} 

uint8_t ask_ds3231::getHour(void)
{
	Wire.beginTransmission(I2C_ADDR_RTC);
	Wire.write(0x02);
	Wire.endTransmission();
	Wire.requestFrom(I2C_ADDR_RTC, 1);
	return bcdToDec(Wire.read());
} 
  

uint32_t ask_ds3231::getMinuteFrom18Year(void)
{
	const uint16_t lDayFromStartYear[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	uint32_t lMinute = 0;
	RTCDateTime lDT = getDateTime();
	lMinute += lDT.minute;
	lMinute += lDT.hour * 60UL;
	lMinute += (lDT.day - 1) * 1440UL;
	lMinute += lDayFromStartYear[(lDT.month - 1)] * 1440UL;
	lMinute += (lDT.year - 2018) * 525600UL;
	
	if (((lDT.year % 4) == 0) && (lDT.month > 2))
	{ 
		lMinute += 1440UL;
	}
	
	for (uint16_t y = 2018; y < lDT.year; y++) {
		if ((y % 4) == 0)
		{ 
			lMinute += 1440UL;
		}
	}
	return lMinute;
} //ask_ds3231::getMinuteFrom18Year()

uint32_t ask_ds3231::getMinuteFrom18Year(RTCDateTime aDT)
{
	const uint16_t lDayFromStartYear[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	uint32_t lMinute = 0;
	lMinute += aDT.minute;
	lMinute += aDT.hour * 60UL;
	lMinute += (aDT.day - 1) * 1440UL;
	lMinute += lDayFromStartYear[(aDT.month - 1)] * 1440UL;
	lMinute += (aDT.year - 2018) * 525600UL;
	if (((aDT.year % 4) == 0) && (aDT.month > 2))
	{ 
		lMinute += 1440UL;
	}
	for (uint16_t y = 2018; y < aDT.year; y++) {
		if ((y % 4) == 0)
		{ 
			lMinute += 1440UL;
		}
	}
	return lMinute;
} //ask_ds3231::getMinuteFrom18Year()

uint32_t ask_ds3231::getSecondFrom18Year(void)
{
	static uint32_t lPreviousSecond = 0;
	uint32_t lCurrentSecond = getMinuteFrom18Year() * 60UL + getSecond();
	if (lCurrentSecond > lPreviousSecond) {
		lPreviousSecond = lCurrentSecond;
		return lCurrentSecond;
	}
	else {
		return lPreviousSecond;
	}
} //ask_ds3231::getSecondFrom18Year()

uint32_t ask_ds3231::getSecondFrom18Year(RTCDateTime aDT)
{
	static uint32_t lPreviousSecond = 0;
	uint32_t lCurrentSecond = getMinuteFrom18Year(aDT) * 60UL + aDT.second;
	if (lCurrentSecond > lPreviousSecond) {
		lPreviousSecond = lCurrentSecond;
		return lCurrentSecond;
	}
	else {
		return lPreviousSecond;
	}
} //ask_ds3231::getSecondFrom18Year()

bool ask_ds3231::sensorActivated() {
	return mSensorActivated;
} // ask_ds3231::return sensorActivated()

