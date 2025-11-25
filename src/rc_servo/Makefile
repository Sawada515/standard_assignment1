TARGET = rc_servo

all:
	gcc ${TARGET}.c -W -Wall -lpigpiod_if2 -lm -lrt -lpthread -O2 -o ${TARGET}

debug:
	gcc ${TARGET}.c -W -Wall -lm -lrt -lpthread -O2 -fsyntax-only -DDEBUG

