// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Rachit Kakkar
// Last Modified: April 2026

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "ST7735.h"
#include "Clock.h"
#include "LaunchPad.h"
#include "TExaS.h"
#include "Timer.h"
#include "SlidePot.h"
#include "DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "Oscilloscope.h"
#include "images/images.h"
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern "C" void TIMG6_IRQHandler(void);

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

SlidePot Sensor(1742,99); // copy calibration from Lab 7
Oscilliscope scope;
Application app;
bool clear = false;

Cursor hCursor(20, 130, HORIZONTAL, 5); //temp values
Cursor hCursor2(100, 130, HORIZONTAL, 5); //temp values
Cursor vCursor(120, 20, VERTICAL, 5);
Cursor vCursor2(120, 100, VERTICAL, 5);
Cursor* cursors[] = {&hCursor, &hCursor2, &vCursor, &vCursor2};
int index = 0;
Cursor* selected = &hCursor;

// games  engine runs at 30Hz
uint32_t last=0,now;
bool toggle_vert_cursor = true; //BY DEFAULT, IT IS UP/DOWN
void TIMG12_IRQHandler(void) {
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOA->DOUTTGL31_0 =  BLUE; // toggle PB22 (minimally intrusive debugging)
    GPIOA->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    // game engine goes here
    // 1) sample slide pot
    uint32_t in = Sensor.In();

    if (in < 1365) {
      if (app.zoom_level != 1) clear = true;
      app.zoom_level = 1;
    } else if (in < 2730) {
      if (app.zoom_level != 2) clear = true;
      app.zoom_level = 2;
    } else {
      if (app.zoom_level != 4) clear = true;
      app.zoom_level = 4;
    }

    // 2) read input switches
    
    now = Switch_In();

    // if(now == 0x01){
    //   toggle_vert_cursor = !toggle_vert_cursor; //switch modes
    //   Sound_CursorToggle();
    //   while(Switch_In() == 0x01); //have to wait for release?
    // }

    if (now == 0x02 && (now != last)) { //pause/play
      scope.paused = !scope.paused;
      GPIOA->DOUTTGL31_0 = (1 << 28);
      Sound_Pause(); //sound
    }

    Cursor* sibling = nullptr;
    if(index == 0){sibling = cursors[1];} // hCursor (sibling is hCursor2)
    else if(index == 1){sibling = cursors[0];} // hCursor2 (sibling is hCursor)
    else if(index == 2){sibling = cursors[3];} // vCursor (sibling is vCursor2)
    else if(index == 3){sibling = cursors[2];} // vCursor2 (sibling is vCursor)

    if (now == 0x04 && (now != last)) { //left/down
      selected->Move(-1, app, sibling);
      Sound_CursorMove();
    }

    if(now == 0x01 && (now != last)) { //right/up
      selected->Move(1, app, sibling);
      Sound_CursorMove();
    }

    if(now == 0x08 && (now != last)){
      index++;
      index %= 4;
      clear = true;
      selected = cursors[index];
      Sound_CursorToggle();
    } 
    
    if(now == 0x09 && (now != last)) {
      clear = true;
      app.cursor_info = !app.cursor_info;
    }

    if (now == 0x0C && (now != last)) {
      clear = true;
      if (app.lang == Language::ENGLISH) {
        app.lang = Language::SPANISH;
      }
      else {
        app.lang = Language::ENGLISH;
      }
    }


    // 3) move sprites
    // 4) start sounds

    // 5) set semaphore
    last = now;
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOA->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
  }
}

void TIMG6_IRQHandler(void) {
  if((TIMG6->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOA->DOUTTGL31_0 = (1 << 27); // toggle PA27 (minimally intrusive debugging)
    GPIOA->DOUTTGL31_0 = (1 << 27); // toggle PA27 (minimally intrusive debugging)

    if (!scope.bufferFull) {
      uint32_t sample = scope.In();
      
      if (scope.sampleIdx == 0) {
        if (sample <= scope.triggerLevel) {
          return;
        }
        // if (sample > (scope.triggerLevel-1) || sample < (scope.triggerLevel - 20)) {
        //   return;
        // }
      }

      scope.Add_Sample(sample);
    }

    GPIOA->DOUTTGL31_0 = (1 << 27);// toggle PA27 (minimally intrusive debugging)
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)"Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  uint32_t last=0,now;
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetCursor(1, 1);
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs
    now = Switch_In(); // Your Lab5 input
    if(now != last){ // change
      ST7735_FillScreen(0x0000);
      ST7735_SetCursor(1, 1);
      ST7735_OutString((char*)"Switch= 0x"); ST7735_OutUHex2(now, ST7735_YELLOW); ST7735_OutString((char*)"\n\r");
    }
    last = now;
  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_BLACK);
  app.Draw_Grid();

  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  // Add line to initalize ADC0 for scope
  scope.Init();
  // Cursor hCursor1(20, 130, Type::HORIZONTAL, 5); //temp step_size
  // Cursor hCursor2(100, 130, Type::VERTICAL, 5);

  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26

  // initialize timer interrupts
  TimerG6_IntArm(8000000 / 50000, 0, 1); // Scope interrupt, 10kHz
  TimerG12_IntArm(8000000 / 30, 0); // Game engine interrupt, 30Hz

  // initialize all data structures
  __enable_irq();

  while(1){
    if (scope.bufferFull && !scope.paused) {
      TIMG6->CPU_INT.IMASK &= 0;
      // disable timer
      // __disable_irq();

      app.Draw_Graph(scope, (hCursor.x - hCursor2.x), (vCursor.y - vCursor2.y));

      scope.bufferFull = false;
      scope.sampleIdx = 0;

      // enable timer
      // __enable_irq();
      TIMG6->CPU_INT.IMASK |= 1;
    }

    // ST7735_SetCursor(0, 15);
    // ST7735_OutUDec(Sensor.In());

    hCursor.Draw();
    hCursor2.Draw();
    vCursor.Draw();
    vCursor2.Draw();

    
    if (clear) {
      ST7735_FillScreen(ST7735_BLACK);
      app.Draw_Grid();
      clear = false;
    }

    // wait for semaphore
       // clear semaphore
       // update ST7735R
    // check for end game or level switch
  }
}
