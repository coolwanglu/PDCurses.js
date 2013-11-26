/* Public Domain Curses */

#include "pdcsdl.h"

RCSID("$Id: pdcutil.c,v 1.6 2008/07/14 04:24:52 wmcbrine Exp $")

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
}

// for emscripten
void PDC_napms(int ms)
{
    PDC_LOG(("PDC_napms() - called: ms=%d\n", ms));

    PDC_update_rects();
    SDL_Delay(ms);
}

void PDC_napms_async(int ms, void (*callback)(void *))
{
    PDC_LOG(("PDC_napms_async() - called: ms=%d\n", ms));

    PDC_update_rects();
    emscripten_async_call(callback, NULL, ms);
}

const char *PDC_sysname(void)
{
    return "SDL";
}
