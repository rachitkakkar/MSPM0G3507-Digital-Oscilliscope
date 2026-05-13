#include "Oscilloscope.h"

Application::Application() {
    paused = 1;
    cnt = 0;
    cursor_info = false;
    zoom_level = 1;
    lang = Language::ENGLISH;
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

#define VREF_MV 3300
#define TOTAL_TIME_MS 10

void Application::Draw_Graph(const Oscilliscope& scope, int32_t deltaX, int32_t deltaY) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        int32_t point = MAP_Y(scope.prevSamples[i]);
        ST7735_DrawPixel(i*zoom_level, point, ST7735_BLACK);
        ST7735_DrawPixel(i*zoom_level, point+1, ST7735_BLACK);
        ST7735_DrawPixel(i*zoom_level+1, point, ST7735_BLACK);
        ST7735_DrawPixel(i*zoom_level+1, point+1, ST7735_BLACK);
        Fix_Grid(i*zoom_level, point);
        Fix_Grid(i*zoom_level, point+1);
        Fix_Grid(i*zoom_level+1, point);
        Fix_Grid(i*zoom_level+1, point+1);
    }

    // Draw_Grid();

    for (int i = 0; i < GRID_WIDTH; i++) {
        int32_t point = MAP_Y(scope.samples[i]);
        ST7735_DrawPixel(i*zoom_level, point, ST7735_YELLOW);
        ST7735_DrawPixel(i*zoom_level, point+1, ST7735_YELLOW);
        ST7735_DrawPixel(i*zoom_level+1, point, ST7735_YELLOW);
        ST7735_DrawPixel(i*zoom_level+1, point+1, ST7735_YELLOW);
    }

    uint32_t minV = 4095, maxV = 0;
    for (int i = 0; i < GRID_WIDTH; i++) {
        if (scope.samples[i] < minV) minV = scope.samples[i];
        if (scope.samples[i] > maxV) maxV = scope.samples[i];
    }
    
    if (!cursor_info) {
        uint32_t vpp_mv = ((maxV - minV) * VREF_MV) / 4095;

        uint32_t midpoint = (maxV + minV) / 2;
        // int32_t crossings = 0;
        // for (int i = 1; i < GRID_WIDTH; i++) {
        //     if (scope.samples[i-1] < midpoint && scope.samples[i] >= midpoint) {
        //         crossings++;
        //     }
        // }

        ST7735_SetCursor(0, 14);
        if (lang == Language::ENGLISH) ST7735_OutString((char*)"Volatage_pp: ");
        else ST7735_OutString((char*)"Voltaje_pp: ");
        ST7735_OutUDec(vpp_mv / 1000);
        ST7735_OutChar('.');
        
        uint32_t decimals = (vpp_mv % 1000) / 10;
        if (decimals < 10) ST7735_OutChar('0'); // Leading zero padding
        ST7735_OutUDec(decimals);
        ST7735_OutString((char*)" V");

        // int x = 0; // skip overflow
        // while (scope.times[x] <= scope.times[x+1]) {
        //     x++;
        // }
        // uint32_t elapsedCycles = (scope.times[x] - scope.times[x+1]);
        // uint32_t elapsedCycles = 12544000;

        // if (elapsedCycles > 0 && crossings > 0) {
        //     uint64_t numerator = (uint64_t)crossings * 80000000;
        //     uint32_t freq_hz = (uint32_t)(numerator / elapsedCycles);

        //     ST7735_SetCursor(0, 15);
        //     ST7735_OutString((char*)"Freq: ");
        //     ST7735_OutUDec(freq_hz);
        //     ST7735_OutString((char*)" Hz    ");
        // }

        uint32_t first_idx = 0;
        uint32_t last_idx = 0;
        uint32_t crossings = 0;
        bool found_first = false;

        for (int i = 1; i < GRID_WIDTH; i++) {
            // Detect Rising Edge crossing the midpoint
            if (scope.samples[i-1] < midpoint && scope.samples[i] >= midpoint) {
                if (!found_first) {
                    first_idx = i;
                    found_first = true;
                }
                last_idx = i;
                crossings++;
            }
        }

        ST7735_SetCursor(0, 15);
        if (lang == Language::ENGLISH) ST7735_OutString((char*)"Frequency: ");
        else ST7735_OutString((char*)"Frecuencia: ");

        if (crossings >= 2) {
            uint32_t actual_periods = crossings - 1;
            uint32_t samples_elapsed = last_idx - first_idx;

            // Total cycles elapsed between the first and last edge
            uint64_t total_cycles = (uint64_t)samples_elapsed * 40945;

            if (total_cycles > 0) {
                uint64_t numerator = (uint64_t)actual_periods * 80000000;
                uint32_t freq_hz = (uint32_t)(numerator / total_cycles);

                ST7735_OutUDec(freq_hz);
                ST7735_OutString((char*)" Hz");
            }
        } else {
            ST7735_OutString((char*)"--- Hz");
        }
    }
    else {
        if (deltaX < 0) deltaX = -deltaX;
        if (deltaY < 0) deltaY = -deltaY;


        deltaX = (deltaX * 40945) / 12;
        ST7735_SetCursor(0, 14);
        if (lang == Language::ENGLISH) ST7735_OutString((char*)"Delta X:  ~");
        else ST7735_OutString((char*)"Cambio de X:  ");
        ST7735_OutUDec((deltaX) / 1000);
        ST7735_OutChar('.');
        
        uint32_t decimals = (deltaX % 1000) / 10;
        if (decimals < 10) ST7735_OutChar('0'); // Leading zero padding
        ST7735_OutUDec(decimals);
        ST7735_OutString((char*)" ms");

        deltaY =(deltaY * VREF_MV) / 127; // Map to 3.3 V (in mV)
        ST7735_SetCursor(0, 15);
        if (lang == Language::ENGLISH) ST7735_OutString((char*)"Delta Y:  ");
        else ST7735_OutString((char*)"Cambio de Y:  ");
        ST7735_OutUDec((deltaY) / 1000);
        ST7735_OutChar('.');
        
        decimals = (deltaY % 1000) / 10;
        if (decimals < 10) ST7735_OutChar('0'); // Leading zero padding
        ST7735_OutUDec(decimals);
        ST7735_OutString((char*)" V");
    }
}

