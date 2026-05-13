/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA18INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081; // input, no pull
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
  uint32_t data = GPIOB->DIN31_0;
  uint32_t state = 0;

  if (data & (1<<16)) state |= 0x1; //move right/down
  
  data = GPIOA->DIN31_0;
  if (data & (1<<16)) state |= 0x2; //toggle pause/play
  if (data & (1<<17)) state |= 0x4; //
  if (data & (1<<18)) state |= 0x8; //move left/up

  return state; // return 0; //replace this your code
}
