/*
 * Arduino receiving data via
 * XLC-RF-5V reciever with VirtualWire protocol.
 * Program must run i a loop, so no delays or putting the uC into
 * any power saving mode is possible.
*/

#include <VirtualWire.h> // https://www.airspayce.com/mikem/arduino/VirtualWire/

#define RADIO 2 // Arduino digital pin for XLC-RF-5V data

void setup()
{
    // setup serial port
    Serial.begin(9600);
 
    // setup VirtualWire library
    vw_set_rx_pin(RADIO);
    vw_setup(2000);
    vw_rx_start(); 
}

void loop()
{
  // częśc wymagana do poprawnego działania biblioteki
  
  // same max lentgh as in transmitter section
  uint8_t buf[50];
  uint8_t buflen = 50;

  if (vw_get_message(buf, &buflen)) //we got sth
  {
    int i;
    for (i = 0; i < buflen; i++) // get the buf
    {
      // print to serial converting ASCII code to char
      Serial.print(char(buf[i]));
    }
    Serial.println();
  }
}
