/**
 *  @file 	dc_motor.c
 *  @author 	sawada souta
 *  @date 	2025/11/12 
 *  @brief	DCモータコントロール関数
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#ifndef DEBUG
#include <pigpiod_if2.h>
#endif

#include "dc_motor.h"

void dc_motor_construct
(dc_motor_t *this,
int pigpio_daemon,
unsigned int pwm_frequency,
unsigned char duty,
unsigned char in1_pin,
unsigned char in2_pin,
unsigned char pwm_pin)
{
	this->pigpio_daemon = pigpio_daemon;
	this->pwm_frequency = pwm_frequency;
	this->duty = duty;

	this->in1_pin = in1_pin;
	this->in2_pin = in2_pin;

	this->pwm_pin = pwm_pin;

#ifndef DEBUG
	set_mode(this->pigpio_daemon, this->in1_pin, PI_OUTPUT);
	set_mode(this->pigpio_daemon, this->in2_pin, PI_OUTPUT);

	hardware_PWM(this->pigpio_daemon, this->pwm_pin, this->pwm_frequency, (unsigned long)this->duty * 10000);
#else
	printf("in1 %hhu\n", this->in1_pin);
	printf("in2 %hhu\n", this->in2_pin);

	printf("pwm : %hhu %u %hhu\n", this->pwm_pin, this->pwm_frequency, this->duty);
#endif
}

void dc_motor_set_frequency(dc_motor_t *this, unsigned int freq)
{
	this->pwm_frequency = freq;

#ifndef DEBUG
	hardware_PWM(this->pigpio_daemon, this->pwm_pin, this->pwm_frequency, (unsigned long)this->duty * 10000);
#else
	printf("pwm : %hhu %u %hhu\n", this->pwm_pin, this->pwm_frequency, this->duty);
#endif	

}

unsigned int dc_motor_get_frequency(dc_motor_t *this)
{
	return this->pwm_frequency;
}

void dc_motor_set_duty(dc_motor_t *this, unsigned char duty)
{
	this->duty = duty;

#ifndef DEBUG
	hardware_PWM(this->pigpio_daemon, this->pwm_pin, this->pwm_frequency, (unsigned long)this->duty * 10000);
#else
	printf("pwm : %hhu %u %hhu\n", this->pwm_pin, this->pwm_frequency, this->duty);
#endif	
}

unsigned char dc_motor_get_duty(dc_motor_t *this)
{
	return this->duty;
}

void dc_motor_set_in1(dc_motor_t *this, unsigned char var)
{
#ifndef DEBUG
	gpio_write(this->pigpio_daemon, this->in1_pin, var);
#else
	printf("in1 %hhu\n", var);
#endif
}

void dc_motor_set_in2(dc_motor_t *this, unsigned char var)
{
#ifndef DEBUG
	gpio_write(this->pigpio_daemon, this->in2_pin, var);
#else
	printf("in2 %hhu\n", var);
#endif
}

