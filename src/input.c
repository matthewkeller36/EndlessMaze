#include <tice.h>
#include <keypadc.h>
#include <debug.h>
#include "input.h"

#define REPEAT_PER_SEC 12

// enum{None = -1, North, East, South, West, Clear};

/**
 * @brief Resets timer 1 to 32K/REPEAT_PER_SEC clock cycles
 * 
 */
void resetTimer(){
    timer_Disable(3);
    timer_SetReload(3, 32768 / REPEAT_PER_SEC);
    timer_Set(3, 32768 / REPEAT_PER_SEC);
}

enum keyinput getMoveDir(){
    enum keyinput retVal = key_None;
    kb_Scan();
    if(kb_Data[6] & kb_Clear){
        return key_Clear;
    }

    if(kb_Data[7]){
        if(timer_Get(3) == timer_GetReload(3) || timer_ChkInterrupt(3, TIMER_RELOADED)){
            dbg_sprintf(dbgout, "Input detected\n");
            if(timer_ChkInterrupt(3, TIMER_RELOADED)){
                timer_AckInterrupt(3, TIMER_RELOADED);
            }
            if(kb_Data[7] & kb_Up){
                retVal = key_North;
            }
            if(kb_Data[7] & kb_Right){
                retVal = key_East;
            }
            if(kb_Data[7] & kb_Down){
                retVal = key_South;
            }
            if(kb_Data[7] & kb_Left){
                retVal = key_West;
            }
        }
        timer_Enable(3, TIMER_32K, TIMER_0INT, TIMER_DOWN);

    }else{
        timer_AckInterrupt(3, TIMER_RELOADED);
        resetTimer();
    }
    return retVal;
}

