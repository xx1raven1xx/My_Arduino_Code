#include <Adafruit_NeoPixel.h>

#define PIN 4
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
 
void setup() 
{
 pixels.begin();
 pixels.setBrightness(20); // Set brightness
 Serial.begin(9600);
}
 
void loop() 
{
 //rainbow(10);
 for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
  int pixelHue = firstPixelHue + (1 * 65536L / pixels.numPixels());
  pixels.setPixelColor(0, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  pixels.setPixelColor(1, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  pixels.setPixelColor(2, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  pixels.setPixelColor(3, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  pixels.show();
  delay(100);
  Serial.println(pixelHue);
 }
}


void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show();
    delay(wait);
  }
}