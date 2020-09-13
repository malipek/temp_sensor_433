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

#include <RH_ASK.h> // https://www.airspayce.com/mikem/arduino/RadioHead/
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

#include <LowPower.h> //https://github.com/rocketscream/Low-Power.git  
#include <DHT.h> // https://github.com/nettigo/DHT.git

#define RADIO 10 // Arduino pin for FS1000A data pin
#define SLEEP_TIME 200 //how many 8-9 second powerdown states
#define TEMP 5 // Arduino pin for DHT22/AM2302 data pin
#define SPEED 1200 // ASK speed

// create global object for DHT22 sensor
DHT dht(TEMP, DHT22);

// create global object for RH library

// one way, pins 2 and 3 are unused
RH_ASK driver(SPEED,2,RADIO,3);

void setup()
{

    // DHT22 initialisation
    dht.begin();
    
    // RH initialisation
    driver.init();
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
    return "{\"temp\":"+String(t)+",\"hum\":"+String(h)+"}";
  }
}

// power the Arduino down
void suspend(){
  short i;
  for (i=0; i<SLEEP_TIME; i++) 
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
}

// transmit data
void transmit(String msg){
  char char_msg[50];
  msg.toCharArray(char_msg, msg.length()+1);
  driver.send((uint8_t *)char_msg, strlen(char_msg));
  driver.waitPacketSent();
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
