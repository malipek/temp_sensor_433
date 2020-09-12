# temp_sensor_433

## Project description
Project for Arduino hardware for sending temperature and humidity readings via 433MHz radio.

### Assumptions
* low energy consumption, battery powered
* low radiation
* no data encryption needed
* JSON data format

### Hardware used
* Arduino Pro Micro (Arduino Leonardo compatible)
* DHT22/AM2302 sensor
* FS1000A 433Mhz transmitter ASK modulation

### Low energy consumption approach
Arduino is reading the data form the sensor and transmits results via 433MHz transmitter.
After each transmission Arduino enables the power down mode for the longest period of Watchdog timer (8 seconds). Wakes up and if the couner hasn't reached the `SLEEP_TIME` value it enables the power down mode again.

## Power consumption
The average power consumption is 180uA, with 5mA peaks for transmission (Arduino has power LED removed).

## Known problems
The project was tested with Arduino Pro Micro (Arduino Leonardo clone). The reset is needed after powering the board up due to `dht11 start condition 1 not met` message on serial console in an infinite loop.

When powered up an reset corrctly, the first transmission should be sent within 5 seconds.