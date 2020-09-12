/*
 * Arduino with DHT22/AM2302 sensor, sending data via
 * FS1000A 433Mhz transmitter after specified (approximatelly)
 * amount of time.
 * After each transmission Arduino goes to power down mode
 * for the longest period of Watchdog timer (8 seconds). Wakes up
 * and if the couner hasn't reached SLEEP_TIME it goes ti power down
 * mode again.
 * It results with the average power consumption of 180uA,
 * with 3mA peaks for transmission (Arduino has power LED removed).
 * Tested with Arduino Pro Micro 
 * (Arduino Leonardo clone). For Arduino Pro Micro
 * reset is needed after powering the board
 * due to "dht11 start condition 1 not met"
 * message on serial console in an infinite loop
 */


#include <VirtualWire.h> // https://www.airspayce.com/mikem/arduino/VirtualWire/
#include <LowPower.h> //https://github.com/rocketscream/Low-Power.git  
#include <DHT.h> // https://github.com/nettigo/DHT.git

#define RADIO 10 // Arduino pin for FS1000A data pin
#define SLEEP_TIME 200 //how many 8-9 second powerdown states
#define TEMP 5 // Arduino pin for DHT22/AM2302 data pin

// create global object for DHT22 sensor
DHT dht(TEMP, DHT22);

void setup()
{
    // FS1000A virtual wire setup
    vw_set_tx_pin(RADIO);
    vw_setup(2000);

    // DHT22 initialisation
    dht.begin();
}

// measure temp and humidity
// if succedeed 
// return JSON with results, empty string otherwise 

String measure(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h)) {
    return ""; 
  }
  else{ 
    return String("{\"temp\":"+String(t)+",\"hum\":"+String(h)+"}");
  }
}

// power the Arduino down
void suspend(){
  short i;
  for (i=0; i<SLEEP_TIME; i++) 
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
}

// transmit data
void transmit(String toSend){
  char msg[50]; // tworzymy tablicę typu char
  toSend.toCharArray(msg, toSend.length() + 1); 
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
}

// main loop
void loop()
{
  String measurement;
  measurement = measure();
  if (measurement != ""){
    transmit(measurement);
    // suspend only after successful measurement
    suspend(); 
  }
}