Cursor::Cursor(int x1, int y1, Type t, int step_size) : x(x1), y(y1), type(t), step_size(step_size) {} 

void Cursor::Draw() {
    if (type == Type::HORIZONTAL) {
        ST7735_DrawBitmap(x, y, PlayerShip0, 18, 8);
    }
    else if (type == Type::VERTICAL){
        ST7735_DrawBitmap(x, y, PlayerShip0_Updated, 7, 14);
    }
}

void Cursor::Erase(Application& app) {
    TIMG6->CPU_INT.IMASK &= 0;

    int w;
    int h;

    //set bitmap dims
    if (type == Type::HORIZONTAL){
        w = 18;
        h = 8;
    } 
    else {
        w = 7;
        h = 14;
    }

    // ST7735_FillRect(x, y-8, 18, 8, ST7735_BLACK);

    //restore black background
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            app.Fix_Grid(x + i, y - j);
        }
    }

    TIMG6->CPU_INT.IMASK |= 1;
}

void Cursor::Move(int direction, Application& app, Cursor* sibling){
    int next_x = x;
    int next_y = y;

    if (type == Type::HORIZONTAL) {
        next_x += (direction * step_size);
        if (next_x <= 0) {
            next_x = GRID_WIDTH;
        }else{
        next_x %= GRID_WIDTH;}
    } else {
        next_y += (direction * step_size);
        if (next_y <= 0) {
            next_y = GRID_HEIGHT;
        }else{
        next_y %= GRID_HEIGHT;}
    }

    //overlap check
    bool overlap = false;
    int w;
    int h;
    if(sibling != nullptr){

    if (type == Type::HORIZONTAL) {
        w = 18;
        h = 8;
    } 
    else {
        w = 7;
        h = 14;
    }

    int l1 = next_x;
    int r1 = next_x + w;
    int t1 = next_y - h;
    int b1 = next_y;

    int l2 = sibling->x;
    int r2 = sibling->x + w; 
    int t2 = sibling->y - h;
    int b2 = sibling->y;

    if (l1 < r2 && r1 > l2 && t1 < b2 && b1 > t2) {
        overlap = true;
    }

    if(overlap == false){
        Erase(app);
        x = next_x;
        y = next_y;
    }

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
    times[sampleIdx] = TIMG12->COUNTERREGS.CTR;
    sampleIdx++;

    if (sampleIdx >= GRID_WIDTH) {
        bufferFull = true;
        sampleIdx = 0;
    }
}