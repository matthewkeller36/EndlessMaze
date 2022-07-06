#ifndef INPUT_H
#define INPUT_H

#include <keypadc.h>

/**
 * @brief Get the move direction using repeated keypresses.
 * 
 * @return Enumerated direction if a key is pressed, -1 otherwise.
 */
int8_t getMoveDir();

#endif