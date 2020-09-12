# Temperature sensor over 433 MHz

## Project description
Project for Arduino hardware for sending temperature and humidity readings via 433MHz radio. Second Arduino based appliance is receiving the data and sending to the Internet connected unit via serial port.

### Assumptions
* low energy consumption, battery powered tramsmitter
* low radiation (low RF energy)
* no data encryption
* JSON data format
* receiver connected with the USB with master computing unit
* energy consumption by the receiver is not so important but should be the lowest as possible

### Hardware used
* Two Arduino Pro Micro (Arduino Leonardo compatible)
* DHT22/AM2302 sensor
* 4,7kOhms pull-up resistor
* FS1000A 433Mhz transmitter
* 3xAAA cells box
* XLC-RF-5V 433Mhz receiver

### External libraries used
* [VirtualWire](https://www.airspayce.com/mikem/arduino/VirtualWire/)
* [LowPower](https://github.com/rocketscream/Low-Power.git)
* [DHT](https://github.com/nettigo/DHT.git)

### Low energy consumption approach
Arduino is reading the data form the sensor and transmits results via 433MHz transmitter.
After each transmission Arduino enables the power down mode for the longest period of Watchdog timer (8 seconds). Wakes up and if the couner hasn't reached the `SLEEP_TIME` value it enables the power down mode again.

## Power consumption

### Transmitter
The average power consumption is 180uA, with 5mA peaks for transmission (Arduino has power LED removed).

### Receiver
The average power consumption is 39mA. Arduino LED power is removed.

## Known problems
The project was tested with Arduino Pro Micro (Arduino Leonardo clone). The reset is needed after powering the board up due to `dht11 start condition 1 not met` message on serial console in an infinite loop.

When powered up an reset corrctly, the first transmission should be sent within 5 seconds.