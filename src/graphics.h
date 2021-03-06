#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "maze_tools.h"
#include <stdint.h>

/**
 * @brief Prints a map to the screen using color arguments
 * 
 * @param maze Pointer to maze structure
 * @param bgColor Color to be used for the background
 * @param wallColor Color to be used for the walls
 * @param playerColor Color to be used for the player
 * @param cellSize Size of cells to be drawn to the screen.
 */
void gfx_dispMaze(maze_t * maze, uint8_t bgColor, uint8_t wallColor, uint8_t playerColor, uint8_t cellSize);

#endif