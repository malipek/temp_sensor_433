/*
 * Arduino Pro Micro receiving data via
 * XLC-RF-5V reciever.
 * Program must run i a loop, so no delays or putting the uC into
 * any power saving mode is possible.
*/

#include <RH_ASK.h> // https://www.airspayce.com/mikem/arduino/RadioHead/
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

#define RADIO 2 // Arduinio Pro Micro digital pin for XLC-RF-5V data
#define SPEED 1200 // ASK speed

// one way, pins 3 and 4 are unused
RH_ASK driver(SPEED,RADIO,3,4);

void setup()
{
    Serial.begin(9600);
    driver.init();
}

void loop()
{
  // buffer for message, max lengt same as on the transmitter's side
  uint8_t msg[32];
  uint8_t len;
  if (driver.recv(msg, &len))
  {
    short i;
    for (i=0; i<len; i++)
      Serial.print( char(msg[i]) );
    Serial.println();     
  }
}
