TARGET = main

SRCS = src/main.c \
src/dc_motor/dc_motor.c \
src/rc_servo/rc_servo.c \
src/distance_sensor/distance_sensor.c \
src/distance_sensor/queue/queue.c \
src/motor_control/motor_control.c \
src/mylib/mylib.c

OBJS = ${SRCS:.c=.o}

CFLAGS = -Wall -W 
LDFLAGS = -lpthread -lm -lrt -ldl

all: LDFLAGS += -lpigpiod_if2 -O2 
all: ${TARGET}

debug: CFLAGS += -DDEBUG -O0 -ggdb3 -g
debug: ${TARGET}

${TARGET}: ${OBJS} 
	make lib
	${CC} ${OBJS} -o ${TARGET} ${LDFLAGS}

%.o: %.c
	${CC} -c $< -o $@ ${CFLAGS}

lib: src/mylib/mylib.so
	${CC} src/mylib/mylib.c -fPIC -W -Wall -DDEBUG -shared -o src/mylib/mylib.so

clean:
	rm -f ${OBJS} ${TARGET}

