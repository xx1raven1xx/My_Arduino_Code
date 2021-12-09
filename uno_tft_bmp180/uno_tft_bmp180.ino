
#include <UTFTGLUE.h>  // class methods are in here
//#include <UTFT.h>
#include <Adafruit_BMP085.h>

//UTFTGLUE myGLCD(0x0154,A2,A1,A3,A4,A0); // было так
UTFTGLUE myGLCD(0x0154, A2, A1, A3, A4, A0);
//UTFT myGLCD(0x0154, A2, A1, A3, A4, A0);
Adafruit_BMP085 bmp;

//extern uint8_t SmallFont[];
extern uint8_t BigFont[];
int i, t, tmr, a, z;
int x, x2;
int y, y2;
int mn, mx;
long temp_array[318];
long pressure_array[318], aver_pressure, prs, tmp;
boolean upd;

void setup() {
  Serial.begin(9600);
  Serial.println("testing...");
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  randomSeed(analogRead(0));
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);

  // Setup the LCD
  myGLCD.InitLCD();
  //myGLCD.setFont(SmallFont);
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  t = millis();
  tmr = millis();
  Serial.println("Start!");
}

void loop() {
    To_LCD();
tmp = bmp.readTemperature();
prs = bmp.readPressure();
/*
//------------------------------------------------------
  if (upd == true) {
    To_LCD();
Serial.print("1");
    // добавлять данные в график каждую минуту (60000)
    if (millis() - t > 6000) {
      i++;
      if (i >= 318) {
        i = 0;
      }
      pressure_array[318] = bmp.readPressure();  //map(pressure, 101192, 102127, 47, 17);
      for (int z = 0; z < 318; z++) {
        pressure_array[z] = pressure_array[z + 1];  // сдвинуть массив давлений КРОМЕ ПОСЛЕДНЕЙ ЯЧЕЙКИ на шаг назад
        Serial.println(pressure_array[z]);
      }
      t = millis();
      Serial.print("2");
    }
    // перерисовка графика каждый цикл программы
    myGLCD.setColor(0, 0, 255);
    for (byte a = 0; a < 318; a++) {
      myGLCD.drawPixel(a, map(pressure_array[a], mn, mx, 47, 17));
      Serial.print("3");
    }
    
    // вычисление MIN MAX значений
    mn = pressure_array[0];
    mx = pressure_array[0];
    for (int a = 0; a < 318; a++) {
      if (mn > pressure_array[a])
        mn = pressure_array[a];
      if (mx < pressure_array[a])
        mx = pressure_array[a];
        Serial.print("4");
    }
  
    //display.display();   // тут вроде нет команды обновления дисплея
    upd = false;
    Serial.print("5");
  }
  if (millis() - tmr > 200) {
    myGLCD.clrScr();
    tmr = millis();
    upd = true;
    Serial.print("6");
  }
//------------------------------------------------------
*/


delay(1000);
/*
      pressure_array[318] = prs;  //map(pressure, 101192, 102127, 47, 17);
      Serial.println("1");
      for (z = 0; z < 318; z++) {
        //pressure_array[z] = pressure_array[z + 1];  // сдвинуть массив давлений КРОМЕ ПОСЛЕДНЕЙ ЯЧЕЙКИ на шаг назад
        //Serial.println(pressure_array[z]);
      }*/
  delay(1000);
//Serial.print("7");

 //Serial.print(t);
 //Serial.println(tmr);
}
/*

  //myGLCD.clrScr(); // долгое обновление экрана
  //myGLCD.setColor(0,0,0);
  //myGLCD.fillRect(1,15,318,224);
  //myGLCD.setColor(0, 0, 255);
  //myGLCD.setBackColor(0, 0, 0);
  //myGLCD.drawLine(159, 15, 159, 224);
  //myGLCD.drawLine(1, 119, 318, 119);
  //delay(2000);
// Draw crosshairs
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);
  for (int i=9; i<310; i+=10)
    myGLCD.drawLine(i, 117, i, 121);
  for (int i=19; i<220; i+=10)
    myGLCD.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  myGLCD.setColor(0,255,255);
  myGLCD.print("Sin", 5, 15);
  for (int i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
  }
  
  myGLCD.setColor(255,0,0);
  myGLCD.print("Cos", 5, 27);
  for (int i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
  }

  myGLCD.setColor(255,255,0);
  myGLCD.print("Tan", 5, 39);
  for (int i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
  }

  delay(2000);

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<(318*20); i++) 
  {
    x++;
    if (x==319)
      x=1;
    if (i>319)
    {
      if ((x==159)||(buf[x-1]==119))
        myGLCD.setColor(0,0,255);
      else
        myGLCD.setColor(0,0,0);
      myGLCD.drawPixel(x,buf[x-1]);
    }
    myGLCD.setColor(0,255,255);
    y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    myGLCD.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
  }
  
  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillCircle(100+(i*20),60+(i*20), 30);
  }
  
  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some lines in a pattern
  myGLCD.setColor (255,0,0);
  for (int i=15; i<224; i+=5)
  {
    myGLCD.drawLine(1, i, (i*1.44)-10, 224);
  }
  myGLCD.setColor (255,0,0);
  for (int i=224; i>15; i-=5)
  {
    myGLCD.drawLine(318, i, (i*1.44)-11, 15);
  }
  myGLCD.setColor (0,255,255);
  for (int i=224; i>15; i-=5)
  {
    myGLCD.drawLine(1, i, 331-(i*1.44), 15);
  }
  myGLCD.setColor (0,255,255);
  for (int i=15; i<224; i+=5)
  {
    myGLCD.drawLine(318, i, 330-(i*1.44), 224);
  }
  
  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,225);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=32+random(256);
    y=45+random(146);
    r=random(30);
    myGLCD.drawCircle(x, y, r);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    myGLCD.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    myGLCD.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(209);
    x2=2+random(316);
    y2=16+random(209);
    myGLCD.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

  for (int i=0; i<10000; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    myGLCD.drawPixel(2+random(316), 16+random(209));
  }

  delay(2000);

  myGLCD.fillScr(0, 0, 255);
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(80, 70, 239, 169);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("That's it!", CENTER, 93);
  myGLCD.print("Restarting in a", CENTER, 119);
  myGLCD.print("few seconds...", CENTER, 132);
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Runtime: (msecs)", CENTER, 210);
  myGLCD.printNumI(millis(), CENTER, 225);
  
  delay (10000);
}
*/
