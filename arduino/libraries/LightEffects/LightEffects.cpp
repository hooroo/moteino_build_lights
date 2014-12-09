#include "LightEffects.h"

uint16_t iteration = 0;

Light::Light(Adafruit_NeoPixel* pixel) : neopixel(pixel), iteration(0) {
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
    // case FUNC_RAINBOW:
    //   rainbow(neopixel, wait);
    //   break;
    case FUNC_RAINBOW_CYCLE:
      rainbowCycle(wait);
      break;
    case FUNC_THEATRE_CHASE:
      theaterChase(colour, wait);
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

void Light::blank() {
  for(uint8_t i = 0; i < neopixel->numPixels(); i++) {
    neopixel->clear();
  }
}

// Fill the dots one after the other with a colour
void Light::colorWipe(Colour c, Time wait) {
  if(iteration < neopixel->numPixels()) {
    neopixel->setPixelColor(iteration, c);
    iteration++;
  }
}

// void rainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
//   uint16_t i, j;

//   if(iteration < 256) {
//     for(i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, colourWheel(strip, (i+iteration) & 255));
//     }
//     strip.show();
//     iteration++;
//   }
// }

void Light::rainbowCycle(Time wait) {
  if (iteration < 256*5) {
    uint16_t i;
    for(i=0; i< neopixel->numPixels(); i++) {
      neopixel->setPixelColor(i, colourWheel(((i * 256 / neopixel->numPixels()) + iteration) & 255));
    }
    iteration++;
  }
}

//Theatre-style crawling lights.
void Light::theaterChase(Colour c, Time wait) {
  static bool theaterChaseShow = true;
  static bool _theatreChaseShow = false;

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
//           strip.setPixelColor(i+q, colourWheel(strip, (i+j) % 255));    //turn every third pixel on
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

