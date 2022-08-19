#include <stdio.h>
#include <debug.h>
#include <stdint.h>
#include "maze_tools.h"

void print_maze(maze_t *maze){
    /* Print the top border */
    for(int i = 0; i < 2 * maze->cols + 2; i++){
        dbg_sprintf(dbgout, "#");
    }
    dbg_sprintf(dbgout, "\n");
    /*Print body of the maze*/
    for(int i = 0; i < 2 * maze->rows; i++){
        dbg_sprintf(dbgout, "#");
        /*Each cell is 2x2, so print side walls for half of the rows 
            and vertical walls for the other half*/
        if(!(i & 1)){
            for(int j = 0; j < maze->cols; j++){
                dbg_sprintf(dbgout, " ");
                dbg_sprintf(dbgout, "%c", (maze->cells[i >> 1][j].east) ? '#' : ' ');
            }
        }
        else{
            for(int j = 0; j < maze->cols; j++){
                dbg_sprintf(dbgout, "%c#", (maze->cells[i >> 1][j].south) ? '#' : ' ');
            }
            
        }
        dbg_sprintf(dbgout, "\n");
    }
}

void walled_maze(maze_t *maze){
    for(uint8_t i = 0; i < maze->rows; i++){
        for(uint8_t j = 0; j < maze->cols; j++){
            maze->cells[i][j].east = 1;
            maze->cells[i][j].south = 1;
        }
    }
}

void clear_visited(maze_t *maze){
    for(uint8_t i = 0; i < maze->rows; i++){
        for(uint8_t j = 0; j < maze->cols; j++){
            maze->cells[i][j].visited = 0;
        }
    }
}

uint8_t getAdjacentNum_IGNORE_VISIT(uint8_t dir[], uint8_t cell_row, uint8_t cell_col, uint8_t maxRow, uint8_t maxCol){
    uint8_t ndir = 0;
    if(cell_row > 0){
        dir[ndir++] = dir_North;
    }
    if(cell_col < maxCol - 1){
        dir[ndir++] = dir_East;
    }
    if(cell_row < maxRow - 1){
        dir[ndir++] = dir_South;
    }
    if(cell_col > 0){
        dir[ndir++] = dir_West;
    }
    return ndir;
}

int getWallCount(maze_t *maze){
    int retVal = 0;
    for(int i = 0; i < maze->rows; i++){
        for(int j = 0; j < maze->cols; j++){
            retVal += maze->cells[i][j].east + maze->cells[i][j].south;
        }
    }
    return retVal;
}

void printUnvisitIndex(maze_t *maze){
    int totalRemain = 0;
    for(int i = 0; i < maze->rows; i++){
        for(int j = 0; j < maze->cols; j++){
            if(!maze->cells[i][j].visited){
                dbg_sprintf(dbgout, "[%d][%d] unvisited\n", i, j);
                totalRemain++;
            }
        }
    }
    dbg_sprintf(dbgout, "Total remaining: %d\n", totalRemain);
}