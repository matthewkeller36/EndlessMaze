#ifndef ALDOUS_BRODER_H
#define ALDOUS_BRODER_H

#include "maze_tools.h"

/**
 * @brief Generates 1/3 of a maze from a blank walled map using the Aldous-Broder method.
 * 
 * @param maze Pointer to maze structure to generate.
 * @param startrow Row in maze to start generation from.
 * @param startcol Column in maze to start generation from.
 * @return Number of remaining cells. 
 */
int AB_gen(maze_t *maze, uint8_t cellSize, uint8_t dispPerCell);

#endif