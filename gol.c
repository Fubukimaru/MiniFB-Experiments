#include <MiniFB.h>

// Random
#include <time.h>
#include <stdlib.h>

#include <math.h>

// Sleep
#include <unistd.h>

#define WIDTH 400
#define HEIGHT 400
#define N_BLACK 400*100

#define SLEEP_MILLIS 100

static unsigned int s_buffer[WIDTH * HEIGHT];

static unsigned int dead = MFB_RGB(0xFF,0xFF,0xFF);
static unsigned int alive = MFB_RGB(0,0,0);
        
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initScreen() {
       int i = 0;
        unsigned int r = rand();

        // White screen
		for (i = 0; i < WIDTH * HEIGHT; ++i)
		{
			s_buffer[i] = dead;
		}

        // Set random points
        srand(time(NULL));   // should only be called once
        for (i = 0; i < N_BLACK; ++i) {
            r = rand()%(WIDTH*HEIGHT);
            while (s_buffer[r] == alive) { // If already alive, rand again
                r = rand()%(WIDTH*HEIGHT);
            }
            s_buffer[r] = alive;
        }
}

unsigned int numberOfNeighbours(unsigned int id) {
    // Check the surrounding points
    int x, y, h, w, count;
    h = round(id / WIDTH);
    w = id-(h*WIDTH);

    count = 0;
    for (x = w-1; x <= w+1; ++x) {
        for (y = h-1; y <= h+1; ++y) {
            //printf("%u, %u, \n", x, y);
            if ( x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT) { //In range
                //printf("%u, ", x*WIDTH+y);
                if (s_buffer[x*WIDTH+y] == alive) ++count; 
            }
        }
        //printf("\n");    
    }

    //printf("Number of neighbours for id %u\n", id);
    //printf("Count: %u\n", count);
    return(count);
}

int main()
{
	unsigned int nNeigh;
    int noise, carry, seed = 0xbeef;
    // Open screen
	if (!mfb_open("Game of Life", WIDTH, HEIGHT))
		return 0;

    initScreen();

	for (;;)
	{
		int i, state;
        printf("Next round\n");
		for (i = 0; i < WIDTH * HEIGHT; ++i)
		{
			//s_buffer[i] = MFB_RGB(0, 0, 0xFF); 
            nNeigh = numberOfNeighbours(i);
            
            /*
             *
                Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
                Any live cell with two or three live neighbours lives on to the next generation.
                Any live cell with more than three live neighbours dies, as if by overpopulation.
                Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            */
            if(s_buffer[i] == alive) { //alive
                if (nNeigh > 3 || nNeigh < 2) s_buffer[i] = dead;
            } else {
                if (nNeigh == 3) s_buffer[i] = alive;
            }
		}

		state = mfb_update(s_buffer);

		if (state < 0)
			break;

        usleep(SLEEP_MILLIS*1000);
	}

	mfb_close();

	return 0;
}

