// Sample RFM69 sender/node sketch, with ACK and optional encryption
// Sends periodic messages of increasing length to gateway (id=1)
// It also looks for an onboard FLASH chip, if present
// Library and code by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 and SPIFlash library at: https://github.com/LowPowerLab/
#include <RFM69.h>
#include <SPI.h>

#define NODEID        2    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define RECIPIENT     1
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define ACK_TIME      30 // max # of ms to wait for an ack

#define LED           9 // Moteinos have LEDs on D9

#define SERIAL_BAUD   115200

#define FUNC_COLORWIPE              0x01
#define FUNC_RAINBOW                0x02
#define FUNC_RAINBOW_CYCLE          0x03
#define FUNC_THEATRE_CHASE          0x04
#define FUNC_THEATRE_CHASE_RAINBOW  0x05
#define FUNC_SOLID_COLOUR           0x06
#define FUNC_THROBBER               0x07

int TRANSMITPERIOD = 300; //transmit a packet to gateway so often (in ms)

uint8_t recipient = 0;

char payload[5];


char buff[20];
byte sendSize = 5;
boolean requestACK = false;
RFM69 radio;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.encrypt(ENCRYPTKEY);
  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", 433);
  Serial.println(buff);

  payload[0] = FUNC_RAINBOW_CYCLE;
  payload[1] = 128;
  payload[2] = 128;
  payload[3] = 0;
  payload[4] = 50;
}

long lastPeriod = -1;
void loop()
{
  //process any serial input
  if (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input == 'x')
    {
      recipient = RECIPIENT;
      int currPeriod = millis() / TRANSMITPERIOD;
      if (currPeriod != lastPeriod)
      {
        input = Serial.read();
        payload[0] = input - 'a';

        input = Serial.read();
        if (input == 'r')
        {
          payload[1] = 128;
          payload[2] = 0;
          payload[3] = 0;
        }
        else if (input == 'g')
        {
          payload[1] = 0;
          payload[2] = 128;
          payload[3] = 0;
        }
        else if (input == 'b')
        {
          payload[1] = 0;
          payload[2] = 0;
          payload[3] = 128;
        }
        else
        {
          payload[1] = 128;
          payload[2] = 128;
          payload[3] = 0;
        }

        lastPeriod = currPeriod;
        send(recipient, payload);

      }
    }
    else if (input == 's')
    {
      recipient = 0;
      int currPeriod = millis() / TRANSMITPERIOD;
      if (currPeriod != lastPeriod)
      {
        readRecipient();

        payload[0] = Serial.read() - 'a';

        if (Serial.available() > 0)
        {
          readColour();
          readTiming();
        }
        else {
          payload[1] = 0x7F;
          payload[2] = 0x7F;
          payload[3] = 0x7F;
          payload[4] = 0x50;
        }

        send(recipient, payload);

      }
    }
  }
  else
  {
    Serial.println("...");
    delay(1000);
  }

//check for any received packets
  if (radio.receiveDone())
  {
    Serial.print('[');
    Serial.print(radio.SENDERID, DEC);
    Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    Blink(LED, 5);
    Serial.println();
  }

  Blink(LED, 3);
}

void send(uint8_t recipient, char payload[5])
{
  Serial.print("Sending[");
  Serial.print(sendSize);
  Serial.print("]: ");
  for(byte i = 0; i < sendSize; i++)
  {
    Serial.print((char)payload[i], HEX);
    Serial.print(" ");
  }
  if (radio.sendWithRetry(recipient, payload, sendSize))
  {
    Serial.print(" ok!");
  }
  else
  {
    Serial.print(" nothing...");
  }
  Serial.println();
}

void readRecipient()
{
  byte numstr[2];
  
  numstr[0] = Serial.read();
  numstr[1] = Serial.read();
  recipient = parseHexStr(numstr);
}

void readColour()
{
  byte numstr[2];
  if (Serial.peek() == '#')
  {
    Serial.read();
    numstr[0] = Serial.read();
    numstr[1] = Serial.read();
    payload[1] = parseHexStr(numstr);


    numstr[0] = Serial.read();
    numstr[1] = Serial.read();
    payload[2] = parseHexStr(numstr);

    numstr[0] = Serial.read();
    numstr[1] = Serial.read();
    payload[3] = parseHexStr(numstr);
  }
  else
  {
    payload[1] = 0x00;
    payload[2] = 0x00;
    payload[3] = 0x00;
  }
}

void readTiming()
{
  uint8_t time = 0;

  if (Serial.peek() == 't')
  {
    Serial.read();
    while (Serial.available() > 0)
    {
      time *= 10;
      time += parseDecDigit(Serial.read());
    }

    payload[4] = time;
  }
  else
  {
    payload[4] = 50;
  }
}

uint8_t parseHexDigit(byte hex)
{
  hex -= '0'; // ASCII offset of '0' from 0x0
  if (hex > 9)
  {
    hex -= ('A' - '0'); // ASCII offset of 'A' from '0'
    if (hex > 5)
    {
      hex -= ('a' - 'A'); //ASCII offset of 'a' from 'A'
    }
    hex += 0xA;
  }

  return (uint8_t) hex;
}

uint8_t parseHexStr(byte hexstr[2])
{
  return (parseHexDigit(hexstr[0]) << 4) + parseHexDigit(hexstr[1]);
}

uint8_t parseDecDigit(byte digit)
{
  digit -= 0x30;
  if (digit > 9)
  {
    digit = 9; // Clamp anything higher than 9
  }

  return (uint8_t) digit;
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN, LOW);
}