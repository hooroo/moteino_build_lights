#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_PIN 6

#define FUNC_COLORWIPE              0x01
#define FUNC_RAINBOW                0x02
#define FUNC_RAINBOW_CYCLE          0x03
#define FUNC_THEATRE_CHASE          0x04
#define FUNC_THEATRE_CHASE_RAINBOW  0x05
#define FUNC_SOLID_COLOUR           0x06
#define FUNC_THROBBER               0x07

void light_setup(Adafruit_NeoPixel*);

void runFunc(Adafruit_NeoPixel *neopixel, volatile byte* input);

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel *neopixel, uint32_t c, uint8_t wait);

void rainbow(Adafruit_NeoPixel *neopixel, uint8_t wait);

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel *neopixel, uint8_t wait);

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel *neopixel, uint32_t c, uint8_t wait);

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel *neopixel, uint8_t wait);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel *neopixel, byte WheelPos);
