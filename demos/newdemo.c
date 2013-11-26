/*
 *  newdemo.c   -   A demo program using PDCurses. The program 
 *          illustrates the use of colors for text output.
 *
 *  Hacks by jbuhler@cs.washington.edu on 12/29/96
 *
 *  $Id: newdemo.c,v 1.39 2008/07/13 16:08:17 wmcbrine Exp $
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

void WaitForUser(void (*)(void*));
int SubWinTest(WINDOW *, void(*)(void*));
int BouncingBalls(WINDOW *, void(*)(void*));
void BouncingBalls_1(void *arg);
void BouncingBalls_2(void *arg);
void trap(int);

void async_1(void * arg);
void async_2(void * arg);
void async_3(void * arg);
void async_4(void * arg);
void async_5(void * arg);
void async_6(void * arg);
void async_7(void * arg);
void async_8(void * arg);
void async_9(void * arg);
void async_10(void * arg);
void async_11(void * arg);
void async_12(void * arg);
void async_13(void * arg);
void async_14(void * arg);
void async_15(void * arg);
void async_16(void * arg);

/* An ASCII map of Australia */

char *AusMap[17] =
{
    "                       A ",
    "           AA         AA ",
    "    N.T. AAAAA       AAAA ",
    "     AAAAAAAAAAA  AAAAAAAA ",
    "   AAAAAAAAAAAAAAAAAAAAAAAAA Qld.",
    " AAAAAAAAAAAAAAAAAAAAAAAAAAAA ",
    " AAAAAAAAAAAAAAAAAAAAAAAAAAAAA ",
    " AAAAAAAAAAAAAAAAAAAAAAAAAAAA ",
    "   AAAAAAAAAAAAAAAAAAAAAAAAA N.S.W.",
    "W.A. AAAAAAAAA      AAAAAA Vic.",
    "       AAA   S.A.     AA",
    "                       A  Tas.",
    ""
};

/* Funny messages for the scroller */

char *messages[] =
{
    "Hello from the Land Down Under",
    "The Land of crocs, and a big Red Rock",
    "Where the sunflower runs along the highways",
    "The dusty red roads lead one to loneliness",
    "Blue sky in the morning and",
    "Freezing nights and twinkling stars",
    NULL
};

static void (*WaitForUser__callback)(void*);
void WaitForUser_(void *arg)
{
    chtype ch = (chtype)arg;

    nodelay(stdscr, FALSE);
    nocbreak();     /* Reset the halfdelay() value */
    cbreak();
    (*WaitForUser__callback)((void*)((ch == '\033') ? ch : 0));
}
void WaitForUser(void (*callback)(void*))
{
    nodelay(stdscr, TRUE);
    halfdelay(50);

    WaitForUser__callback = callback;
    getch_async(WaitForUser_);
}


int SubWinTest(WINDOW *win, void (*callback)(void*))
{
    WINDOW *swin1, *swin2, *swin3;
    int w, h, sw, sh, bx, by;

    wattrset(win, 0);
    getmaxyx(win, h, w);
    getbegyx(win, by, bx);

    sw = w / 3;
    sh = h / 3;

    if ((swin1 = derwin(win, sh, sw, 3, 5)) == NULL)
        return 1;
    if ((swin2 = subwin(win, sh, sw, by + 4, bx + 8)) == NULL)
        return 1;
    if ((swin3 = subwin(win, sh, sw, by + 5, bx + 11)) == NULL)
        return 1;

    init_pair(8, COLOR_RED, COLOR_BLUE);
    wbkgd(swin1, COLOR_PAIR(8));
    werase(swin1);
    mvwaddstr(swin1, 0, 3, "Sub-window 1");
    wrefresh(swin1);

    init_pair(9, COLOR_CYAN, COLOR_MAGENTA);
    wbkgd(swin2, COLOR_PAIR(9));
    werase(swin2);
    mvwaddstr(swin2, 0, 3, "Sub-window 2");
    wrefresh(swin2);

    init_pair(10, COLOR_YELLOW, COLOR_GREEN);
    wbkgd(swin3, COLOR_PAIR(10));
    werase(swin3);
    mvwaddstr(swin3, 0, 3, "Sub-window 3");
    wrefresh(swin3);

    delwin(swin1);
    delwin(swin2);
    delwin(swin3);
    WaitForUser(callback);

    return 0;
}

