/*
 *--------------------------------------
 * Program Name: MazeDash
 * Author: matkeller19
 * License:
 * Description:
 *--------------------------------------
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

#include "maze_tools.h"
#include "Aldous-Broder.h"
#include "Wilsons.h"
#include "graphics.h"

enum{MENU, GENERATE, PLAY, QUIT};

struct game{
    maze_t maze;
    char state;
    uint8_t bgColor, wallColor, playerColor;
};



int main(void){
    static struct game game = {
        .bgColor = 62,
        .playerColor = 13,
        .wallColor = 0,
        .maze.rows = 10,
        .maze.cols = 15
    };
    if(game.maze.rows > MAX_CELL_ROWS){
        game.maze.rows = MAX_CELL_ROWS;
    }
    if(game.maze.cols > MAX_CELL_COLS){
        game.maze.cols = MAX_CELL_COLS;
    }
    gfx_Begin();
    walled_maze(&game.maze);
    clear_visited(&game.maze);
    timer_Disable(1);
    timer_Set(1, 0);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
    int numToVisit = AB_gen(&game.maze, 0, 0);
    numToVisit = wilsons_gen(&game.maze, numToVisit);
    timer_Disable(1);
    gfx_dispMaze(&game.maze, game.bgColor, game.wallColor, game.playerColor);
    real_t elapsed_real;
    char str[10];
    elapsed_real = os_FloatToReal((float)timer_GetSafe(1, TIMER_UP) / 32768 <= 0.001f ? 0.0f : (float)timer_GetSafe(1, TIMER_UP) / 32768);
    os_RealToStr(str, &elapsed_real, 8, 1, 2);
    gfx_PrintString(" Time: ");
    gfx_PrintString(str);
    while(!os_GetCSC());
    gfx_End();
    return 0;
}
