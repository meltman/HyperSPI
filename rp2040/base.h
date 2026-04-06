#pragma once
#include "leds.h"

// These should match your CMakeLists.txt definitions
#ifndef DATA_PIN
    #define DATA_PIN 14 // Fallback default
#endif

#define STRIP_COUNT 4
#define LEDS_PER_STRIP 180 

class Base {
public:
    // Pointers for the 4 parallel strings
    LED_DRIVER *lane1, *lane2, *lane3, *lane4;
    int totalLeds;
    int queueEnd;
    uint8_t buffer[MAX_BUFFER];

    Base() : lane1(nullptr), lane2(nullptr), lane3(nullptr), lane4(nullptr), 
             totalLeds(0), queueEnd(0) {}

    void initLedStrip(int count) {
        totalLeds = count;
        
        // The NeopixelParallel driver automatically assigns itself 
        // to the next GPIO pin in sequence (DATA_PIN, DATA_PIN+1, etc.)
        lane1 = new LED_DRIVER(LEDS_PER_STRIP, DATA_PIN);
        lane2 = new LED_DRIVER(LEDS_PER_STRIP, DATA_PIN);
        lane3 = new LED_DRIVER(LEDS_PER_STRIP, DATA_PIN);
        lane4 = new LED_DRIVER(LEDS_PER_STRIP, DATA_PIN);
    }

    bool setStripPixel(int index, ColorRgb color) {
        // Route data based on absolute index from the SPI stream
        if (index < 180)
            lane1->SetPixel(index, color);
        else if (index < 360)
            lane2->SetPixel(index - 180, color);
        else if (index < 540)
            lane3->SetPixel(index - 360, color);
        else if (index < 720)
            lane4->SetPixel(index - 540, color);

        return (index < totalLeds - 1);
    }

    void renderLeds(bool force) {
        // In the parallel driver, one call triggers the DMA for ALL lanes
        if (lane1) lane1->renderAllLanes();
    }
    
    int getLedsNumber() { return totalLeds; }
};

// Global instance used by main.h and hyperspi.cpp
extern Base base;
