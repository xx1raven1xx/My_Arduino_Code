

void To_LCD() {
  myGLCD.setFont(BigFont);
  myGLCD.setTextSize(4);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.printNumF(bmp.readTemperature(), 2, 15, 3);

  myGLCD.setColor(255, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.printNumF(bmp.readPressure()/133.33, 2, 15, 80);
/*
  myGLCD.setBackColor(100, 100, 100);
  myGLCD.setColor(255,255,0);
  myGLCD.print("Testing to graph", CENTER, 227);
  myGLCD.print("Привет МИР!", CENTER, 200);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 319, 225);  // область где будем рисовать график!!!
  delay(500);
  */
}
