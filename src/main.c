/*
 *--------------------------------------
 * Program Name: ENDLSSMZ
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

int main(void){
    static struct game_s game;
    struct player_s player;
    uint8_t file;
    if(!(file = ti_Open("EMSETT", "r"))){
        game.bgColor = 255;
        game.wallColor = 0;
        game.playerColor = 31;
        game.goalColor = 37;
        game.maze.rows = 10;
        game.maze.cols = 10;
    }else{
        ti_Read(&game.bgColor, 1, 1, file);
        ti_Read(&game.wallColor, 1, 1, file);
        ti_Read(&game.playerColor, 1, 1, file);
        ti_Read(&game.goalColor, 1, 1, file);
        ti_Read(&game.maze.rows, 1, 1, file);
        ti_Read(&game.maze.cols, 1, 1, file);
    }
    ti_Close(file);
    game.state = GENERATE;
    timer_Disable(3);
    timer_SetReload(3, 32768 / 5);
    timer_Set(3, 32768 / 5);
    
    srand(rtc_Time());
    init_graphics();

    while(game.state != QUIT){
        if(game.state == MENU){

        }else if(game.state == GENERATE){

            // Fill maze and clear visited flags (necessary conditions for both generation methods)
            walled_maze(&game.maze);
            clear_visited(&game.maze);

            // Generate ~1/3 of the maze using Aldous-Broder
            int numToVisit = AB_gen(&game.maze, 0, 0);

            // Generate remaining maze using Wilson algorithm, update game state
            numToVisit = wilsons_gen(&game.maze, numToVisit);
            if(numToVisit == 0) game.state = PLAY;
            player.row = player.col = 0;
            player.moveDir = -1;
            game.cellSize = (((MAZE_Y_MAX - MAZE_Y_MIN)) / game.maze.rows < ((MAZE_X_MAX - MAZE_X_MIN))  / game.maze.cols ? 
                (MAZE_Y_MAX - MAZE_Y_MIN) / game.maze.rows : 
                (MAZE_X_MAX - MAZE_X_MIN) / game.maze.cols);
            
            // Only display maze once. Rest will be partial redraws.
            gfx_dispMaze(&game.maze, game.bgColor, game.wallColor, game.goalColor, game.cellSize);
            continue;

        }else if(game.state == PLAY){
            enum keyinput thisMove = getMoveDir();
            switch(thisMove){
                case key_North:
                    if(player.row > 0 && !game.maze.cells[player.row - 1][player.col].south){
                        player.row--;
                        player.moveDir = dir_North;
                    }
                    break;
                case key_East:
                    if(player.col < game.maze.cols && !game.maze.cells[player.row][player.col].east){
                        player.col++;
                        player.moveDir = dir_East;
                    }
                    break;
                case key_South:
                    if(player.row < game.maze.rows && !game.maze.cells[player.row][player.col].south){
                        player.row++;
                        player.moveDir = dir_South;
                    }
                    break;
                case key_West:
                    if(player.col > 0 && !game.maze.cells[player.row][player.col - 1].east){
                        player.col--;
                        player.moveDir = dir_West;
                    }
                    break;
            }

            if(player.row == game.maze.rows - 1 && player.col == game.maze.cols - 1){
                game.state = QUIT;
            }
            
            render_play(&game, &player);
            player.moveDir = -1;
            
        }else if(game.state == QUIT){
            break;
        }
    }
    
    gfx_End();

    if((file = ti_Open("EMSETT", "w"))){
        ti_Write(&game.bgColor, 1, 1, file);
        ti_Write(&game.wallColor, 1, 1, file);
        ti_Write(&game.playerColor, 1, 1, file);
        ti_Write(&game.goalColor, 1, 1, file);
        ti_Write(&game.maze.rows, 1, 1, file);
        ti_Write(&game.maze.cols, 1, 1, file);
        ti_Close(file);
    }
    return 0;
}
