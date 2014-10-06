// Sample RFM69 sender/node sketch, with ACK and optional encryption
// Sends periodic messages of increasing length to gateway (id=1)
// It also looks for an onboard FLASH chip, if present
// Library and code by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 and SPIFlash library at: https://github.com/LowPowerLab/
#include <RFM69.h>
#include <SPI.h>

#define NODEID        2    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define GATEWAYID     1
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

char payload[5];


char buff[20];
byte sendSize=5;
boolean requestACK = false;
RFM69 radio;

void setup() {
  Serial.begin(SERIAL_BAUD);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.encrypt(ENCRYPTKEY);
  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", 433);
  Serial.println(buff);
  
  payload[0] = FUNC_RAINBOW;
  payload[1] = 0;
  payload[2] = 0;
  payload[3] = 0;
  payload[4] = 50;
}

long lastPeriod = -1;
void loop() {
  //process any serial input
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    if (input >= 48 && input <= 57) //[0,9]
    {
      TRANSMITPERIOD = 100 * (input-48);
      if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 1000;
      Serial.print("\nChanging delay to ");
      Serial.print(TRANSMITPERIOD);
      Serial.println("ms\n");
    }

    if (input == 'r') { //d=dump register values
      radio.readAllRegs();
    }
    else if (input == 'x') {
      int currPeriod = millis()/TRANSMITPERIOD;
      if (currPeriod != lastPeriod)
      {
        lastPeriod=currPeriod;
        Serial.print("Sending[");
        Serial.print(sendSize);
        Serial.print("]: ");
        for(byte i = 0; i < sendSize; i++) {
          Serial.print((char)payload[i], HEX);
          Serial.print(" ");
        }

        if (radio.sendWithRetry(GATEWAYID, payload, sendSize)) {
          Serial.print(" ok!");
        }
        else {
          Serial.print(" nothing...");
        }
        Serial.println();

      }
    }
    
    //if (input == 'E') //E=enable encryption
    //  radio.encrypt(KEY);
    //if (input == 'e') //e=disable encryption
    //  radio.encrypt(null);
  }
  else {
    Serial.println("...");
    delay(1000);
  }

  //check for any received packets
  if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
    Serial.print((char)radio.DATA[i]);
    // Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    Blink(LED,5);
    Serial.println();
  }

  Blink(LED,3);
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}