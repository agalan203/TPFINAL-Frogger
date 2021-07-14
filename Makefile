CC := gcc
OPTIONS = -g -Wall

froggerraspi: mainraspi.o front_raspi.o backend.o joydrv.o termlib.o disdrv.o mapa.o rana.o
	${CC} ${OPTIONS} mainraspi.o front_raspi.o backend.o joydrv.o termlib.o disdrv.o mapa.o rana.o -o froggerraspi

front_raspi.o: front_raspi.c globalstuff.h disdrv.h joydrv.h termlib.h
	${CC} ${OPTIONS} -c  front_raspi.c

backend.o: backend.c rana.h mapa.h
	${CC} ${OPTIONS} -c  backend.c

rana.o: rana.c rana.h
	${CC} ${OPTIONS} -c  rana.c

mapa.o: mapa.c mapa.h
	${CC} ${OPTIONS} -c  mapa.c

mainraspi.o: mainraspi.c backend.h frontraspi.h globalstuff.h
	${CC} ${OPTIONS} -c  mainraspi.c

froggerallegro: mainallegro.o frontall.o backend.o mapa.o rana.o
	${CC} ${OPTIONS} mainallegro.o frontall.o backend.o mapa.o rana.o -o froggerallegro `pkg-config --cflags --libs allegro-5` `pkg-config --cflags --libs allegro_acodec-5` `pkg-config --cflags --libs allegro_audio-5` `pkg-config --cflags --libs allegro_color-5` `pkg-config --cflags --libs allegro_font-5` `pkg-config --cflags --libs allegro_image-5` `pkg-config --cflags --libs allegro_main-5` `pkg-config --cflags --libs allegro_memfile-5` `pkg-config --cflags --libs allegro_physfs-5` `pkg-config --cflags --libs allegro_primitives-5` `pkg-config --cflags --libs allegro_ttf-5` `pkg-config --cflags --libs allegro_video-5`

frontall.o: frontall.c globalstuff.h
	${CC} ${OPTIONS} -c  frontall.c

mainallegro.o: mainallegro.c backend.h frontall.h globalstuff.h
	${CC} ${OPTIONS} -c  mainallegro.c

clean:
	rm mapa.o rana.o backend.o frontall.o mainallegro.o mainraspi.o front_raspi.o