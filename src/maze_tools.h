#ifndef MAZE_TOOLS_H
#define MAZE_TOOLS_H

#include <stdint.h>

#define MAZE_X_MIN 10
#define MAZE_X_MAX 320 - MAZE_X_MIN
#define MAZE_Y_MIN 10
#define MAZE_Y_MAX 240 - MAZE_Y_MIN

#define MAX_CELL_COLS (int) ((MAZE_X_MAX - MAZE_X_MIN) / 4)
#define MAX_CELL_ROWS (int) ((MAZE_Y_MAX - MAZE_Y_MIN) / 4)

enum {dir_North, dir_East, dir_South, dir_West};

enum keyinput{
    key_None = -1, key_North, key_East, key_South, key_West, key_Clear
};

/**
 * @brief Cell structure used in making a maze.
 * 
 * Stores the walls, visited flag, and direction value used in Wilson's algorithm.
 * 
 */
typedef struct {
    unsigned east : 1;
    unsigned south : 1;
    unsigned visited : 1;
    unsigned wilson_dir : 2; // follows enum
    
}cell_t;

typedef struct {
    cell_t cells[MAX_CELL_ROWS][MAX_CELL_COLS];
    uint8_t rows, cols;
}maze_t;



struct game_s{
    maze_t maze;
    char state;
    uint8_t cellSize;
    uint8_t bgColor, wallColor, playerColor, goalColor;
};

struct player_s{
    uint8_t row, col;
    int8_t moveDir;
};

/**
 * @brief Fills a maze with walls.
 * 
 * @param maze Pointer to maze.
 */
void walled_maze(maze_t *maze);

/**
 * @brief Clears the visited bit in all cells of a maze.
 * 
 * @param maze Pointer to maze.
 */
void clear_visited(maze_t *maze);

/**
 * @brief Used for debugging. Prints maze to dbgout.
 * 
 * @param maze Pointer to maze structure.
 */
void print_maze(maze_t *maze);

/**
 * @brief Gets the number of directions to move from a cell ignoring the visited flag.
 * 
 * @param dir Array to store possible directions.
 * @param cell_row Current cell row.
 * @param cell_col Current cell column.
 * @param maxRow Maximum row.
 * @param maxCol Maximum column.
 * @return Number of possible movement directions.
 */
uint8_t getAdjacentNum_IGNORE_VISIT(uint8_t dir[], uint8_t cell_row, uint8_t cell_col, uint8_t maxRow, uint8_t maxCol);

/**
 * @brief For debugging. Gets the number of walls in the maze.
 * 
 * @param maze Pointer to maze structure.
 * @return Number of walls in the maze.
 */
int getWallCount(maze_t *maze);

/**
 * @brief For debugging. Prints the indexes of all unvisited cells and total unvisited cells.
 * 
 * @param maze Pointer to maze structure.
 */
void printUnvisitIndex(maze_t *maze);

#endif