/**
 *  @file 	distance_sensor.c
 *  @author 	sawada souta
 *  @date 	2025/11/12 
 *  @brief	ads1015用
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#ifndef DEBUG
#include <pigpiod_if2.h>
#endif

#include "./queue/queue.h"
#include "distance_sensor.h"

#define QUEUE_LENGTH	32

#define DEV_ID		0x48

//CH[0-3] - GND
enum AdcChannel {
	CH0 = 0x04,
	CH1 = 0x05,
	CH2 = 0x06,
	CH3 = 0x07
};

enum AdcDataRate {
	SPS128  = 0x00,
	SPS250  = 0x01,
	SPS490  = 0x02,
	SPS920  = 0x03,
	SPS1600 = 0x04,
	SPS2400 = 0x05,
	SPS3300 = 0x06
};

//lower 2bits
#define	CONVERT_REG		0x00
#define CONFIG_REG		0x01
#define LO_THRE_REG		0x02
#define HI_THRE_REG		0x03	

#define calc_distance(this, ch) \
(double)((26.549 * pow(read_adc((this), (ch)) * 0.002, -1.2091F)))

static void set_read_channel(distance_sensor_t *this, enum AdcChannel ch);
static short read_adc(distance_sensor_t *this, enum AdcChannel ch);
static unsigned char adc_is_busy(distance_sensor_t *this);

void start_distance_sensor_thread(distance_sensor_t *this);
void *get_distance_sensor_thread(void *arg);

void distance_sensor_construct(distance_sensor_t *this, int pigpio_daemon) {
	this->pigpio_daemon = pigpio_daemon;

	init_queue(&(this->distance_sensor.ch0), QUEUE_LENGTH);
	init_queue(&(this->distance_sensor.ch1), QUEUE_LENGTH);
	init_queue(&(this->distance_sensor.ch2), QUEUE_LENGTH);
	init_queue(&(this->distance_sensor.ch3), QUEUE_LENGTH);

#ifndef DEBUG
	RE_OPEN_I2C_HANDLE:
	this->i2c_handler = i2c_open(this->pigpio_daemon, 1, DEV_ID, 0);
	if(this->i2c_handler < 0) {
		perror("i2c init");

		goto RE_OPEN_I2C_HANDLE;
	}
#else
	this->i2c_handler = 0;
#endif

	static bool is_created_thread = false;

	if(!is_created_thread) {
		is_created_thread = true;

		start_distance_sensor_thread(this);
	}
}

void distance_sensor_deconstruct(distance_sensor_t *this)
{
	delete_queue(&(this->distance_sensor.ch0));
	delete_queue(&(this->distance_sensor.ch1));
	delete_queue(&(this->distance_sensor.ch2));
	delete_queue(&(this->distance_sensor.ch3));
}

double distance_sensor_get_value(distance_sensor_t *this, int ch)
{
	double ret;

	switch(ch) {
	case 0:
		ret = get_average(&(this->distance_sensor.ch0));
		break;
	case 1:
		ret = get_average(&(this->distance_sensor.ch1));
		break;
	case 2:
		ret = get_average(&(this->distance_sensor.ch2));
		break;
	case 3:
		ret = get_average(&(this->distance_sensor.ch3));
		break;
	default:
		ret = 0.0;

		break;
	}

	return ret;
}

void *get_distance_sensor_thread(void *arg)
{
	distance_sensor_t *this = (distance_sensor_t *)arg;

	enum AdcChannel ch[4] = { CH0, CH1, CH2, CH3 };
	
	double ret;

	while(1) {
		for(signed char i = 0; i < 4; ++i) {
			ret = calc_distance(this, ch[i]);

			switch(i) {
			case 0:
				enqueue(&(this->distance_sensor.ch0), ret);
				break;
			case 1:
				enqueue(&(this->distance_sensor.ch1), ret);
				break;
			case 2:
				enqueue(&(this->distance_sensor.ch2), ret);
				break;
			case 3:
				enqueue(&(this->distance_sensor.ch3), ret);
				break;
			default:
				break;
			}
		}
	}
}

void start_distance_sensor_thread(distance_sensor_t *this)
{
	pthread_t distance_sensor_thread_t;
	int ret;

	RETRY_CREATE_THREAD:

	ret = pthread_create(&distance_sensor_thread_t, NULL, get_distance_sensor_thread, this);

	if(ret) {
		perror("distance_sensor thread_create_error");

		goto RETRY_CREATE_THREAD;
	}

	pthread_detach(distance_sensor_thread_t);
}

static void set_read_channel(distance_sensor_t *this, enum AdcChannel ch)
{
	unsigned short config = 0x8303;

	config |= (ch << 12);		//set channel
	config |= (SPS128 << 5);	//set data rate

	config = ((config >> 8) & 0x00FF) | ((config << 8) & 0xFF00);

//	printf("config %04X\n", config);

#ifndef DEBUG
	i2c_write_word_data(this->pigpio_daemon, this->i2c_handler, CONFIG_REG, config);
#endif
}

static short read_adc(distance_sensor_t *this, enum AdcChannel ch)
{
	short ret = 0x0000;
	short result = 0x0000;

	set_read_channel(this, ch);

	while(adc_is_busy(this));

#ifndef DEBUG
	ret = i2c_read_word_data(this->pigpio_daemon, this->i2c_handler, CONVERT_REG);
#else
	return rand() % 0x03FF;
#endif
	ret = ((ret >> 8) & 0x00FF) | ((ret << 8) & 0xFF00);

	//MSB - 8 -> convert_data[11:4]
	//7 - 4   -> convert_data[3:0]
	//result = ~((((ret & 0xFF00) >> 4) | ((ret & 0x00F0) >> 4)) - 1); 
	result = ret >> 4;

	return result;
}

static unsigned char adc_is_busy(distance_sensor_t *this)
{
#ifndef DEBUG
	short ret = i2c_read_word_data(this->pigpio_daemon, this->i2c_handler, CONFIG_REG);
#else
	short ret = 0x0001;
#endif
	
	if(ret >= 0x0001) {
		return 0;		//adc complete
	}
	else {
		usleep(1000 * 1);

		return 1;		//adc not complete
	}
}

