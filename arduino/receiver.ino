// Sample RFM69 receiver/gateway sketch, with ACK and optional encryption
// Passes through any wireless received messages to the serial port & responds to ACKs
// It also looks for an onboard FLASH chip, if present
// Library and code by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 and SPIFlash library at: https://github.com/LowPowerLab/

#include <RFM69.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <LightEffects.h>

#define NODEID        1    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define ACK_TIME      30 // max # of ms to wait for an ack
#define SERIAL_BAUD   115200

#define LED           9 // Moteinos have LEDs on D9

RFM69 radio;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.encrypt(ENCRYPTKEY);
  radio.promiscuous(false);
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);
}

void Blinker(byte pin, int delay_ms) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(delay_ms);
  digitalWrite(pin, LOW);
}

byte ackCount=0;
void loop() {
  if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");

    for (byte i = 0; i < radio.DATALEN; i++) {
      Serial.print((char)radio.DATA[i]);
    }

    Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");

    ExchangeAck();
    Serial.println();
    Blinker(LED,3);
  }
}

void ExchangeAck() {
  if (radio.ACKRequested())
  {
    byte theNodeID = radio.SENDERID;
    radio.sendACK();
    Serial.print(" - ACK sent.");

    // When a node requests an ACK, respond to the ACK
    // and also send a packet requesting an ACK (every 3rd one only)
    // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
    if (ackCount++%3==0)
    {
      Serial.print(" Pinging node ");
      Serial.print(theNodeID);
      Serial.print(" - ACK...");
      delay(3); //need this when sending right after reception .. ?
      if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0)) {  // 0 = only 1 attempt, no retries
        Serial.print("ok!");
      } else {
        Serial.print("nothing");
      }
    }
  }
}

