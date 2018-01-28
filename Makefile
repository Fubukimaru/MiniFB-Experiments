all: noise gol

noise: noise.c
	gcc noise.c -lminifb -lX11 -o noise
gol: gol.c
	gcc gol.c -lminifb -lX11 -o gol
clean:
	rm noise gol
