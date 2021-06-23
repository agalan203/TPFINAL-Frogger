CC := gcc
OPTIONS = -g -Wall

frontrpi: front_raspi.o joydrv.o termlib.o disdrv.o
	${CC} ${OPTIONS} front_raspi.o joydrv.o termlib.o disdrv.o -o frontrpi

front_raspi.o: front_raspi.c globalstuff.h disdrv.h joydrv.h termlib.h
	${CC} ${OPTIONS} -c  front_raspi.c

frontall: front_all.o 
	${CC} ${OPTIONS} -o frontall `pkg-config --cflags --libs allegro-5` `pkg-config --cflags --libs allegro_acodec-5` `pkg-config --cflags --libs allegro_audio-5` `pkg-config --cflags --libs allegro_color-5` `pkg-config --cflags --libs allegro_font-5` `pkg-config --cflags --libs allegro_image-5` `pkg-config --cflags --libs allegro_main-5` `pkg-config --cflags --libs allegro_memfile-5` `pkg-config --cflags --libs allegro_physfs-5` `pkg-config --cflags --libs allegro_primitives-5` `pkg-config --cflags --libs allegro_ttf-5` `pkg-config --cflags --libs allegro_video-5`

front_all.o: front_all.c
	${CC} ${OPTIONS} -c  front_all.c