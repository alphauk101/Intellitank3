#ifndef dht_h
#define dht_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "defines.h"

#define DHT_LIB_VERSION "0.1.08"



class dht
{
public:
	int read11(uint8_t pin);

	double humidity;
	double temperature;

private:
	uint8_t bits[5];  // buffer to receive data
	int read(uint8_t pin);
};
#endif
