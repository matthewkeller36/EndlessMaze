#include "maze_tools.h"
#include "Aldous-Broder.h"
#include "graphics.h"
#include <stdlib.h>
#include <debug.h>
#include "graphics.h"
#include <tice.h>

int AB_gen(maze_t *maze, uint8_t cellSize, uint8_t dispPerCell){
    
    int numToVisit = maze->rows * maze->cols;
    uint8_t cell_row = maze->rows / 2, cell_col = maze->cols / 2;
    uint8_t directions[4], ndirs;
    cell_t *currentCell  = &maze->cells[cell_row][cell_col], *nextCell;
    int lim = (numToVisit * 2 / 3);

    while(numToVisit > lim || !currentCell->visited){
        
        currentCell->visited = 1;
        ndirs = getAdjacentNum_IGNORE_VISIT(directions, cell_row, cell_col, maze->rows, maze->cols);

        // Move in random direction
        switch(directions[rand() % ndirs]){
            case dir_North:
                nextCell = &maze->cells[cell_row - 1][cell_col];

                // If cell hasn't been visited, remove correct wall and decrease amount to visit
                if(!nextCell->visited){
                    numToVisit--;
                    nextCell->south = 0;
                    if(dispPerCell){
                        gfx_hideWall(cell_row - 1, cell_col, dir_South, cellSize);
                    }
                }
                cell_row--;
                break;
            case dir_East:
                nextCell = &maze->cells[cell_row][cell_col + 1];

                // If cell hasn't been visited, remove correct wall and decrease amount to visit
                if(!nextCell->visited){
                    numToVisit--;
                    currentCell->east = 0;
                    if(dispPerCell){
                        gfx_hideWall(cell_row, cell_col, dir_East, cellSize);
                    }
                }
                cell_col++;
                break;
            case dir_South:
                nextCell = &maze->cells[cell_row + 1][cell_col];

                // If cell hasn't been visited, remove correct wall and decrease amount to visit
                if(!nextCell->visited){
                    numToVisit--;
                    currentCell->south = 0;
                    if(dispPerCell){
                        gfx_hideWall(cell_row, cell_col, dir_South, cellSize);
                    }
                }
                cell_row++;
                break;
            case dir_West:
                nextCell = &maze->cells[cell_row][cell_col - 1];

                // If cell hasn't been visited, remove correct wall and decrease amount to visit
                if(!nextCell->visited){
                    numToVisit--;
                    nextCell->east = 0;
                    if(dispPerCell){
                        gfx_hideWall(cell_row, cell_col - 1, dir_East, cellSize);
                    }
                }
                cell_col--;
                break;
        }
        currentCell = &maze->cells[cell_row][cell_col];
    }
    
    dbg_sprintf(dbgout, "Ending AB\n");
    return numToVisit - 1;
}