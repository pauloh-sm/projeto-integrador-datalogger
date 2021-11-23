#include <SPI.h>
#include <SD.h>
#include <DS1307.h>

#define pinChipSelectSD 4

File dataLogger;

enum sensorType {
  FuelSensor = 1,
  Speedometer = 2,
  Temperature = 3
} sensorsType;

DS1307 rtc(A4, A5);

void setup() {
  startConfigure();
  sdConfigure();
  rtcConfigure();
}

void loop() {
  dataLogger = fileOpen();
  
  if (dataLogger) {
    String dataRead = serialRead();

    if(dataRead.length() > 0){
      readData(dataRead);
    }    
  }

  fileClose(dataLogger);
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

void startConfigure() {
  Serial.begin(9600);
}

void loggerFuelSensor(String dataFuel) {
  String fuelSensorData = getDateTimeNow()+";"+dataFuel;
  dataLogger.println(fuelSensorData);
}

void loggerSpeedometerSensor(File dataLogger) {
//  byte fuelSpeedometerState = digitalRead(pinSpeedometerSensor);
//  String fuelSensorData = getDateTimeNow()+";"+String(Speedometer)+";"+String(fuelSensorState);
//  dataLogger.println(fuelSensorData);
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

String serialRead() {
  String incomingData = "";
  if (Serial.available() > 0) {
    incomingData = Serial.readString();    
  }

  return incomingData;
}

void readData(String data){
  if(data[0] == (char)FuelSensor){
    loggerFuelSensor(data);
  }
}
