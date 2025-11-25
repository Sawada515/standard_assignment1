/**
 *  @file 	mylib.c
 *  @author 	sawada souta
 *  @date 	2025/11/25
 *  @brief	share memory open, read, write
 *  @note bug fix
 */

#include "../defines.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

void*
c_py_shm_open(void)
{
	void *addr = 0;
	int shm_fd;

	//shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
	shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
	if (shm_fd == -1) {
		perror("shm_open");
		return 0;
	}

//	if (ftruncate(shm_fd, SHM_SIZE) == -1) {
//		perror("ftruncate");
//		close(shm_fd);
//		return 0;
//	}

	addr = (void*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		close(shm_fd);
		return 0;
	}

	close(shm_fd);

	memset(addr, 0, SHM_SIZE);

	return addr;
}

void 
shm_write(void *shm_addr,
	  uint8_t r_duty,
	  uint8_t l_duty,
	  uint8_t direction,
	  uint8_t angle,
	  double distance_sensor1,
	  double distance_sensor2,
	  double distance_sensor3,
	  double distance_sensor4)
{
	data_packet_t tmp = { 0 };

	tmp.r_duty = r_duty;
	tmp.l_duty = l_duty;
	tmp.direction = direction;
	tmp.angle = angle;

	tmp.distance_sensor1 = distance_sensor1;
	tmp.distance_sensor2 = distance_sensor2;
	tmp.distance_sensor3 = distance_sensor3;
	tmp.distance_sensor4 = distance_sensor4;

	memcpy(shm_addr, &tmp, sizeof(tmp));

#ifdef DEBUG
	printf("c %hhu\n", tmp.r_duty);
	printf("c %hhu\n", tmp.l_duty);
	printf("c %hhu\n", tmp.direction);
	printf("c %hhu\n", tmp.angle);

	printf("c %lf\n", tmp.distance_sensor1);
	printf("c %lf\n", tmp.distance_sensor2);
	printf("c %lf\n", tmp.distance_sensor3);
	printf("c %lf\n", tmp.distance_sensor4);
#endif
}

void 
shm_write_motor_data(void *shm_addr,
	  uint8_t r_duty,
	  uint8_t l_duty,
	  uint8_t direction,
	  uint8_t angle)
{
	data_packet_t tmp = { 0 };

	memcpy(&tmp, shm_addr, sizeof(tmp));

	tmp.r_duty = r_duty;
	tmp.l_duty = l_duty;
	tmp.direction = direction;
	tmp.angle = angle;

	memcpy(shm_addr, &tmp, sizeof(tmp));
}

void 
shm_read(void *shm_addr,
	 uint8_t *r_duty,
	 uint8_t *l_duty,
	 uint8_t *direction,
	 uint8_t *angle,
	 double *distance_sensor1,
	 double *distance_sensor2,
	 double *distance_sensor3,
	 double *distance_sensor4)
{
	data_packet_t tmp;

	memcpy(&tmp, shm_addr, sizeof(tmp));

	if(r_duty != NULL) {
		*r_duty = tmp.r_duty;
	}

	if(l_duty != NULL) {
		*l_duty = tmp.l_duty;
	}

	if(direction != NULL) {
		*direction = tmp.direction;
	}

	if(angle != NULL) {
		*angle = tmp.angle;
	}
	
	if(distance_sensor1 != NULL) {
		*distance_sensor1 = tmp.distance_sensor1;	
	}

	if(distance_sensor2 != NULL) {
		*distance_sensor2 = tmp.distance_sensor2;	
	}

	if(distance_sensor3 != NULL) {
		*distance_sensor3 = tmp.distance_sensor3;	
	}

	if(distance_sensor4 != NULL) {
		*distance_sensor4 = tmp.distance_sensor4;	
	}
}

