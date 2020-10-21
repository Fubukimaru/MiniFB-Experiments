WIN_GCC=i686-w64-mingw32-gcc 
GCC=gcc

LIBS=-lminifb -lGL -lGLU -lglut -lX11

FLAGS=-g

all: noise gol

noise: noise.c
	$(GCC) $(FLAGS) noise.c $(LIBS) -o noise
gol: gol.c
	$(GCC) $(FLAGS) gol.c $(LIBS) -o gol
noise.exe: noise.c
	$(WIN_GCC) $(FLAGS) noise.c $(LIBS) -o noise
gol.exe: gol.c
	$(WIN_GCC) $(FLAGS) gol.c $(LIBS) -o gol
clean:
	rm noise gol
