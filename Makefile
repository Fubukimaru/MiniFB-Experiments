all: noise gol

noise: 
	gcc noise.c -lminifb -lX11 -o noise
gol:
	gcc gol.c -lminifb -lX11 -o gol
clean:
	rm noise gol
