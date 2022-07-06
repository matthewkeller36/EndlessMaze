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
#include "input.h"

enum{MENU, GENERATE, PLAY, QUIT};

struct game{
    maze_t maze;
    char state;
    uint8_t cellSize;
    uint8_t bgColor, wallColor, playerColor, goalColor;
};

struct player{
    uint8_t row, col;
};

int main(void){
    static struct game game;
    struct player player;
    ti_var_t file;
    gfx_Begin();
    if(!(file = ti_Open("EMSETT", "r"))){
        game.bgColor = 255;
        game.wallColor = 0;
        game.playerColor = 31;
        game.goalColor = 37;
        game.maze.rows = 20;
        game.maze.cols = 20;
    }else{
        ti_Read(&game.bgColor, 1, 1, file);
        ti_Read(&game.wallColor, 1, 1, file);
        ti_Read(&game.playerColor, 1, 1, file);
        ti_Read(&game.goalColor, 1, 1, file);
        ti_Read(&game.maze.rows, 1, 1, file);
        ti_Read(&game.maze.cols, 1, 1, file);
    }
    game.state = GENERATE;
    timer_Disable(1);
    timer_SetReload(1, 32768 / 5);
    timer_Set(1, 32768 / 5);
    srand(rtc_Time());

    while(game.state != QUIT){
        if(game.state == MENU){

        }else if(game.state == GENERATE){
            walled_maze(&game.maze);
            clear_visited(&game.maze);
            int numToVisit = AB_gen(&game.maze, 0, 0);
            numToVisit = wilsons_gen(&game.maze, numToVisit);
            if(numToVisit == 0) game.state = PLAY;
            player.row = player.col = 0;
            game.cellSize = (((MAZE_Y_MAX - MAZE_Y_MIN)) / game.maze.rows < ((MAZE_X_MAX - MAZE_X_MIN))  / game.maze.cols ? 
                (MAZE_Y_MAX - MAZE_Y_MIN) / game.maze.rows : 
                (MAZE_X_MAX - MAZE_X_MIN) / game.maze.cols);
            gfx_dispMaze(&game.maze, game.bgColor, game.wallColor, game.playerColor, game.cellSize);
            continue;
        }else if(game.state == PLAY){
            int8_t moveDir = getMoveDir();
            if(moveDir == -1){
                continue;
            }
            gfx_SetColor(game.bgColor);
            gfx_FillRectangle(MAZE_X_MIN + player.col * game.cellSize + 1, MAZE_Y_MIN + player.row * game.cellSize + 1, game.cellSize - 1, game.cellSize - 1);

            switch(moveDir){
                case North:
                    if(player.row > 0 && !game.maze.cells[player.row - 1][player.col].south){
                        player.row--;
                    }
                    break;
                case East:
                    if(player.col < game.maze.cols && !game.maze.cells[player.row][player.col].east){
                        player.col++;
                    }
                    break;
                case South:
                    if(player.row < game.maze.rows && !game.maze.cells[player.row][player.col].south){
                        player.row++;
                    }
                    break;
                case West:
                    if(player.col > 0 && !game.maze.cells[player.row][player.col - 1].east){
                        player.col--;
                    }
                    break;

            }
            gfx_SetColor(game.playerColor);
            gfx_FillRectangle(MAZE_X_MIN + player.col * game.cellSize + 1, MAZE_Y_MIN + player.row * game.cellSize + 1, game.cellSize - 1, game.cellSize - 1);
        }
    }
    
    gfx_End();
    return 0;
}
