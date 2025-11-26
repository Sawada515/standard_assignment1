#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct data_packet_t {
	uint8_t r_duty;
	uint8_t l_duty;
	uint8_t direction;

	uint8_t angle;

	double distance_sensor1;
	double distance_sensor2;
	double distance_sensor3;
	double distance_sensor4;
} __attribute__((packed)) data_packet_t;

int main(void)
{
	data_packet_t *data;

	int shm_fd = shm_open("/gpio_control_shm", O_RDWR, 0666);
	if(shm_fd == -1) {
		perror("shm open");

		return -1;
	}

	data = mmap(NULL, 48, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(data == MAP_FAILED) {
		perror("mmap");

		close(shm_fd);

		return -1;
	}

	close(shm_fd);

	printf("%hhu %hhu %hhu %hhu\n", data->r_duty, data->l_duty, data->direction, data->angle);

	munmap(data, 48);

	return 0;
}

