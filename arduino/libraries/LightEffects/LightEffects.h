#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_PIN 6

#define FUNC_NOOP					0x00 // a
#define FUNC_COLORWIPE              0x01 // b
#define FUNC_RAINBOW                0x02 // c
#define FUNC_RAINBOW_CYCLE          0x03 // d
#define FUNC_THEATRE_CHASE          0x04 // e
#define FUNC_THEATRE_CHASE_RAINBOW  0x05 // f
#define FUNC_SOLID_COLOUR           0x06 // g
#define FUNC_THROBBER               0x07 // h
#define FUNC_BLANK					0x08 // i


typedef uint32_t Colour;
typedef uint8_t Time;

class Light {
private:
	Adafruit_NeoPixel* neopixel;
	uint16_t iteration;
	uint16_t chaseThird;
	bool chaseShow;

	Colour colourWheel(byte);

public:
	Light(Adafruit_NeoPixel*);

	void runFunc(volatile byte*, bool);

	void blank();
	void colorWipe(Colour, Time);
	void rainbow(Time);
	void rainbowCycle(Time);
	void theaterChase(Colour, Time);
	void theaterChaseRainbow(Time);
};
