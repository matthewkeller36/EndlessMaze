#include <stdlib.h>
#include <debug.h>
#include <stdint.h>
#include <tice.h>
#include "Wilsons.h"
#include "graphics.h"

/**
 * @brief Adds Wilson chain to a maze starting at the given row and column.
 * 
 * @param maze Pointer to maze structure to add chain to.
 * @param row Starting row of the chain.
 * @param col Starting column of the chain.
 * @return Number of cells added from the chain.
 */
int build_wilson_chain(maze_t *maze, uint8_t row, uint8_t col){
    cell_t *currCell = &maze->cells[row][col];
    int visited = 0;
    /* Iterate until chain has reached a visited cell */
    while(!currCell->visited){
        /* Increased visited number of cells, move in Wilson direction */
        visited++;
        currCell->visited = 1;
        switch(currCell->wilson_dir){
            case North:
                row--;
                currCell = &maze->cells[row][col];
                currCell->south = 0;
                break;
            case East:
                col++;
                currCell->east = 0;
                currCell = &maze->cells[row][col];
                break;
            case South:
                row++;
                currCell->south = 0;
                currCell = &maze->cells[row][col];
                break;
            case West:
                col--;
                currCell = &maze->cells[row][col];
                currCell->east = 0;
                break;
        }
        dbg_sprintf(dbgout, "Walls Remaining: %d\n",getWallCount(maze));
    }
    return visited;
}

int wilsons_gen(maze_t *maze, int numToVisit){
    /*Seed a new chain at a random starting point*/
    uint8_t cur_col = rand() % maze->cols;
    uint8_t cur_row = rand() % maze->rows;
    uint8_t start_col = cur_col;
    uint8_t start_row = cur_row;
    uint8_t newChain = 1;

    uint8_t directions[4], ndirs;
    

    while(numToVisit > 0){
        /* If starting cell has been added already, find a new starting cell */
        if(maze->cells[start_row][start_col].visited){
            start_col = rand() % maze->cols;
            start_row = rand() % maze->rows;
            continue;
        }
        /* Set the current position to the starting if a new chain */
        if(newChain){
            cur_col = start_col;
            cur_row = start_row;
            newChain = 0;
        }
        cell_t *currentCell = &maze->cells[cur_row][cur_col];
        /* Add the chain to the maze. Set the flag to start a new chain */
        
        if(currentCell->visited){
            numToVisit -= build_wilson_chain(maze, start_row, start_col);
            dbg_sprintf(dbgout, "remaining: %d\n", numToVisit);
            newChain = 1;
            continue;
        }

        /* Find the number of directions to move from current cell and move. Save direction for building chain */
        ndirs = getAdjacentNum_IGNORE_VISIT(directions, cur_row, cur_col, maze->rows, maze->cols);
        currentCell->wilson_dir = directions[rand() % ndirs];
        switch(currentCell->wilson_dir){
            case North:
                cur_row--;
                break;
            case East:
                cur_col++;
                break;
            case South:
                cur_row++;
                break;
            case West:
                cur_col--;
                break;
        }
    }
    
    return numToVisit;
}