struct BouncingBallsContext
{
    chtype c1, c2, c3, ball1, ball2, ball3;
    int w, h, x1, y1, xd1, yd1, x2, y2, xd2, yd2, x3, y3, xd3, yd3, c;
    WINDOW *win;
    void (*callback)(void*);
} bbctx;
int BouncingBalls(WINDOW *win, void (*callback)(void*))
{
    bbctx.win = win;
    bbctx.callback = callback;
    curs_set(0);

    wbkgd(win, COLOR_PAIR(1));
    wrefresh(win);
    wattrset(win, 0);

    init_pair(11, COLOR_RED, COLOR_GREEN);
    init_pair(12, COLOR_BLUE, COLOR_RED);
    init_pair(13, COLOR_YELLOW, COLOR_WHITE);

    bbctx.ball1 = 'O' | COLOR_PAIR(11);
    bbctx.ball2 = '*' | COLOR_PAIR(12);
    bbctx.ball3 = '@' | COLOR_PAIR(13);

    getmaxyx(win, bbctx.h, bbctx.w);

    bbctx.x1 = 2 + rand() % (bbctx.w - 4);
    bbctx.y1 = 2 + rand() % (bbctx.h - 4);
    bbctx.x2 = 2 + rand() % (bbctx.w - 4);
    bbctx.y2 = 2 + rand() % (bbctx.h - 4);
    bbctx.x3 = 2 + rand() % (bbctx.w - 4);
    bbctx.y3 = 2 + rand() % (bbctx.h - 4);

    bbctx.xd1 = 1;
    bbctx.yd1 = 1;
    bbctx.xd2 = 1;
    bbctx.yd2 = -1;
    bbctx.xd3 = -1;
    bbctx.yd3 = 1;

    nodelay(stdscr, TRUE);
    BouncingBalls_1(NULL);
    return 0;
}
void BouncingBalls_1(void *arg)
{
    getch_async(BouncingBalls_2);
}
void BouncingBalls_2(void *arg)
{
    WINDOW *win = bbctx.win;
    int ch = (int)arg;
    int h = bbctx.h;
    int w = bbctx.w;
    if(ch == ERR)
    {
        bbctx.x1 += bbctx.xd1;
        if (bbctx.x1 <= 1 || bbctx.x1 >= w - 2)
            bbctx.xd1 *= -1;

        bbctx.y1 += bbctx.yd1;
        if (bbctx.y1 <= 1 || bbctx.y1 >= h - 2)
            bbctx.yd1 *= -1;

        bbctx.x2 += bbctx.xd2;
        if (bbctx.x2 <= 1 || bbctx.x2 >= w - 2)
            bbctx.xd2 *= -1;

        bbctx.y2 += bbctx.yd2;
        if (bbctx.y2 <= 1 || bbctx.y2 >= h - 2)
            bbctx.yd2 *= -1;

        bbctx.x3 += bbctx.xd3;
        if (bbctx.x3 <= 1 || bbctx.x3 >= w - 2)
            bbctx.xd3 *= -1;

        bbctx.y3 += bbctx.yd3;
        if (bbctx.y3 <= 1 || bbctx.y3 >= h - 2)
            bbctx.yd3 *= -1;

        bbctx.c1 = mvwinch(win, bbctx.y1, bbctx.x1);
        bbctx.c2 = mvwinch(win, bbctx.y2, bbctx.x2);
        bbctx.c3 = mvwinch(win, bbctx.y3, bbctx.x3);

        mvwaddch(win, bbctx.y1, bbctx.x1, bbctx.ball1);
        mvwaddch(win, bbctx.y2, bbctx.x2, bbctx.ball2);
        mvwaddch(win, bbctx.y3, bbctx.x3, bbctx.ball3);

        wmove(win, 0, 0);
        wrefresh(win);

        mvwaddch(win, bbctx.y1, bbctx.x1, bbctx.c1);
        mvwaddch(win, bbctx.y2, bbctx.x2, bbctx.c2);
        mvwaddch(win, bbctx.y3, bbctx.x3, bbctx.c3);

        napms_async(150, BouncingBalls_1);
    }
    else{
        nodelay(stdscr, FALSE);
        ungetch(ch);
        (*(bbctx.callback))(NULL);
    }
}

