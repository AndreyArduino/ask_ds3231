#include <ask_ds3231.h>

ask_ds3231 gRTC;
RTCDateTime gRTCDT;

void setup() {
  Serial.begin(115200);
  // инициализируем часы
  gRTC.begin();

  if (!gRTC.sensorActivated()) {
    Serial.println(F("ds3231 NOT activated!"));

  }
  else {
    Serial.println(F("ds3231 activated!"));
    // установка времени отдельными параметрами
    // gRTC.setDateTime(2018, 1, 1, 0, 0, 0);

    // установка времени через структуру
    // gRTCDT.year = 2018;
    // gRTCDT.month = 1
    // gRTCDT.day = 1;
    // gRTCDT.hour = 0;
    // gRTCDT.minute = 0;
    // gRTCDT.second = 0;
    // gRTC.setDateTime(gRTCDT);
    Serial.println(F("Setup date/time - 2018.01.01 00:00:00"));
  }
}

void loop() {
  static uint8_t previousSecond = 0;
  static uint32_t previousTemperatureMeasuring = 0;
  static uint32_t TemperatureMeasuring = 0;
  char lBuff[36];


  if (gRTC.sensorActivated()) {
    if (previousSecond != gRTC.getSecond())	{
      previousSecond = gRTC.getSecond();
      // получаем дату и время
      gRTCDT = gRTC.getDateTime();
      lBuff[0] = '\0';
      sprintf(lBuff, "%4d.%02d.%02d %02d:%02d:%02d", gRTCDT.year, gRTCDT.month, gRTCDT.day, gRTCDT.hour, gRTCDT.minute, gRTCDT.second);
      Serial.print(F("Date Time - "));  Serial.println(lBuff);
      TemperatureMeasuring = gRTC.getSecondFrom18Year();
      Serial.print(F("getSecondFrom18Year - "));  Serial.println(TemperatureMeasuring);
    }
    if (TemperatureMeasuring - previousTemperatureMeasuring > 64)	{
      previousTemperatureMeasuring = TemperatureMeasuring;
      lBuff[0] = '\0';
      uint16_t lTemperature =  gRTC.getTemperature();
      sprintf(lBuff, "%d.%02d", lTemperature / 100, lTemperature % 100);
      Serial.print(F("Temperature - "));  Serial.println(lBuff);
    }
  }
}