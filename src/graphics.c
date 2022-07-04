#include <tice.h>
#include <stdint.h>

#include "graphics.h"


void gfx_dispMaze(maze_t *maze, uint8_t bgColor, uint8_t wallColor, uint8_t playerColor){
    int cellSize = (((MAZE_Y_MAX - MAZE_Y_MIN)) / maze->rows < ((MAZE_X_MAX - MAZE_X_MIN))  / maze->cols ? 
        (MAZE_Y_MAX - MAZE_Y_MIN) / maze->rows : 
        (MAZE_X_MAX - MAZE_X_MIN) / maze->cols);
    int mazeWidth = cellSize * maze->cols;
    int mazeHeight = cellSize * maze->rows;

    gfx_FillScreen(bgColor);
    gfx_SetColor(wallColor);
    gfx_Rectangle(MAZE_X_MIN, MAZE_Y_MIN, mazeWidth + 1, mazeHeight + 1);
    for(int i = 0; i < maze->rows; i++){
        for(int j = 0; j < maze->cols; j++){
            if(maze->cells[i][j].east){
                gfx_VertLine(MAZE_X_MIN + (1 + j) * cellSize, MAZE_Y_MIN + i * cellSize, cellSize + 1);
            }
            if(maze->cells[i][j].south){
                gfx_HorizLine(MAZE_X_MIN + j * cellSize, MAZE_Y_MIN + (1 + i) * cellSize, cellSize + 1);
            }
        }
    }
    gfx_PrintStringXY("Width: ", MAZE_X_MIN, MAZE_Y_MIN + mazeHeight + 2);
    gfx_PrintInt(maze->cols, 1);
    gfx_PrintString(" Height: ");
    gfx_PrintInt(maze->rows, 1);

}