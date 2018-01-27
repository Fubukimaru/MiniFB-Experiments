#include <MiniFB.h>

if (!mfb_open("my display", 800, 600))
	return 0;

for (;;)
{
	int state;

	// TODO: add some fancy rendering to the buffer of size 800 * 600

	state = mfb_update(buffer);

	if (state < 0)
		break;
}

mfb_close();
