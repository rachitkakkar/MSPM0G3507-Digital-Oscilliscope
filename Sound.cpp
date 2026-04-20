// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"



void SysTick_IntArm(uint32_t period, uint32_t priority){
  // write this
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
// write this
  DAC5_Init(); //init 12-bit DAC

  int period = 7273; //80 MHZ / 11000Hz
  int priority = 2;

  // copied from textbooks
  SysTick->CTRL = 0x00;      // disable SysTick during setup
  SysTick->LOAD = period-1;  // reload value
  SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000))|(priority<<30); // priority 2
  SysTick->VAL = 0;          // any write to VAL clears COUNT and sets VAL equal to LOAD
  SysTick->CTRL = 0x07;      // enable SysTick with 80 MHz bus clock and interrupts
}

const uint8_t *SoundPtr;
uint32_t SoundCount = 0;


extern "C" void SysTick_Handler(void);
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    // output one value to DAC if a sound is active
    if(SoundCount > 0){
      
      DAC5_Out(*SoundPtr);
      SoundPtr++; //next value in array
      SoundCount--; //decrement counter
    }
    

}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
  __disable_irq();
  SoundPtr = pt;
  SoundCount = count;
  __enable_irq();
  
}

void Sound_Pause(void){
  if(SoundCount == 0){ //no sound overlaps (maybe change)
  Sound_Start(pause_sound, 3128); //delivered to mailbox
  }

}

void Sound_CursorMove(void){
  if(SoundCount == 0){
  Sound_Start(move_cursor_sound, 2511);
  }
}

void Sound_CursorToggle(void){
  if(SoundCount == 0){
    Sound_Start(toggle_cursor_sound, 4002);
  }
}





void Sound_Shoot(void){
// write this
  Sound_Start( shoot, 4080);
}
void Sound_Killed(void){
// write this

}
void Sound_Explosion(void){
// write this

}

void Sound_Fastinvader1(void){

}
void Sound_Fastinvader2(void){

}
void Sound_Fastinvader3(void){

}
void Sound_Fastinvader4(void){

}
void Sound_Highpitch(void){

}
