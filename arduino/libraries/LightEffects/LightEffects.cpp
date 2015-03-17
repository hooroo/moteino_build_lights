#include "LightEffects.h"

uint16_t iteration = 0;

Light::Light(Adafruit_NeoPixel* pixel) : neopixel(pixel), iteration(0), chaseThird(0) {
  neopixel->begin();
  // neopixel->setBrightness(64);
  neopixel->show(); // Initialize all pixels to 'off'
};

void Light::runFunc(volatile byte* input, bool reset) {
  if (reset) {
    iteration = 0;
    // blank(neopixel);
  }
  Colour colour = neopixel->Color(input[1], input[2], input[3]);
  Time wait = (Time) input[4];
  switch (input[0]) {
    case FUNC_BLANK:
      blank();
      break;
    case FUNC_COLORWIPE:
      colorWipe(colour, wait);
      break;
    case FUNC_RAINBOW:
      rainbow(wait);
      break;
    case FUNC_RAINBOW_CYCLE:
      rainbowCycle(wait);
      break;
    case FUNC_THEATRE_CHASE:
      theaterChase(colour, wait);
      break;
    case FUNC_THEATRE_CHASE_RAINBOW:
      theaterChaseRainbow(wait);
      break;
    default:
      break;
  }

  neopixel->show();
}

void Light::blank() {
  for(uint8_t i = 0; i < neopixel->numPixels(); i++) {
    neopixel->clear();
  }
}

// Fill the dots one after the other with a colour
void Light::colorWipe(Colour c, Time wait) {
  if(iteration < neopixel->numPixels()) {
    neopixel->setPixelColor(iteration, c);
    iterate();
  }
}

void Light::rainbow(Time wait) {
  uint16_t i;

  for(i=0; i<neopixel->numPixels(); i++) {
    neopixel->setPixelColor(i, colourWheel((i+iteration) & 255));
  }
  iterate();
}

void Light::rainbowCycle(Time wait) {
  uint16_t i;
  for(i=0; i< neopixel->numPixels(); i++) {
    neopixel->setPixelColor(i, colourWheel(((i * 256 / neopixel->numPixels()) + iteration) & 255));
  }

  iterate();
}

//Theatre-style crawling lights.
void Light::theaterChase(Colour c, Time wait) {
  uint16_t i;
  neopixel->clear();
  for (i=0; i < neopixel->numPixels(); i=i+3) {
    neopixel->setPixelColor(i+chaseThird, c);    //turn every third pixel on
  }

  chase();
  delay(wait);
}

// //Theatre-style crawling lights with rainbow effect
void Light::theaterChaseRainbow(Time wait) {
  uint16_t i;
  neopixel->clear();
  for (i=0; i < neopixel->numPixels(); i=i+3) {
    neopixel->setPixelColor(i+chaseThird, colourWheel((iteration) % 255));    //turn every third pixel on
  }
  neopixel->show();

  chase();
  iterate();
  delay(wait);
}

void Light::chase(void) {
  ++chaseThird;
  if (chaseThird >= 3) { chaseThird = 0; }
}

void Light::iterate(void) {
  ++iteration;
  if (iteration >= 256) { iteration = 0; }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Light::colourWheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return neopixel->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return neopixel->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return neopixel->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

