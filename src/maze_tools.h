#ifndef MAZE_TOOLS_H
#define MAZE_TOOLS_H

#define MAZE_X_MIN 10
#define MAZE_X_MAX 320 - MAZE_X_MIN
#define MAZE_Y_MIN 10
#define MAZE_Y_MAX 240 - MAZE_Y_MIN

#define MAX_CELL_COLS (int) ((MAZE_X_MAX - MAZE_X_MIN) / 4)
#define MAX_CELL_ROWS (int) ((MAZE_Y_MAX - MAZE_Y_MIN) / 4)

enum{North, East, South, West, Ndirs};

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
    int finishrow, finishcol;
    int rows, cols;
}maze_t;

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
int getAdjacentNum_IGNORE_VISIT(int dir[], int cell_row, int cell_col, int maxRow, int maxCol);

#endif