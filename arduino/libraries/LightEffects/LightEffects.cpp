#include "LightEffects.h"

void light_setup(Adafruit_NeoPixel strip) {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void runFunc(Adafruit_NeoPixel strip, char input[8]) {
  uint32_t colour = strip.Color(input[1], input[2], input[3]);
  uint8_t wait = (uint8_t) input[4];
  switch (input[0]) {
    case FUNC_COLORWIPE:
      colorWipe(strip, colour, wait);
      break;
    case FUNC_RAINBOW:
      rainbow(strip, wait);
      break;
    case FUNC_RAINBOW_CYCLE:
      rainbowCycle(strip, wait);
      break;
    case FUNC_THEATRE_CHASE:
      theaterChase(strip, colour, wait);
      break;
    case FUNC_THEATRE_CHASE_RAINBOW:
      theaterChaseRainbow(strip, wait);
      break;
    default:
      break;
  }
}


// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(Adafruit_NeoPixel strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for (uint16_t j=0; j<10; j++) {  //do 10 cycles of chasing
    for (uint16_t q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel strip, uint8_t wait) {
  for (uint16_t j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (uint16_t q=0; q < 3; q++) {
        for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel(strip, (i+j) % 255));    //turn every third pixel on
        }
        strip.show();

        delay(wait);

        for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel strip, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

