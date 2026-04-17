#include "Oscilloscope.h"

Application::Application() {
    paused = 1;
    lang = ENGLISH;
}

void Application::Draw_Grid() {
    int width = 128;
    int height = 128;
    // ST7735_FillScreen(ST7735_BLACK);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        if (y % 8 == 0) {
            ST7735_DrawFastHLine(0, y, GRID_WIDTH, MINOR_GRID_LINE);
        }
    }
    for (int x = 0; x < GRID_WIDTH; x++) {
        if (x % 32 == 0 || x == GRID_WIDTH-1) {
            ST7735_DrawFastVLine(x, 0, GRID_HEIGHT, MAJOR_GRID_LINE);   // major line
        } else if (x % 8 == 0) {
            ST7735_DrawFastVLine(x, 0, GRID_HEIGHT, MINOR_GRID_LINE);   // minor line
        }
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        if (y % 32 == 0 || y == GRID_HEIGHT-1) {
            ST7735_DrawFastHLine(0, y, GRID_WIDTH, MAJOR_GRID_LINE);
        }
    }
}

void Application::Fix_Grid(uint32_t x, uint32_t y) {
    if (y % 8 == 0) {
        ST7735_DrawPixel(x, y, MINOR_GRID_LINE);
    }
    if (x % 32 == 0 || x == GRID_WIDTH-1) {
        ST7735_DrawPixel(x, y, MAJOR_GRID_LINE);   // major line
    } else if (x % 8 == 0) {
        ST7735_DrawPixel(x, y, MINOR_GRID_LINE);   // minor line
    }
    if (y % 32 == 0 || y == GRID_HEIGHT-1) {
        ST7735_DrawPixel(x, y, MAJOR_GRID_LINE);
    }
}

void Application::Draw_Graph(const Oscilliscope& scope) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        int32_t point = MAP_Y(scope.prevSamples[i]);
        ST7735_DrawPixel(i, point, ST7735_BLACK);
        ST7735_DrawPixel(i, point+1, ST7735_BLACK);
        ST7735_DrawPixel(i+1, point, ST7735_BLACK);
        ST7735_DrawPixel(i+1, point+1, ST7735_BLACK);
        Fix_Grid(i, point);
        Fix_Grid(i, point+1);
        Fix_Grid(i+1, point);
        Fix_Grid(i+1, point+1);
    }

    // Draw_Grid();

    for (int i = 0; i < GRID_WIDTH; i++) {
        int32_t point = MAP_Y(scope.samples[i]);
        ST7735_DrawPixel(i, point, ST7735_YELLOW);
        ST7735_DrawPixel(i, point+1, ST7735_YELLOW);
        ST7735_DrawPixel(i+1, point, ST7735_YELLOW);
        ST7735_DrawPixel(i+1, point+1, ST7735_YELLOW);
    }
}

Cursor::Cursor(int x1, int y1, Type t) : x(x1), y(y1), type(t) {} 

void Cursor::Draw() {
    if (type == Type::HORIZONTAL) {
        ST7735_DrawBitmap(x, y, PlayerShip0, 18, 8);
    }
}


Oscilliscope::Oscilliscope() {
    sampleIdx = 0;
    bufferFull = false;
    paused = false;
}

void Oscilliscope::Init() {
    ADC0->ULLMEM.GPRCM.RSTCTL = 0xB1000003; // 1) reset
    ADC0->ULLMEM.GPRCM.PWREN = 0x26000001;  // 2) activate
    Clock_Delay(24);                        // 3) wait
    ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // 4) ULPCLK
    ADC0->ULLMEM.CLKFREQ = 7;               // 5) 40-48 MHz
    ADC0->ULLMEM.CTL0 = 0x03010000;         // 6) divide by 8
    ADC0->ULLMEM.CTL1 = 0x00000000;         // 7) mode
    ADC0->ULLMEM.CTL2 = 0x00000000;         // 8) MEMRES
    ADC0->ULLMEM.MEMCTL[0] = 3;             // 9) channel 3 is PA24
    ADC0->ULLMEM.SCOMP0 = 0;                // 10) 8 sample clocks
    ADC0->ULLMEM.CPU_INT.IMASK = 0;         // 11) no interrupt
}

uint32_t Oscilliscope::In() {
    ADC0->ULLMEM.CTL0 |= 0x00000001;             // 1) enable conversions
    ADC0->ULLMEM.CTL1 |= 0x00000100;             // 2) start ADC
    uint32_t volatile delay=ADC0->ULLMEM.STATUS; // 3) time to let ADC start
    while((ADC0->ULLMEM.STATUS&0x01)==0x01){}    // 4) wait for completion
    return ADC0->ULLMEM.MEMRES[0];               // 5) 12-bit result
}

void Oscilliscope::Add_Sample(uint32_t sample) {
    prevSamples[sampleIdx] = samples[sampleIdx];
    samples[sampleIdx] = sample;
    sampleIdx++;

    if (sampleIdx >= GRID_WIDTH) {
        bufferFull = true;
        sampleIdx = 0;
    }
}