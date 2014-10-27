#include "LightEffects.h"

uint16_t iteration = 0;

void light_setup(Adafruit_NeoPixel *neopixel) {
  neopixel->begin();
  // neopixel->setBrightness(64);
  neopixel->show(); // Initialize all pixels to 'off'
}

void runFunc(Adafruit_NeoPixel *neopixel, volatile byte* input, bool reset) {
  if (reset) {
    iteration = 0;
    // blank(neopixel);
  }
  uint32_t colour = neopixel->Color(input[1], input[2], input[3]);
  uint8_t wait = (uint8_t) input[4];
  switch (input[0]) {
    case FUNC_BLANK:
      blank(neopixel);
      break;
    case FUNC_COLORWIPE:
      colorWipe(neopixel, colour, wait);
      break;
    // case FUNC_RAINBOW:
    //   rainbow(neopixel, wait);
    //   break;
    case FUNC_RAINBOW_CYCLE:
      rainbowCycle(neopixel, wait);
      break;
    case FUNC_THEATRE_CHASE:
      theaterChase(neopixel, colour, wait);
      break;
    // case FUNC_THEATRE_CHASE_RAINBOW:
    //   theaterChaseRainbow(neopixel, wait);
    //   break;
    default:
      break;
  }

  neopixel->show();
  // delay(wait);
}

void blank(Adafruit_NeoPixel *neopixel) {
  for(uint8_t i = 0; i < neopixel->numPixels(); i++) {
    neopixel->setPixelColor(i, neopixel->Color(1, 1, 1));
  }
}

// Fill the dots one after the other with a colour
void colorWipe(Adafruit_NeoPixel *neopixel, uint32_t c, uint8_t wait) {
  if(iteration < neopixel->numPixels()) {
    neopixel->setPixelColor(iteration, c);
    iteration++;
  }
}

// void rainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
//   uint16_t i, j;

//   if(iteration < 256) {
//     for(i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, Wheel(strip, (i+iteration) & 255));
//     }
//     strip.show();
//     iteration++;
//   }
// }

void rainbowCycle(Adafruit_NeoPixel* neopixel, uint8_t wait) {
  if (iteration < 256*5) {
    uint16_t i;
    for(i=0; i< neopixel->numPixels(); i++) {
      neopixel->setPixelColor(i, Wheel(neopixel, ((i * 256 / neopixel->numPixels()) + iteration) & 255));
    }
    iteration++;
  }
}

//Theatre-style crawling lights.
bool theaterChaseShow = true;
bool _theatreChaseShow = false;
void theaterChase(Adafruit_NeoPixel *neopixel, uint32_t c, uint8_t wait) {
  if (iteration < 10) {  //do 10 cycles of chasing
    for (uint16_t q=0; q < 3; q++) {
      if (theaterChaseShow) {
        for (uint16_t i=0; i < neopixel->numPixels(); i=i+3) {
          neopixel->setPixelColor(i+q, c);    //turn every third pixel on
        }
        neopixel->show();
      }
      else {
        for (uint16_t i=0; i < neopixel->numPixels(); i=i+3) {
          neopixel->setPixelColor(i+q, 1);        //turn every third pixel off
        }
      }
    }
    
    theaterChaseShow = _theatreChaseShow;
    _theatreChaseShow = !theaterChaseShow;
  }
}

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

