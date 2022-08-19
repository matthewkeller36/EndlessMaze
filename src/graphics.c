#include <tice.h>
#include <stdint.h>
#include <graphx.h>
#include <debug.h>

#include "graphics.h"

#define FRAME_LIMIT 60
#define NUM_FRAMES_PER_MOVE 4

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
    gfx_BlitBuffer();

}

void render_play(struct game_s *game, struct player_s *player){

    int playerEndX = MAZE_X_MIN + player->col * game->cellSize + 1;
    int playerEndY = MAZE_Y_MIN + player->row * game->cellSize + 1;

    // Not initialized as steps. Used to find starting position before division due to 
    // possible pixel loss when dividing.
    int8_t xStep = game->cellSize * ((player->moveDir == key_East) - (player->moveDir == key_West));
    int8_t yStep = game->cellSize * ((player->moveDir == key_South) - (player->moveDir == key_North));

    // Clear player from screen at start of frame.
    gfx_SetColor(game->bgColor);
    gfx_FillRectangle_NoClip(playerEndX - xStep, playerEndY - yStep, game->cellSize - 1, game->cellSize - 1);

    // Set pixel steps to be... well... actual steps.
    xStep /= NUM_FRAMES_PER_MOVE;
    yStep /= NUM_FRAMES_PER_MOVE;

    for(uint8_t i = NUM_FRAMES_PER_MOVE; i > 0; i--){
        // Remove old player and draw new player mid animation.
        gfx_SetColor(game->bgColor);
        gfx_FillRectangle_NoClip(playerEndX - xStep * i, playerEndY - yStep * i, game->cellSize - 1, game->cellSize - 1);
        gfx_SetColor(game->playerColor);
        gfx_FillRectangle_NoClip(playerEndX - xStep * (i - 1), playerEndY - yStep * (i - 1), game->cellSize - 1, game->cellSize - 1);
        waitNextFrame();
        gfx_BlitBuffer();
    }
}