/* Trap interrupt */

void trap(int sig)
{
    if (sig == SIGINT)
    {
        endwin();

        exit(0);
    }
}

// emscripten: globals for async functions
WINDOW * ml_win;
chtype ml_ch;
chtype ml_save[80];
int ml_i, ml_j;
int ml_width, ml_height;
int ml_w;
char * ml_message;
int ml_msg_len;
int ml_scroll_len;
char *ml_scrollbuf;
char *ml_visbuf;
int ml_stop;
void async_1(void * arg)
{
    WINDOW * win = ml_win;

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    wbkgd(win, COLOR_PAIR(1));
    werase(win);

    init_pair(2, COLOR_RED, COLOR_RED);
    wattrset(win, COLOR_PAIR(2));
    box(win, ' ', ' ');
    wrefresh(win);
  
    wattrset(win, 0);

    /* Do random output of a character */

    ml_ch = 'a';

    nodelay(stdscr, TRUE);

    ml_i = 0;
    async_2(NULL);
}
void async_2(void * arg)
{
    WINDOW * win = ml_win;
    int x,y;
    while(ml_i < 5000)
    {
        x = rand() % (ml_width - 2) + 1;
        y = rand() % (ml_height - 2) + 1;

        mvwaddch(win, y, x, ml_ch);
        wrefresh(win);

        getch_async(async_3);
        return;
    }
    async_4(NULL);
}
void async_3(void * arg)
{
    WINDOW * win = ml_win;
    int ret = (int)arg;
    if (ret != ERR)
    {
        async_4(NULL);
        return;
    }

    if (ml_i == 2000)
    {
        ml_ch = 'b';
        init_pair(3, COLOR_CYAN, COLOR_YELLOW);
        wattrset(win, COLOR_PAIR(3));
    }

    ++ ml_i;
    // async call to avoid overflowing the stack
    napms_async(1, async_2);
}
void async_4(void *arg)
{
    nodelay(stdscr, FALSE);
    SubWinTest(ml_win, async_15);
}
 
void async_15(void *arg)
{
    WINDOW * win = ml_win;
    /* Erase and draw green window */

    init_pair(4, COLOR_YELLOW, COLOR_GREEN);
    wbkgd(win, COLOR_PAIR(4));
    wattrset(win, A_BOLD);
    werase(win);
    wrefresh(win);

    /* Draw RED bounding box */

    wattrset(win, COLOR_PAIR(2));
    box(win, ' ', ' ');
    wrefresh(win);

    /* Display Australia map */

    wattrset(win, A_BOLD);
    ml_i = 0;
    async_5(NULL);
}
void async_5(void * arg)
{
    WINDOW * win = ml_win;
    while (*AusMap[ml_i])
    {
        mvwaddstr(win, ml_i + 1, 8, AusMap[ml_i]);
        wrefresh(win);
        ++ml_i;
        napms_async(100, async_5);
        return;
    }
    async_6(NULL);
}
void async_6(void *arg)
{
    WINDOW * win = ml_win;
    int height = ml_height;
    int width = ml_width;

    init_pair(5, COLOR_BLUE, COLOR_WHITE);
    wattrset(win, COLOR_PAIR(5) | A_BLINK);
    mvwaddstr(win, height - 2, 3,
            " PDCurses 3.4 - DOS, OS/2, Win32, X11, SDL");
    wrefresh(win);

    /* Draw running messages */

    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    wattrset(win, COLOR_PAIR(6));
    ml_w = width - 2;
    nodelay(win, TRUE);

    ml_j = 0;
    async_7(NULL);
}

