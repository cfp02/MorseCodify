#pragma once

#include <Arduino.h>
#include "morse_code.h"

// Output mode configuration
enum class OutputMode {
    LED_ONLY,
    VIBRATION_ONLY,
    BOTH
};

// XIAO BLE Sense built-in RGB LED
#define PIN_LED1 P0_06    // BLUE
#define PIN_LED2 P0_24    // RED
#define PIN_LED3 P0_16    // GREEN

class MorseConverter {
private:
    // Buffer for storing morse code strings
    char morseBuffer[256];
    
    // Timing constants (in milliseconds)
    static const int DOT_DURATION = 100;
    static const int DASH_DURATION = DOT_DURATION * 3;
    static const int SYMBOL_SPACE = DOT_DURATION;
    static const int LETTER_SPACE = DOT_DURATION * 3;
    static const int WORD_SPACE = DOT_DURATION * 7;
    
    // Pin configuration
    pin_size_t vibrationPin;
    OutputMode outputMode;
    
    const char* findMorseCode(char c);

public:
    explicit MorseConverter(pin_size_t vib_pin, OutputMode mode = OutputMode::LED_ONLY);
    pin_size_t getVibrationPin() const;
    void setOutputMode(OutputMode mode);
    OutputMode getOutputMode() const;
    const char* textToMorse(const char* text);
    void playMorseCode(const char* morse);
    void playText(const char* text);
    
    // Simple RGB LED control (true = LED on, false = LED off)
    void setRGB(bool r, bool g, bool b);
    
    // Status LED control
    void indicateIdle();        // Blue
    void indicateProcessing();  // Yellow (Red + Green)
    void indicatePlaying();     // Green
    void indicateError();       // Red
    void clearStatus();         // All off
}; 