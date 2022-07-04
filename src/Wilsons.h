#ifndef WILSONS_H
#define WILSONS_H

#include "maze_tools.h"

/**
 * @brief Generates a maze using Wilson's Algorithm.
 * 
 * @param maze Pointer to maze to be generated.
 * @param numToVisit Number of unvisited cells.
 * @return Remaining number of cells to visit (should be 0).
 */
int wilsons_gen(maze_t *maze, int numToVisit);

#endif