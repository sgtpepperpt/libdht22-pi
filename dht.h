#ifndef __DHT_H__
#define __DHT_H__

#ifdef __cplusplus
extern "C"{
#endif

int read_dht_data(int dht_pin, float* temperature, float* humidity);

#ifdef __cplusplus
}
#endif

#endif
