#include <tice.h>
#include <stdint.h>
#include <graphx.h>
#include <debug.h>

#include "graphics.h"

#define FRAME_LIMIT 60
#define NUM_FRAMES_PER_MOVE 3

void init_graphics(){
    gfx_Begin();
    gfx_SetDrawBuffer();

    // Set up timer for maximum framerate.
    timer_Set(1, 32678 / FRAME_LIMIT);
    timer_SetReload(1, 32678 / FRAME_LIMIT);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
}

/**
 * @brief Waits until enough time has elapsed since previous frame for next one.
 * 
 */
void waitNextFrame(){
    while(!timer_ChkInterrupt(1, TIMER_RELOADED));
    timer_Set(1, 32678 / FRAME_LIMIT);
    timer_AckInterrupt(1, TIMER_RELOADED);
    return;
}


void gfx_dispMaze(maze_t *maze, uint8_t bgColor, uint8_t wallColor, uint8_t goalColor, uint8_t cellSize){
    
    int mazeWidth = cellSize * maze->cols;
    int mazeHeight = cellSize * maze->rows;

    gfx_FillScreen(bgColor);
    gfx_SetColor(goalColor);
    gfx_FillRectangle_NoClip(MAZE_X_MIN + cellSize * (maze->cols - 1) + 1, MAZE_Y_MIN + cellSize * (maze->rows - 1) + 1, 
        cellSize - 1, cellSize - 1);
    gfx_SetColor(wallColor);
    gfx_Rectangle_NoClip(MAZE_X_MIN, MAZE_Y_MIN, mazeWidth + 1, mazeHeight + 1);
    for(uint8_t i = 0; i < maze->rows; i++){
        for(uint8_t j = 0; j < maze->cols; j++){
            if(maze->cells[i][j].east){
                gfx_VertLine_NoClip(MAZE_X_MIN + (1 + j) * cellSize, MAZE_Y_MIN + i * cellSize, cellSize + 1);
            }
            if(maze->cells[i][j].south){
                gfx_HorizLine_NoClip(MAZE_X_MIN + j * cellSize, MAZE_Y_MIN + (1 + i) * cellSize, cellSize + 1);
            }
        }
    }
    gfx_PrintStringXY("Width: ", MAZE_X_MIN, MAZE_Y_MIN + mazeHeight + 2);
    gfx_PrintInt(maze->cols, 1);
    gfx_PrintString(" Height: ");
    gfx_PrintInt(maze->rows, 1);
    gfx_SetColor(bgColor);
    waitNextFrame();
    gfx_BlitBuffer();
}

void gfx_hideWall(uint8_t row, uint8_t col, uint8_t direction, uint8_t cellSize){
    if(direction == dir_East){
        gfx_VertLine_NoClip(MAZE_X_MIN + (1 + col) * cellSize, MAZE_Y_MIN + row * cellSize + 1, cellSize - 1);
    }else{
        gfx_HorizLine_NoClip(MAZE_X_MIN + col * cellSize + 1, MAZE_Y_MIN + (1 + row) * cellSize, cellSize - 1);
    }
    gfx_BlitRectangle(gfx_buffer, MAZE_X_MIN + col * cellSize, MAZE_Y_MIN + row * cellSize, cellSize + 1, cellSize + 1);
}

void render_play(struct game_s *game, struct player_s *player){

    // Not initialized as steps. Used to find starting position before division due to 
    // possible pixel loss when dividing.
    int8_t xStep = game->cellSize * ((player->moveDir == dir_East) - (player->moveDir == dir_West));
    int8_t yStep = game->cellSize * ((player->moveDir == dir_South) - (player->moveDir == dir_North));

    int playerStartX = MAZE_X_MIN + player->col * game->cellSize + 1 - xStep;
    int playerStartY = MAZE_Y_MIN + player->row * game->cellSize + 1 - yStep;

    // Set pixel steps to be... well... actual steps.
    xStep /= NUM_FRAMES_PER_MOVE;
    yStep /= NUM_FRAMES_PER_MOVE;

    // Pre compute buffer blitting parameters
    unsigned int bufX = playerStartX - game->cellSize * (xStep < 0);
    unsigned int bufY = playerStartY - game->cellSize * (yStep < 0);
    unsigned int bufWidth = game->cellSize * (1 + (xStep != 0));
    unsigned int bufHeight = game->cellSize * (1 + (yStep != 0));

    // Remove original player
    gfx_SetColor(game->bgColor);
    gfx_FillRectangle_NoClip(playerStartX, playerStartY, game->cellSize - 1, game->cellSize - 1);

    dbg_sprintf(dbgout, "X: %d Y:%d\n", playerStartX, playerStartY);
    dbg_sprintf(dbgout, "xStep: %d yStep:%d, Cell Size: %d\n", xStep, yStep, game->cellSize);

    for(uint8_t i = 0; i < NUM_FRAMES_PER_MOVE; i++){
        // Remove old player and draw new player mid animation.
        gfx_SetColor(game->playerColor);
        gfx_FillRectangle_NoClip(playerStartX + xStep * (i + 1), playerStartY + yStep * (i + 1), game->cellSize - 1, game->cellSize - 1);
        waitNextFrame();
        gfx_BlitRectangle(gfx_buffer, bufX, bufY, bufWidth, bufHeight);
        gfx_SetColor(game->bgColor);
        gfx_FillRectangle_NoClip(playerStartX + xStep * (i + 1), playerStartY + yStep * (i + 1), game->cellSize - 1, game->cellSize - 1);
    }
    gfx_SetColor(game->playerColor);
    gfx_FillRectangle_NoClip(playerStartX + game->cellSize * (xStep != 0), playerStartY + game->cellSize * (yStep != 0), game->cellSize - 1, game->cellSize - 1);
    gfx_BlitRectangle(gfx_buffer, bufX, bufY, bufWidth, bufHeight);
}