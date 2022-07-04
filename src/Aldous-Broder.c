#include "maze_tools.h"
#include "Aldous-Broder.h"
#include "graphics.h"
#include <stdlib.h>
#include <debug.h>


int AB_gen(maze_t *maze, int startrow, int startcol){
    
    int numToVisit = maze->rows * maze->cols;
    int cell_row = startrow, cell_col = startcol;
    int directions[4], ndirs;
    cell_t *currentCell, *nextCell;
    int lim = (numToVisit * 2 / 3);

    while(numToVisit > lim){
        currentCell = &maze->cells[cell_row][cell_col];
        if(!currentCell->visited){
            dbg_sprintf(dbgout, "numToVisit: %d\n", numToVisit);
            // dbg_sprintf(dbgout, "pos: %d, %d\n", cell_col, cell_row);
            // print_maze(maze);
        }
        
        
        currentCell->visited = 1;
        ndirs = getAdjacentNum_IGNORE_VISIT(directions, cell_row, cell_col, maze->rows, maze->cols);
        

        switch(directions[rand() % ndirs]){
            case North:
                nextCell = &maze->cells[cell_row - 1][cell_col];
                if(!nextCell->visited){
                    numToVisit--;
                    nextCell->south = 0;
                }
                cell_row--;
                break;
            case East:
                nextCell = &maze->cells[cell_row][cell_col + 1];
                if(!nextCell->visited){
                    numToVisit--;
                    currentCell->east = 0;
                }
                cell_col++;
                break;
            case South:
                nextCell = &maze->cells[cell_row + 1][cell_col];
                if(!nextCell->visited){
                    numToVisit--;
                    currentCell->south = 0;
                }
                cell_row++;
                break;
            case West:
                nextCell = &maze->cells[cell_row][cell_col - 1];
                if(!nextCell->visited){
                    numToVisit--;
                    nextCell->east = 0;
                }
                cell_col--;
                break;
        }
    }
    

    return numToVisit;
}