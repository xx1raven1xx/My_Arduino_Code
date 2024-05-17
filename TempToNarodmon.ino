// необходимо добавить в код загрузку по воздуху OTA.
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 15
#define TEMPERATURE_PRECISION 12 // Lower resolution
#define debug true // вывод отладочных сообщений
#define postingInterval  330000 // интервал между отправками данных в секундах (330 сек=5,5 минут)

// #define ssid  "Alex 2.4"
// #define password  "123456789"
#define ssid  "Pro8"
#define password  "123456789"
#define DHCP true

IPAddress local_IP(192, 168, 0, 61);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 0, 1);
IPAddress secondaryDNS(8, 8, 4, 4);
#define chanal 6//канал wifi
byte macAP[6] = {0xF8, 0x1A, 0x67, 0x86, 0x14, 0x70}; //mac роутера


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer = { 0x28, 0xC1, 0xB0, 0x03, 0x00, 0x00, 0x00, 0x60 };
String Hostname; //имя железки - выглядит как ESPAABBCCDDEEFF т.е. ESP+mac адрес.



void setup(void)
{
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  Serial.println("TempToNarodmon.ino");

  // Start up the library
  sensors.begin();
  sensors.setResolution(Thermometer, TEMPERATURE_PRECISION);
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    //Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
  SendToNarodmon();
  delay(postingInterval);
}



void wificonect() { // процедура подключения к Wifi.
  if (DHCP)  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    int i;
    if (debug) if (!(i % 100))Serial.print(".");
    delay(500);
    // для глубокого сна нужно замыкать RST на D0
    //i++; if (i > 3000)  ESP.deepSleep(60e6);//СПИМ 1 МИНУТу
  }
  if (debug) Serial.println("connected...");
  Hostname = "ESP" + WiFi.macAddress();
  Hostname.replace(":", "");// удаляем из названия двоеточия
  // WiFi.hostname(Hostname); // Название станции внутри локальной вайфай сети

  if (debug){ Serial.println(); Serial.print (millis()); Serial.println (" Подключено к wifi");}
}


bool SendToNarodmon() { // Собственно формирование пакета и отправка.
  //DeviceAddress tempDeviceAddress;

  wificonect();// подключаемся к сети
  if (debug) {
    Serial.println(WiFi.localIP()); Serial.println(WiFi.macAddress()); Serial.print("Narodmon ID: "); Serial.println(Hostname);
  }
  WiFiClient client;

  String buf;
  buf = "#" + Hostname + "\n"; //mac адрес для авторизации датчика

  client.connect("narodmon.ru", 8283);   // подключение

  if (debug) Serial.print(buf);


  //Данные от ESP ( Напряжение питания,уровень wifi
  //buf = buf + "#VCC#" + String(VCC) + "#Напряжение батареи\n"; //показания температуры
  buf = buf + "#WIFI#"  + String(WiFi.RSSI()) + "#Уровень WI-FI " + String(WiFi.SSID()) + "\n"; // уровень WIFI сигнала
  if (debug) Serial.print(buf);

  //DS18B20 в самом конце чтоб было время на измерения
  //    sensors.getAddress(tempDeviceAddress, i);
 
    buf = buf + "#T1#" + String(sensors.getTempCByIndex(0)) + "#DS18B20" + "\n"; //и температура


  String worcktime = String(millis());
  float WTime = worcktime.toInt(); WTime /= 1000;
  buf = buf + "#WORKTIME#"  + String(WTime) + "\n"; // уровень WIFI сигнала
  buf = buf + "##\n"; //окончание передачи

  client.print(buf); // и отправляем данные
  Serial.println("---------------Данные буфера!--------------");
  Serial.println(buf);
  Serial.println("---------------Данные буфера!--------------");


  delay(100);// сделать 100 если нужен ответ или 10 если не нужен . Время активности увеличивается в 2 раза
  while (client.available()) {
    String line = client.readStringUntil('\r'); // если что-то в ответ будет - все в Serial
    Serial.println(line);
  }
  client.stop();
  return true; //ушло
}
