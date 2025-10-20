all:
	gcc -c c8vm.c
	gcc -c display.c `sdl2-config --cflags`
	gcc -c main.c `sdl2-config --cflags`
	gcc -o chip8 c8vm.o display.o main.o `sdl2-config --libs`

clean:
	rm -f *.o chip8
