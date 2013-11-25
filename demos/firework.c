/* $Id: firework.c,v 1.25 2008/07/13 16:08:17 wmcbrine Exp $ */

#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define DELAYSIZE 200

void myrefresh(void);
void get_color(void);
void explode(void);

short color_table[] =
{
    COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
    COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

// modified for emscripten
int ml_start, ml_end, ml_row, ml_diff, ml_flag, ml_direction;
int ml_step, ml_func;
int ml_explode_row, ml_explode_col;
int ml_do_erase;
void main_loop()
{
    if(ml_func == 0) {
        myrefresh(); 
        if(ml_step == 0) {
            do {
                ml_start = rand() % (COLS - 3);
                ml_end = rand() % (COLS - 3);
                ml_start = (ml_start < 2) ? 2 : ml_start;
                ml_end = (ml_end < 2) ? 2 : ml_end;
                ml_direction = (ml_start > ml_end) ? -1 : 1;
                ml_diff = abs(ml_start - ml_end);
            } while (ml_diff < 2 || ml_diff >= LINES - 2);

            attrset(A_NORMAL);

            ml_row = 0;
            ++ml_step;
        }

        while(ml_row < ml_diff)
        {
            if(ml_do_erase)
            {
                erase();
                ml_do_erase = 0;
            }
            mvaddstr(LINES - ml_row, ml_row * ml_direction + ml_start,
                    (ml_direction < 0) ? "\\" : "/");

            if (ml_flag++)
            {
                ml_flag = 0;
                ++ml_row;
                ml_do_erase = 1;
                return;
            }

            ++ml_row;
        }
        ml_do_erase = 0;

        if (ml_flag++)
        {
            ml_flag = 0;
            ml_func = 1;
            ml_step = 0;
            return;
        }

        ml_func = 1;
        ml_step = 0;
    } 
        
    if(ml_func == 1) {
        if(ml_step == 0) {
            ml_explode_row = LINES - ml_row;
            ml_explode_col = ml_diff * ml_direction + ml_start;
        }

        explode();
        return;
    }
}

int main(int argc, char **argv)
{
    int i, seed;

#ifdef XCURSES
    Xinitscr(argc, argv);
#else
    initscr();
#endif
    nodelay(stdscr, TRUE);
    noecho();

    if (has_colors())
        start_color();

    for (i = 0; i < 8; i++)
        init_pair(i, color_table[i], COLOR_BLACK);

    seed = time((time_t *)0);
    srand(seed);
    ml_flag = 0;
       
    // for emscripten
    ml_step = 0;
    ml_func = 0;
    ml_do_erase = 0;
    emscripten_set_main_loop(main_loop, 1000 / DELAYSIZE);

    endwin();

    return 0;
}

void explode()
{
    myrefresh();
    if(ml_step == 0)
    {
        erase();
        mvaddstr(ml_explode_row, ml_explode_col, "-");
        ++ ml_step;
        return;
    } else if (ml_step == 1) {
        --ml_explode_col;
        get_color();
        mvaddstr(ml_explode_row - 1, ml_explode_col, " - ");
        mvaddstr(ml_explode_row,     ml_explode_col, "-+-");
        mvaddstr(ml_explode_row + 1, ml_explode_col, " - ");
        ++ ml_step;
        return;
    } else if (ml_step == 2) {
        --ml_explode_col;
        get_color();
        mvaddstr(ml_explode_row - 2, ml_explode_col, " --- ");
        mvaddstr(ml_explode_row - 1, ml_explode_col, "-+++-");
        mvaddstr(ml_explode_row,     ml_explode_col, "-+#+-");
        mvaddstr(ml_explode_row + 1, ml_explode_col, "-+++-");
        mvaddstr(ml_explode_row + 2, ml_explode_col, " --- ");
        ++ ml_step;
        return;
    } else if (ml_step == 3) {
        get_color();
        mvaddstr(ml_explode_row - 2, ml_explode_col, " +++ ");
        mvaddstr(ml_explode_row - 1, ml_explode_col, "++#++");
        mvaddstr(ml_explode_row,     ml_explode_col, "+# #+");
        mvaddstr(ml_explode_row + 1, ml_explode_col, "++#++");
        mvaddstr(ml_explode_row + 2, ml_explode_col, " +++ ");
        ++ ml_step;
        return;
    } else if (ml_step == 4) {
        get_color();
        mvaddstr(ml_explode_row - 2, ml_explode_col, "  #  ");
        mvaddstr(ml_explode_row - 1, ml_explode_col, "## ##");
        mvaddstr(ml_explode_row,     ml_explode_col, "#   #");
        mvaddstr(ml_explode_row + 1, ml_explode_col, "## ##");
        mvaddstr(ml_explode_row + 2, ml_explode_col, "  #  ");
        ++ ml_step;
        return;
    } else if (ml_step == 5) {
        get_color();
        mvaddstr(ml_explode_row - 2, ml_explode_col, " # # ");
        mvaddstr(ml_explode_row - 1, ml_explode_col, "#   #");
        mvaddstr(ml_explode_row,     ml_explode_col, "     ");
        mvaddstr(ml_explode_row + 1, ml_explode_col, "#   #");
        mvaddstr(ml_explode_row + 2, ml_explode_col, " # # ");
        ++ml_step;
        return;
    } else if (ml_step == 6) {
        erase();
        ml_func = 0;
        ml_step = 0;
        return;
    }
}

void myrefresh(void)
{
    //napms(DELAYSIZE);
    move(LINES - 1, COLS - 1);
    refresh();
}

void get_color(void)
{
    chtype bold = (rand() % 2) ? A_BOLD : A_NORMAL;
    attrset(COLOR_PAIR(rand() % 8) | bold);
}
