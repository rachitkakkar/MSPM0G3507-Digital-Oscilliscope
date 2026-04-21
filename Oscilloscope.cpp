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
    if (x > GRID_WIDTH-1 || y > GRID_HEIGHT-1) { 
        ST7735_DrawPixel(x, y, ST7735_BLACK);
        return;
    }

    if (x % 32 == 0 || x == GRID_WIDTH-1 || y % 32 == 0 || y == GRID_HEIGHT-1) {
        ST7735_DrawPixel(x, y, MAJOR_GRID_LINE);   // major line
    } 
    
    else if (x % 8 == 0 || y % 8 == 0) {
        ST7735_DrawPixel(x, y, MINOR_GRID_LINE);   // minor line
    }

    else{
        ST7735_DrawPixel(x, y, ST7735_BLACK); //else just do normal black
    }
}

#define VREF_MV 3260
#define TOTAL_TIME_MS 10

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

    uint32_t minV = 4095, maxV = 0;
    int32_t crossings = 0;
    
    for (int i = 0; i < GRID_WIDTH; i++) {
        if (scope.samples[i] < minV) minV = scope.samples[i];
        if (scope.samples[i] > maxV) maxV = scope.samples[i];
    }
    
    uint32_t vpp_mv = ((maxV - minV) * VREF_MV) / 4095;

    uint32_t midpoint = (maxV + minV) / 2;
    for (int i = 1; i < GRID_WIDTH; i++) {
        if (scope.samples[i-1] < midpoint && scope.samples[i] >= midpoint) {
            crossings++;
        }
    }

    uint32_t freq_hz = (crossings * 1000) / TOTAL_TIME_MS;

    ST7735_SetCursor(0, 14);
    ST7735_OutString((char*)"Vpp:  ");
    ST7735_OutUDec(vpp_mv / 1000); // Whole Volts
    ST7735_OutChar('.');
    ST7735_OutUDec((vpp_mv % 1000) / 10); // Two digits of mV
    ST7735_OutString((char*)" V  ");

    ST7735_SetCursor(0, 15);
    ST7735_OutString((char*)"Freq: ");
    ST7735_OutUDec(freq_hz);
    ST7735_OutString((char*)" Hz    ");
}

Cursor::Cursor(int x1, int y1, Type t, int step_size) : x(x1), y(y1), type(t), step_size(step_size) {} 

void Cursor::Draw() {
    if (type == Type::HORIZONTAL) {
        ST7735_DrawBitmap(x, y, PlayerShip0, 18, 8);
    }
    else if (type == Type::VERTICAL){
        ST7735_DrawBitmap(x, y, vertical_orange_cursor, 18, 8);
    }
}

void Cursor::Erase(Application& app) {
    int w;
    int h;

    //set bitmap dims
    if (type == Type::HORIZONTAL){
        w = 18;
        h = 8;
    } 
    else {
        w = 8;
        h = 18;
    }

    // ST7735_FillRect(x, y-8, 18, 8, ST7735_BLACK);

    //restore black background
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            app.Fix_Grid(x + i, y - j);
        }
    }

}

void Cursor::Move(int direction, Application& app){
    Erase(app);
    if (type == Type::HORIZONTAL) {
        x += (direction * step_size);
        x %= GRID_WIDTH;
    } else {
        y += (direction * step_size) % GRID_HEIGHT;
        y %= GRID_HEIGHT;
    }
    Erase(app);

    // // defining bounds
    // int width;
    // int height;

    // if (type == Type::HORIZONTAL) {
    //     width = 18;
    //     height = 8;
    // } 
    // else {
    //     // Vertical layout: taller than wide
    //     width = 8;
    //     height = 18;
    // }

    // //boundary checks
    // if(x < 0){x = 0;}
    // if (x > 127 - width) {x = 127 - width;}

    // if(y < 0){y = 0;}
    // if(y > 127 - height){y = 127 - height;}
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