// разрешение дисплея 84*48
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include "SparkFunBME280.h"

// если закоментировать данный дефайн, то DEBUG(x) не будет заменятся выводом в сериал
// #define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif


Adafruit_PCD8544 display = Adafruit_PCD8544(18, 23, 19, 5, 14);

BME280 mySensor; //Uses I2C address 0x76 (jumper closed)
const int ledPin1 = 27;

byte i;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
unsigned long pressure_array[83], val, aver_pressure, pressure, t, tmr;
int mn, mx;
boolean recievedFlag, upd;
String strData = "";

void setup() {
  Serial.begin(9600);
  display.begin();
  display.setContrast(60);
  display.display(); // заставка дисплея
  delay(2000);
  display.clearDisplay();
  Wire.begin();
  mySensor.setI2CAddress(0x76);
  mySensor.beginI2C();
  t = millis();
  tmr = millis();
  // почему то первое чтение давления идет неправильное, старался избежать этого таким методом но не помогло.
  for (byte a = 0; a < 83; a++)
  {
    pressure_array[a] = aver_sens();
  }

DEBUG("-----------------");
  // тут тоже старался избежать неправильного значения первого чтения давления
for (byte i = 0; i < 10; i ++)
{
  delay(1000);
    for (byte a = 0; a < 83; a++)
  {
    pressure_array[a] = aver_sens();
  }
}

}

void loop() {
  if (upd == true)
  { 
  To_LCD();

  // добавлять данные в график каждую минуту (60000)
    if (millis() - t > 60000)
    {
      i++;
      if (i >= 83)
        {
          i = 0;
        }
      pressure_array[83] = pressure; //map(pressure, 101192, 102127, 47, 17);
      for (byte z = 0; z < 83; z++) {
        pressure_array[z] = pressure_array[z + 1];     // сдвинуть массив давлений КРОМЕ ПОСЛЕДНЕЙ ЯЧЕЙКИ на шаг назад
        Serial.println(pressure_array[z]);
      }
      t = millis();
    }
    // перерисовка графика каждый цикл программы
    for (byte a = 0; a < 83; a++)
      {
        display.drawLine(a, map(pressure_array[a], mn, mx, 47, 17), a, 47, BLACK);
      }

mn = pressure_array[0];
mx = pressure_array[0];
for (byte a = 0; a < 83; a++){
if (mn > pressure_array[a])
    mn =pressure_array[a];
if (mx < pressure_array[a])
    mx = pressure_array[a];
}

    DEBUG("----------------");
    DEBUG(mn);
    DEBUG(mx);

    display.display();
    upd = false;
 }
 if (millis() - tmr > 200){
      display.clearDisplay();
      tmr = millis();
      upd = true;
 }

//---------------------------------------------------------------------
// Чтение данных и сериал порта. Обработка команд сериал порта.
if (Serial.available() > 0) {                 // если есть что то на вход
    strData = "";                               // очистить строку
    while (Serial.available() > 0) {            // пока идут данные
      strData += (char)Serial.read();           // забиваем строку принятыми данными
      delay(2);                                 // обязательно задержка, иначе вылетим из цикла раньше времени
    }
    recievedFlag = true;                        // поднять флаг что получили данные
  }
  if (recievedFlag) {                           // если есть принятые данные
    int intVal = strData.substring(2).toInt();  // перевести в int всю строку кроме первых двух символов!
    String header = strData.substring(0, 2);    // создать мини строку, содержащую первые два символа
    if (strData.startsWith("mn"))               // если строка начинается с mn
      mn = intVal;
    if (strData.startsWith("mx"))               // если строка начинается с mx
      mx = intVal;
    if (strData.startsWith("ar")) {              // если строка начинается с ar
      int intIndx = strData.substring(2,4).toInt();  
      int intValue = strData.substring(4).toInt();
      pressure_array[intIndx] = intValue;
    }
    if (strData.startsWith("ra")) {              // если строка начинается с ar
      int intIndx = strData.substring(2).toInt();
      Serial.println(pressure_array[intIndx]);
    }
    if (strData.startsWith("hg"))
      Serial.println(float(intVal)*133.322F);
    if (strData.startsWith("mm"))
      Serial.println(float(intVal)*0.75F);
    

    recievedFlag = false;                       // данные приняты, мой капитан!
    // выводим в порт для отладки
    //Serial.print(EEPROM.read(0)); Serial.print(" ");
    //Serial.print(EEPROM.read(1)); Serial.println(" ");
    //Serial.print(button1); Serial.print(" ");
    //Serial.print(button2); Serial.println();
  }
//---------------------------------------------------------------------

}


void To_LCD() {
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.print("Pres:");
    pressure = aver_sens();
    display.println(pressure / 100.0F * 0.75F, 2);
    display.print("Temp: ");
    display.println(mySensor.readTempC(), 2);
    display.drawLine(0, 17, 0, 47, BLACK);
    display.drawLine(display.width() - 1, 47, 0, 47, BLACK);
}

// усреднение вывода давнелия
long aver_sens() {
  pressure = 0;
  for (byte i = 0; i < 10; i++) {
    pressure += mySensor.readFloatPressure();
  }
  aver_pressure = pressure / 10;
  return aver_pressure;
}
