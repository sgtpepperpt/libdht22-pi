// From https://github.com/nebulx29/dht22/blob/master/dht22.c

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#include "dht.h"

#define MAX_TIMINGS	85
#define WAIT_TIME 2000
#define MAX_RETRIES	5

int read_dht_data(int dht_pin, float* temperature, float* humidity) {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi\n");
        return 1;
    }

    int retries = 0;
    while (retries++ < MAX_RETRIES) {
        int data[5] = { 0, 0, 0, 0, 0 };

    	int laststate = HIGH;
    	int j = 0;

    	/* pull pin down for 18 milliseconds */
    	pinMode(dht_pin, OUTPUT);
    	digitalWrite(dht_pin, LOW);
    	delay(18);

    	/* prepare to read the pin */
    	pinMode(dht_pin, INPUT);

    	/* detect change and read data */
    	for (int i = 0; i < MAX_TIMINGS; i++) {
    		int counter = 0;
    		while ( digitalRead( dht_pin ) == laststate ) {
    			counter++;
    			delayMicroseconds( 1 );
    			if ( counter == 255 ) {
    				break;
    			}
    		}
    		laststate = digitalRead( dht_pin );

    		if ( counter == 255 )
    			break;

    		/* ignore first 3 transitions */
    		if ((i >= 4) && (i % 2 == 0)) {
    			/* shove each bit into the storage bytes */
    			data[j / 8] <<= 1;
    			if (counter > 16)
    				data[j / 8] |= 1;
    			j++;
    		}
    	}

    	/*
    	 * check we read 40 bits (8bit x 5) + verify checksum in the last byte
    	 * print it out if data is good
    	 */
    	if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
    		float h = (float)((data[0] << 8) + data[1]) / 10;
    		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;

    		if (data[2] & 0x80)
    			c = -c;

    		*temperature = c;
    		*humidity = h;
    		return 0;
    	}

        delay(WAIT_TIME);
    }

    return 1;
}
