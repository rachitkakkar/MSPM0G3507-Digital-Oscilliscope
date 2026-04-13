#ifndef _SCOPEH_
#define _SCOPEH_

#include "../inc/ST7735.h"
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"

#define MAJOR_GRID_LINE ST7735_LIGHTGREY
#define MINOR_GRID_LINE ST7735_DARKGREY
#define MAP_Y(x) (127 - ((x) * 127 / 4095))
#define GRID_WIDTH 128
#define GRID_HEIGHT 128

// --- OSCILLISCOPE --
struct Oscilliscope {
    volatile int samples[GRID_WIDTH];
    volatile int sampleIdx;
    volatile bool bufferFull;
    const int triggerLevel = 2048;

    Oscilliscope();
    void Init();
    uint32_t In();
    void Add_Sample(uint32_t sample);
};

// --- APPLICATION ---
enum Language {
    ENGLISH,
    HINDI
};

struct Application {
    int paused;
    Language lang;

    Application();
    void Init_Startup();
    void Draw_Grid();
    void Draw_Graph(const Oscilliscope& scope);
};

#endif