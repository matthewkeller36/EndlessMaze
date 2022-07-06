#include <tice.h>
#include <keypadc.h>
#include <debug.h>
#include "input.h"

#define REPEAT_PER_SEC 5

/**
 * @brief Resets timer 1 to 32K/REPEAT_PER_SEC clock cycles
 * 
 */
void resetTimer(){
    timer_Disable(1);
    timer_SetReload(1, 32768 / REPEAT_PER_SEC);
    timer_Set(1, 32768 / REPEAT_PER_SEC);
}

enum{North, East, South, West};

int8_t getMoveDir(){
    int8_t retVal = -1;
    kb_Scan();

    if(kb_Data[7]){
        if(timer_1_Counter == timer_1_ReloadValue || (timer_IntStatus & TIMER1_RELOADED)){
            if(timer_1_Counter == timer_1_ReloadValue){
                dbg_sprintf(dbgout, "Reload value\n");
            }
            if(timer_IntStatus & TIMER1_RELOADED){
                timer_IntAcknowledge = TIMER1_RELOADED;
                dbg_sprintf(dbgout, "Int reloaded\n");
            }
            if(kb_Data[7] & kb_Up){
                retVal = North;
            }
            if(kb_Data[7] & kb_Right){
                retVal = East;
            }
            if(kb_Data[7] & kb_Down){
                retVal = South;
            }
            if(kb_Data[7] & kb_Left){
                retVal = West;
            }
        }
        timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_0INT | TIMER1_DOWN;

    }else{
        timer_IntAcknowledge = TIMER1_RELOADED;
        resetTimer();
    }
    return retVal;
}

