#ifndef MORSE_CONVERTER_H
#define MORSE_CONVERTER_H

#include <Arduino.h>
#include "morse_code.h"

// Output mode configuration
enum class OutputMode {
    LED_ONLY,
    VIBRATION_ONLY,
    BOTH
};

#define LED_PIN 21  // Orange user LED 

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
    uint8_t vibrationPin;
    OutputMode outputMode;
    
    const char* findMorseCode(char c);

public:
    explicit MorseConverter(uint8_t vib_pin, OutputMode mode = OutputMode::LED_ONLY);
    uint8_t getVibrationPin() const;
    void setOutputMode(OutputMode mode);
    OutputMode getOutputMode() const;
    const char* textToMorse(const char* text);
    void playMorseCode(const char* morse);
    void playText(const char* text);
    
    // LED control
    void setLED(bool state);  // true = on, false = off (handles active LOW)
    
    // Status LED patterns
    void indicateIdle();        // Steady on
    void indicateProcessing();  // Fast blink (3 times)
    void indicatePlaying();     // Single blink
    void indicateError();       // Rapid blink (5 times)
    void clearStatus();         // Off
};

#endif // MORSE_CONVERTER_H 