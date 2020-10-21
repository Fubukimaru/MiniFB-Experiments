#include <MiniFB.h>

// Random
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <math.h>

// Sleep
#include <unistd.h>

static uint32_t g_width     = 600;
static uint32_t g_height    = 600;
static uint32_t g_hewi;
static uint32_t *g_buffer   = 0x0;


static uint32_t n_black         = 600*300;
static uint32_t sleep_millis    = 100;

// static unsigned int g_buffer[WIDTH * HEIGHT];
static uint32_t *neighbours = 0x0;

static uint32_t dead = MFB_RGB(0xFF,0xFF,0xFF);
static uint32_t alive = MFB_RGB(0,0,0);

static bool di = true;
        

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void resize(struct mfb_window *window, int width, int height) {
    static uint32_t* res;
    if (!di) { // If interruptions not disabled
        printf("Resizing %d x %d\n", width, height);
        (void) window;
        g_width  = width;
        g_height = height;
        g_hewi = width*height;
        res = realloc(g_buffer, g_hewi * 4);
        assert(res);
        g_buffer = res;
        res = realloc(neighbours, g_hewi);
        assert(res);
        neighbours = res;
        printf("g_hewi: %d\n", g_hewi);
        printf("Done resizing\n");
    }
}

void init_buffer() {
        int i = 0;
        unsigned int r = rand();

        // White screen
		for (i = 0; i < g_hewi; ++i)
		{
			g_buffer[i] = dead;
		}

        // Set random points
        srand(time(NULL));   // should only be called once
        for (i = 0; i < n_black; ++i) {
            r = rand()%(g_hewi);
            while (g_buffer[r] == alive) { // If already alive, rand again
                r = rand()%(g_hewi);
            }
            g_buffer[r] = alive;
        }
}

void count_living() {
        int i, l = 0;
		for (i = 0; i < g_hewi; ++i)
		{
			if (g_buffer[i] == alive) ++l;
		}
        printf("Living %d\n", l);
}

unsigned int numberOfNeighbours(unsigned int id) {
    // Check the surrounding points
    int x, y, h, w, count;
    h = round(id / g_width);
    w = id-(h*g_width);

    count = 0;
    for (x = w-1; x <= w+1; ++x) {
        for (y = h-1; y <= h+1; ++y) {
            //printf("%u, %u, \n", x, y);
            if ( x >= 0 && y >= 0 && x < g_width && y < g_height) { //In range
                //printf("%u, ", x*WIDTH+y);
                if (g_buffer[x*g_width+y] == alive) ++count; 
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
    uint32_t i, noise, carry, seed = 0xbeef;
    // Open screen
    //
    struct mfb_window *window = mfb_open_ex("Game of Life", g_width, g_height,
            WF_RESIZABLE);
	if (!window) return 0;

    g_hewi      = g_width*g_height;
    g_buffer    = (uint32_t *) calloc(g_hewi * 4, sizeof(uint32_t));
    neighbours  = (uint32_t *) calloc(g_hewi, sizeof(uint32_t));
    mfb_set_resize_callback(window, resize);

    mfb_set_viewport(window, 0, 0, g_width, g_height);
    //resize(window, g_width - 100, g_height - 100);  // to resize buffer


    init_buffer();

	do {
		int i, state;
        count_living();
		for (i = 0; i < g_hewi; ++i)
		{
			//g_buffer[i] = MFB_RGB(0, 0, 0xFF); 
            neighbours[i] = numberOfNeighbours(i);
        }

        for (i = 0; i < g_hewi; ++i)
		{
			//g_buffer[i] = MFB_RGB(0, 0, 0xFF); 
            nNeigh = neighbours[i];
               
            /*
             *
                Any live cell with fewer than two live neighbours dies, 
                    as if caused by underpopulation.
                Any live cell with two or three live neighbours lives on to
                    the next generation.
                Any live cell with more than three live neighbours dies, 
                    as if by overpopulation.
                Any dead cell with exactly three live neighbours becomes 
                    a live cell, as if by reproduction.
            */
            if(g_buffer[i] == alive) { //alive
                if (nNeigh > 3 || nNeigh < 2) g_buffer[i] = dead;
            } else {
                if (nNeigh == 3) g_buffer[i] = alive;
            }
		}

        // state = mfb_update(g_buffer);
        state = mfb_update_ex(window, g_buffer, g_width, g_height);

		if (state < 0)
			break;

        //di = false; // Enable interrupts
        usleep(sleep_millis*1000);
        di = true; // Disable interrupts
	} while(mfb_wait_sync(window));

    free(g_buffer);
    free(neighbours);

	return 0;
}

