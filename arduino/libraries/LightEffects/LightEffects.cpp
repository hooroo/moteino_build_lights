#include "LightEffects.h"

uint16_t iteration = 0;

void light_setup(Adafruit_NeoPixel *neopixel) {
  neopixel->begin();
  // neopixel->setBrightness(100);
  neopixel->show(); // Initialize all pixels to 'off'
}

void runFunc(Adafruit_NeoPixel *neopixel, volatile byte* input, bool reset) {
  if (reset) {
    iteration = 0;
  }
  uint32_t colour = neopixel->Color(input[1], input[2], input[3]);
  uint8_t wait = (uint8_t) input[4];
  switch (input[0]) {
    case FUNC_COLORWIPE:
      colorWipe(strip, colour, wait);
      break;
    // case FUNC_RAINBOW:
    //   rainbow(strip, wait);
    //   break;
    case FUNC_RAINBOW_CYCLE:
      rainbowCycle(neopixel, wait);
      break;
    // case FUNC_THEATRE_CHASE:
    //   theaterChase(strip, colour, wait);
    //   break;
    // case FUNC_THEATRE_CHASE_RAINBOW:
    //   theaterChaseRainbow(strip, wait);
    //   break;
    default:
      break;
  }
}


// Fill the dots one after the other with a colour
void colorWipe(Adafruit_NeoPixel *strip, uint32_t c, uint8_t wait) {
  if(iteration < strip.numPixels()) {
      strip.setPixelColor(iteration, c);
      strip.show();
      iteration++;
  }
  else {
    iteration = 0;
  }
}

// void rainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
//   uint16_t i, j;

//   for(j=0; j<256; j++) {
//     for(i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
//     }
//     strip.show();
//     delay(wait);
//   }
// }

// Slightly different, this makes the rainbow equally distributed throughout
// void rainbowCycle(Adafruit_NeoPixel *neopixel, uint8_t wait) {
//   uint16_t iteration;

//   for(iteration=0; iteration<256*5; iteration++) { // 5 cycles of all colors on wheel
//     _innerRainbowCycle(neopixel, iteration);
//     delay(wait);
//   }
// }

void _innerRainbowCycle(Adafruit_NeoPixel * neopixel) {
  uint16_t i;
  for(i=0; i< neopixel->numPixels(); i++) {
      neopixel->setPixelColor(i, Wheel(neopixel, ((i * 256 / neopixel->numPixels()) + iteration) & 255));
    }
    neopixel->show();
}

void rainbowCycle(Adafruit_NeoPixel* neopixel, uint8_t wait) {
  if (iteration < 256*5) {
    _innerRainbowCycle(neopixel);
    iteration++;
  }
}

// //Theatre-style crawling lights.
// void theaterChase(Adafruit_NeoPixel *strip, uint32_t c, uint8_t wait) {
//   for (uint16_t j=0; j<10; j++) {  //do 10 cycles of chasing
//     for (uint16_t q=0; q < 3; q++) {
//       for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//         strip.setPixelColor(i+q, c);    //turn every third pixel on
//       }
//       strip.show();

//       delay(wait);

//       for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//         strip.setPixelColor(i+q, 0);        //turn every third pixel off
//       }
//     }
//   }
// }

// //Theatre-style crawling lights with rainbow effect
// void theaterChaseRainbow(Adafruit_NeoPixel strip, uint8_t wait) {
//   for (uint16_t j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//     for (uint16_t q=0; q < 3; q++) {
//         for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//           strip.setPixelColor(i+q, Wheel(strip, (i+j) % 255));    //turn every third pixel on
//         }
//         strip.show();

//         delay(wait);

//         for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//           strip.setPixelColor(i+q, 0);        //turn every third pixel off
//         }
//     }
//   }
// }

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel *neopixel, byte WheelPos) {
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

