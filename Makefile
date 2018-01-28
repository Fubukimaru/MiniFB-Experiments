WIN_GCC=i686-w64-mingw32-gcc 
GCC=gcc

all: noise gol

noise: noise.c
	$(GCC) noise.c -lminifb -lX11 -o noise
gol: gol.c
	$(GCC) gol.c -lminifb -lX11 -o gol
noise.exe: noise.c
	$(WIN_GCC) noise.c -lminifb -lX11 -o noise
gol.exe: gol.c
	$(WIN_GCC) gol.c -lminifb -lX11 -o gol
clean:
	rm noise gol
