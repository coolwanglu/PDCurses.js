/* Public Domain Curses */

#include <curspriv.h>

RCSID("$Id: beep.c,v 1.34 2008/07/13 16:08:17 wmcbrine Exp $")

/*man-start**************************************************************

  Name:                                                         beep

  Synopsis:
        int beep(void);
        int flash(void);

  Description:
        beep() sounds the audible bell on the terminal, if possible;
        if not, it calls flash().

        flash() "flashes" the screen, by inverting the foreground and
        background of every cell, pausing, and then restoring the
        original attributes.

  Return Value:
        These functions return OK.

  Portability                                X/Open    BSD    SYS V
        beep                                    Y       Y       Y
        flash                                   Y       Y       Y

**man-end****************************************************************/

// for emscripten
/*
int beep(void)
{
    PDC_LOG(("beep() - called\n"));

    if (SP->audible)
        PDC_beep();
    else
        flash();

    return OK;
}
*/
int beep(void) {
    return OK;
}
int beep_async(void (*callback)(void*))
{
    PDC_LOG(("beep_async() - called\n"));

    if (SP->audible)
        PDC_beep();
    else
        flash_async(callback);

    return OK;
}

//for emscripten
/*
int flash(void)
{
    int z, y, x;

    PDC_LOG(("flash() - called\n"));

    / * Reverse each cell; wait; restore the screen * /

    for (z = 0; z < 2; z++)
    {
        for (y = 0; y < LINES; y++)
            for (x = 0; x < COLS; x++)
                curscr->_y[y][x] ^= A_REVERSE;

        wrefresh(curscr);

        if (!z)
            napms(50);
    }

    return OK;
}
*/
int flash(void) {
    return OK;
}
static int flash_async__z;
static void (*flash_async__callback)(void*);
static void flash_async_(void * arg)
{
    int y, x;
    /* Reverse each cell; wait; restore the screen */
    while(flash_async__z < 2)
    {
        for (y = 0; y < LINES; y++)
            for (x = 0; x < COLS; x++)
                curscr->_y[y][x] ^= A_REVERSE;

        wrefresh(curscr);

        if (!flash_async__z) {
            ++ flash_async__z;
            napms_async(50, flash_async_);
            return;
        }
        ++flash_async__z;
    }

    (*flash_async__callback)((void*)OK);
}

int flash_async(void (*callback)(void*))
{

    PDC_LOG(("flash_async() - called\n"));
    flash_async__z = 0;
    flash_async__callback = callback;
    flash_async_(NULL);
    return OK;
}
