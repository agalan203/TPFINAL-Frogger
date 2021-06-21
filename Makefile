CC := gcc
OPTIONS = -g -Wall

frontrpi: front_raspi.o joydrv.o termlib.o disdrv.o
	${CC} ${OPTIONS} front_raspi.o joydrv.o termlib.o disdrv.o -o frontrpi

front_raspi.o: front_raspi.c globalstuff.h disdrv.h joydrv.h termlib.h
	${CC} ${OPTIONS} -c  front_raspi.c

clean: 
	rm *.o