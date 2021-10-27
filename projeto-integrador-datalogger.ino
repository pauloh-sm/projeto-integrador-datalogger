#include <SPI.h>
#include <SD.h>
#include <DS1307.h>

#define pinChipSelectSD 4
#define pinFuelSensor 5

enum sensorType {
  FuelSensor = 1
} sensorsType;

DS1307 rtc(A4, A5);

void setup() {
  pinConfigure();
  sdConfigure();
  rtcConfigure();
}

void loop() {
  File dataLogger = fileOpen();
  
  if (dataLogger) {
    loggerFuelSensor(dataLogger);
    fileClose(dataLogger);
  }

  delay(5000);
}

void rtcConfigure() {
  rtc.halt(false);
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
}

void sdConfigure() {
  if (!SD.begin(pinChipSelectSD)) {
    while (1);
  }
}

void pinConfigure() {
  pinMode(pinFuelSensor, INPUT_PULLUP);
}

void loggerFuelSensor(File dataLogger) {
  byte fuelSensorState = digitalRead(pinFuelSensor);
  String fuelSensorData = getDateTimeNow()+";"+String(FuelSensor)+";"+String(fuelSensorState);
  dataLogger.println(fuelSensorData);
}

File fileOpen() {
  return SD.open("baja_data_logger.csv", FILE_WRITE);
}

void fileClose(File dataLogger) {
  dataLogger.close();
}

String getDateTimeNow() {
  return String(rtc.getDateStr(FORMAT_SHORT))+"T"+String(rtc.getTimeStr());
}