void async_7(void * arg)
{
    /* jbuhler's re-hacked scrolling messages */

    while(messages[ml_j] != NULL)
    {
        ml_message = messages[ml_j];
        ml_msg_len = strlen(ml_message);
        ml_scroll_len = ml_w + 2 * ml_msg_len;
        ml_scrollbuf = malloc(ml_scroll_len);
        ml_visbuf = ml_scrollbuf + ml_msg_len;
        ml_stop = 0;
        ml_i = ml_w + ml_msg_len;
        ++ml_j;
        async_8(NULL);
        return;
    }
    async_11(NULL);
}

void async_8(void * arg)
{
    WINDOW *win = ml_win;
    while(ml_i > 0)
    {
        memset(ml_visbuf, ' ', ml_w);
        strncpy(ml_scrollbuf + ml_i, ml_message, ml_msg_len);
        mvwaddnstr(win, ml_height / 2, 1, ml_visbuf, ml_w);
        wrefresh(win);

        --ml_i;
        wgetch_async(win, async_9);
        return;
    }
    async_10(NULL);
}

void async_9(void * arg)
{
    int ret = (int)arg;
    if(ret != ERR)
    {
        flushinp();
        ml_stop = 1;
        async_10(NULL);
        return;
    }
    napms_async(100,async_8);
}
void async_10(void * arg)
{
    free(ml_scrollbuf);
    if (ml_stop)
    {
        async_11(NULL);
        return;
    }
    async_7(NULL);
}
void async_11(void * arg)
{
    int j = 0;
    int i;
    int width = ml_width;
    int height = ml_width;
    WINDOW *win = ml_win;
    chtype ch;

    /*  Draw running 'A's across in RED */

    init_pair(7, COLOR_RED, COLOR_GREEN);
    wattron(win, COLOR_PAIR(7));

    for (i = 2; i < width - 4; ++i)
    {
        ch = mvwinch(win, 5, i);
        ml_save[j++] = ch;
        ch = ch & 0x7f;
        mvwaddch(win, 5, i, ch);
    }

    wrefresh(win);

    /* Put a message up; wait for a key */

    i = height - 2;
    wattrset(win, COLOR_PAIR(5));
    mvwaddstr(win, i, 3,
        "   Type a key to continue or ESC to quit  ");
    wrefresh(win);
    WaitForUser(async_12);
}

void async_12(void * arg)
{
    int ret = (int)arg;
    if (ret == '\033')
    {
        async_14(NULL);
        return;
    }

    WINDOW *win = ml_win;
    /* Restore the old line */

    wattrset(win, 0);

    int i,j;
    for (i = 2, j = 0; i < ml_width - 4; ++i)
        mvwaddch(win, 5, i, ml_save[j++]);

    wrefresh(win);

    BouncingBalls(win, async_16);
}

void async_16(void *arg)
{
    /* BouncingBalls() leaves a keystroke in the queue */

    WaitForUser(async_13);
}
void async_13(void *arg)
{
    int ret = (int)arg;
    if (ret == '\033')
    {
        async_14(NULL);
        return;
    }
    async_1(NULL);
}

void async_14(void * arg)
{
    endwin();
}

int main(int argc, char **argv)
{
    WINDOW *win;
    int width, height, seed;

#ifdef XCURSES
    Xinitscr(argc, argv);
#else
    initscr();
#endif
    seed = time((time_t *)0);
        srand(seed);

    start_color();
# if defined(NCURSES_VERSION) || (defined(PDC_BUILD) && PDC_BUILD > 3000)
    use_default_colors();
# endif
    cbreak();
    noecho();

    curs_set(0);

#if !defined(__TURBOC__) && !defined(OS2)
    signal(SIGINT, trap);
#endif
    noecho();

    /* refresh stdscr so that reading from it will not cause it to 
       overwrite the other windows that are being created */

    refresh();

    /* Create a drawing window */

    width  = 48;
    height = 15;

    win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

    if (win == NULL)
    {
        endwin();

        return 1;
    }

    ml_win = win;
    ml_width = width;
    ml_height = height;
    async_1(NULL);
    return 0;
}
