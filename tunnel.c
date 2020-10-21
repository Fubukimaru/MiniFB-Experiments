#include <MiniFB.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static uint32_t  g_width  = 800;
static uint32_t  g_height = 800;
static uint32_t *g_buffer = 0x0;


private width: number;
private height: number;
private src: ImageData;

private distances: number[] = [];
private angles: number[] = [];
private shades: number[] = [];

void init() 
{
    uint32_t x, y, sqy, sqx, i = 0;
    //this.src = await demoFX.loadImage("space.jpg");
    for (y = 0; y < g_height * 2; ++y) {
        sqy = (y - g_height) * (y - g_height);
        for (x = 0; x < g_width * 2; ++x) {
            sqx = (x - g_width) * (x - g_width);
            i = x*g_width+y
 // TODO: !!!!!!!!!!!!!              Continuar aquí !!!!!!!!!!!!!!!!!!!!!!!!
            distances[i] = Math.floor(32 * this.src.height /
                                           Math.sqrt(sqx + sqy)) % this.src.height);
            this.angles.push(Math.round(this.src.width *
                                        Math.atan2(y - this.height,
                                                   x - this.width) / Math.PI));
            this.shades.push(Math.min(Math.sqrt(sqx + sqy), 255));
        }
    }
}

  public render(ctx: CanvasRenderingContext2D, t: number): void {
    const dest: ImageData = ctx.getImageData(0, 0, this.width, this.height);
    const time: number = t / 1000;
    const shiftx: number = Math.floor(this.src.width * time);
    const shifty: number = Math.floor(this.src.height * 0.25 * time);
    const centerx: number = this.width / 2 + Math.floor(this.width / 4 *
                                                        Math.sin(time / 4));
    const centery: number = this.height / 2 + Math.floor(this.height / 4 *
                                                         Math.sin(time / 2));
    const stride: number = this.width * 2;
    let destOfs: number = 0;
    for (let y: number = 0; y < this.height; y++) {
      let srcOfs: number = y * stride + centerx + centery * stride;
      for (let x: number = 0; x < this.width; x++) {
        const u: number = (this.distances[srcOfs] + shiftx) & 0xff;
        let v: number = (this.angles[srcOfs] + shifty) % this.src.height;
        while (v < 0) {
          v += this.src.height;
        }
        const shade: number = this.shades[srcOfs] / 255;
        srcOfs++;
        let pixOfs: number = (u + (v << 8)) << 2;
        dest.data[destOfs++] = this.src.data[pixOfs++] * shade;  // r
        dest.data[destOfs++] = this.src.data[pixOfs++] * shade;  // g
        dest.data[destOfs++] = this.src.data[pixOfs++] * shade;  // b
        dest.data[destOfs++] = this.src.data[pixOfs++];  // a
      }
    }
    ctx.putImageData(dest, 0, 0);
  }
}

int main()
{
    uint32_t    i, noise, carry, seed = 0xbeef;

    struct mfb_window *window = mfb_open_ex("Noise Test", g_width, g_height, WF_RESIZABLE);
    if (!window)
        return 0;

    g_buffer = (uint32_t *) malloc(g_width * g_height * 4);

    mfb_set_viewport(window, 0, 0, g_width, g_height);

    mfb_update_state state;
    do {
        for (i = 0; i < g_width * g_height; ++i) {
            noise = seed;
            noise >>= 3;
            noise ^= seed;
            carry = noise & 1;
            noise >>= 1;
            seed >>= 1;
            seed |= (carry << 30);
            noise &= 0xFF;
            g_buffer[i] = MFB_RGB(noise, noise, noise); 
        }

        state = mfb_update_ex(window, g_buffer, g_width, g_height);
        if (state != STATE_OK) {
            window = 0x0;
            break;
        }
    } while(mfb_wait_sync(window));

    return 0;
}
