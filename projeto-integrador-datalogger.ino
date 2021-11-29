#include <SPI.h>
#include <SD.h>
//#include <DS1307.h>

#define pinChipSelectSD 4

File dataLogger;

enum sensorType {
  FuelSensor = 1,
  Speedometer = 2,
  Temperature = 3
} sensorsType;

//DS1307 rtc(A4, A5);

void setup() {
  startConfigure();
  sdConfigure();
//  rtcConfigure();
}

void loop() {
  fileOpen();
  
  if (dataLogger) {
    String dataRead = serialRead();

    if(dataRead.length() > 0){
      readData(dataRead);
    }
    dataLogger.close();
  }
}

//void rtcConfigure() {
//  rtc.halt(false);
//  rtc.setSQWRate(SQW_RATE_1);
//  rtc.enableSQW(true);
//}

void sdConfigure() {
  Serial.println("Iniciando SD");

  delay(1000);
  
  if (!SD.begin(pinChipSelectSD)) {
    Serial.println("Erro ao iniciar SD");
    while (1);
  }

  Serial.println("SD iniciado");
}

void startConfigure() {
  Serial.begin(9600);
}

void loggerFuelSensor(String dataFuel) {
//  String fuelSensorData = getDateTimeNow()+";"+dataFuel;
  String fuelSensorData = String(FuelSensor)+";"+dataFuel;
  dataLogger.println(fuelSensorData);

  Serial.print("Record: ");
  Serial.println(fuelSensorData);
}

void fileOpen() {
  dataLogger = SD.open("baja.csv", FILE_WRITE);

  if (dataLogger) {
    
  } else {
    Serial.println("Error opening...");
  }
}

//String getDateTimeNow() {
//  return String(rtc.getDateStr(FORMAT_SHORT))+"T"+String(rtc.getTimeStr());
//}

String serialRead() {
  String incomingData = "";
  if (Serial.available() > 0) {
    incomingData = Serial.readString();   
    Serial.print("incomingData: ");
    Serial.println(incomingData); 
  }
  return incomingData;
}

void readData(String data){
  Serial.print("ReadData: ");
  Serial.println(data);

  Serial.print("SensorType: ");
  Serial.println(data[0]);
  
  if(String(data[0]) == String(FuelSensor)){
    loggerFuelSensor(returnData(data));
  }
  else {
    Serial.println("Sensor Type Not Found!!");
  }
}

String returnData(String completeData){
  String justData = "";
  for(int i = 1; i<completeData.length(); i++){
    if(completeData[i] != '\r' && completeData[i] != '\n')
      justData += String(completeData[i]);
  }
  return justData;
}
