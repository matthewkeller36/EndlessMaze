#ifndef INPUT_H
#define INPUT_H

#include <keypadc.h>
#include "maze_tools.h"

/**
 * @brief Get the move direction using repeated keypresses.
 * 
 * @return Enumerated direction if a key is pressed, -1 otherwise.
 */
enum keyinput getMoveDir();

#endif