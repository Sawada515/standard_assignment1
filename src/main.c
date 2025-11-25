/**
 *  @file 	main.c
 *  @author 	sawada souta
 *  @date 	2025/11/24
 *  @brief	gpio操作 main関数
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "rc_servo/rc_servo.h"
#include "dc_motor/dc_motor.h"
#include "distance_sensor/distance_sensor.h"
#include "distance_sensor/queue/queue.h"
#include "motor_control/motor_control.h"
#include "defines.h"

#ifndef DEBUG
#include <pigpiod_if2.h>
#endif

void signal_handler(int signum __attribute__((unused)));

volatile unsigned char IsRun = 1;

int main(void)
{
	int pigpio_daemon;
#ifndef DEBUG
RE_CONNECT_PIGPIO_DAEMON:
	pigpio_daemon = pigpio_start(NULL, NULL);
	if(pigpio_daemon < 0) {
		perror("pigpio");

		goto RE_CONNECT_PIGPIO_DAEMON;
	}
#else
	pigpio_daemon = 0;
#endif

	int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if(shm_fd == -1) {
		perror("shm_open");

		return -1;
	}

	if(ftruncate(shm_fd, SHM_SIZE) == -1) {
		perror("ftruncate");

		close(shm_fd);

		return -1;
	}

	data_packet_t *shm_data = (data_packet_t *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if(shm_data == MAP_FAILED) {
		perror("mmap");

		close(shm_fd);

		return -1;
	}

	close(shm_fd);

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	distance_sensor_t 	sensor;
	rc_servo_t		servo;
	dc_motor_t		right_motor;
	dc_motor_t		left_motor;
	
	memset(&sensor, 0, sizeof(sensor));
	memset(&servo, 0, sizeof(servo));
	memset(&right_motor, 0, sizeof(right_motor));
	memset(&left_motor, 0, sizeof(left_motor));

	distance_sensor_construct(&sensor, pigpio_daemon);
	rc_servo_construct(&servo, 90, pigpio_daemon);

	dc_motor_construct(&right_motor, \
			pigpio_daemon, \
			set_pwm(22000), \
			set_duty(35), \
			M1_IN1, \
			M1_IN2, \
			M1_PWM);

	dc_motor_construct(&left_motor, \
			pigpio_daemon, \
			set_pwm(22000), \
			set_duty(35), \
			M2_IN1, \
			M2_IN2, \
			M2_PWM);

	move_brake(&right_motor, &left_motor);

	unsigned char r_duty = 0;
	unsigned char l_duty = 0;

	direction_t direction = 0;

	unsigned char angle = 0;

	double distance_sensor1 = 0;
	double distance_sensor2 = 0;
	double distance_sensor3 = 0;
	double distance_sensor4 = 0;

	while(IsRun) {
		distance_sensor1 =  distance_sensor_get_value(&sensor, 0);
		distance_sensor2 =  distance_sensor_get_value(&sensor, 1);
		distance_sensor3 =  distance_sensor_get_value(&sensor, 2);
		distance_sensor4 =  distance_sensor_get_value(&sensor, 3);

#ifdef DEBUG
		printf("dis %lf\n", distance_sensor1);
		printf("dis %lf\n", distance_sensor2);
		printf("dis %lf\n", distance_sensor3);
		printf("dis %lf\n", distance_sensor4);
#endif

		shm_write_sensor_value(shm_data, distance_sensor1, distance_sensor2, distance_sensor3, distance_sensor4);

		shm_read(shm_data, &r_duty, &l_duty, (unsigned char*)&direction, &angle, NULL, NULL, NULL, NULL);

		rc_servo_set_angle(&servo, angle);

		switch(direction) {
		case STOP:
			move_brake(&right_motor, &left_motor);
			
			break;
		case FORWARD:
			move_forward(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case BACK:
			move_back(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case RIGHT_TURN:
			move_right_turn(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case LEFT_TURN:
			move_left_turn(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case RIGHT_FRONT:
			move_right_front(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case LEFT_FRONT:
			move_left_front(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case RIGHT_REAR:
			move_right_rear(&right_motor, &left_motor, r_duty, l_duty);

			break;
		case LEFT_REAR:
			move_left_rear(&right_motor, &left_motor, r_duty, l_duty);

			break;
		default:
			move_brake(&right_motor, &left_motor);
			
			break;
		}
	}

	distance_sensor_deconstruct(&sensor);

#ifndef DEBUG
   	pigpio_stop(pigpio_daemon);
#endif
	munmap(shm_data, SHM_SIZE);

	shm_unlink(SHM_NAME);

	return 0;
}

void signal_handler(int signum __attribute__((unused)))
{
	IsRun = 0;

#ifdef DEBUG
	printf("catch signal\n");	//unsafe

	fflush(stdout);
#endif
}
