all:
	mkdir -p .build
	gcc -c c8vm.c -o .build/c8vm.o
	gcc -c display.c `sdl2-config --cflags` -o .build/display.o
	gcc -c sound.c `sdl2-config --cflags` -o .build/sound.o
	gcc -c util.c -o .build/util.o
	gcc -c main.c `sdl2-config --cflags` -o .build/main.o
	gcc -o chip8 .build/c8vm.o .build/display.o .build/sound.o .build/util.o .build/main.o `sdl2-config --libs` -lm

clean:
	rm -rf .build chip8
