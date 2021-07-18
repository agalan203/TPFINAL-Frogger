CC := gcc
OPTIONS = -g -Wall

#Para compilar para plataforma RPI: make froggerraspi
#Para compilar para plataforma 64-bit grafica: make froggerallegro

froggerraspi: mainraspi.o mainfun.o frontraspi.o backend.o joydrv.o termlib.o disdrv.o mapa.o rana.o
	${CC} ${OPTIONS} mainraspi.o mainfun.o frontraspi.o backend.o joydrv.o termlib.o disdrv.o mapa.o rana.o -o froggerraspi

mainfun.o: mainfun.c globalstuff.h
	${CC} ${OPTIONS} -c mainfun.c 

frontraspi.o: frontraspi.c globalstuff.h disdrv.h joydrv.h termlib.h
	${CC} ${OPTIONS} -c  frontraspi.c

backend.o: backend.c rana.h mapa.h
	${CC} ${OPTIONS} -c  backend.c

rana.o: rana.c rana.h
	${CC} ${OPTIONS} -c  rana.c

mapa.o: mapa.c mapa.h
	${CC} ${OPTIONS} -c  mapa.c

mainraspi.o: mainraspi.c backend.h frontraspi.h mainfun.h
	${CC} ${OPTIONS} -c  mainraspi.c

froggerallegro: mainallegro.o mainfun.o frontall.o backend.o mapa.o rana.o 
	${CC} ${OPTIONS} mainallegro.o mainfun.o frontall.o backend.o mapa.o rana.o -o froggerallegro `pkg-config --cflags --libs allegro-5` `pkg-config --cflags --libs allegro_acodec-5` `pkg-config --cflags --libs allegro_audio-5` `pkg-config --cflags --libs allegro_color-5` `pkg-config --cflags --libs allegro_font-5` `pkg-config --cflags --libs allegro_image-5` `pkg-config --cflags --libs allegro_main-5` `pkg-config --cflags --libs allegro_memfile-5` `pkg-config --cflags --libs allegro_physfs-5` `pkg-config --cflags --libs allegro_primitives-5` `pkg-config --cflags --libs allegro_ttf-5` `pkg-config --cflags --libs allegro_video-5`

frontall.o: frontall.c globalstuff.h
	${CC} ${OPTIONS} -c  frontall.c

mainallegro.o: mainallegro.c backend.h frontall.h mainfun.h
	${CC} ${OPTIONS} -c  mainallegro.c

cleanraspi:
	rm mainraspi.o mainfun.o frontraspi.o backend.o mapa.o rana.o

cleanallegro:
	rm mainfun.o frontall.o backend.o mapa.o rana.o mainallegro.o
