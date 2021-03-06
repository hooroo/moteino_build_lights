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

#define NEO_PIN       5 // Arbitrary pin assignment.

RFM69 radio;
Adafruit_NeoPixel wheel = Adafruit_NeoPixel(24, NEO_PIN, NEO_GRB + NEO_KHZ800);
Light* light = NULL;

volatile byte receive_buffer[5];


void setup() {
  Serial.begin(SERIAL_BAUD);
  light = new Light(&wheel);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.encrypt(ENCRYPTKEY);
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", 433);
  Serial.println(buff);
}

byte ackCount=0;
void loop() {
  bool reset = false;

  if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++) {
      Serial.print((char)radio.DATA[i], HEX);
      Serial.print(" ");
    }

    if (radio.DATALEN == 5) {
      Serial.println("running function...");
      for (int bit=0; bit < radio.DATALEN; bit++) {
        receive_buffer[bit] = radio.DATA[bit];
        reset = true;
      }
      // rainbowCycle(&wheel, 50);
    }

    
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
        if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0)) { // 0 = only 1 attempt, no retries
          Serial.print("ok!");
        }
        else {
          Serial.print("nothing");
        }
      }
    }

    // Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
    

    Serial.println();
  }

  light->runFunc(receive_buffer, reset);
  Serial.println("function ran");

  Time wait = receive_buffer[4];

  Blink(LED, 3);
  delay(wait);
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}