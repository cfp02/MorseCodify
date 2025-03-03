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

// Morse playback states
enum class PlaybackState {
    IDLE,
    SYMBOL_ON,
    SYMBOL_OFF,
    LETTER_SPACE
};

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
    
    // PWM configuration
    static const int pwmFreq = 5000;
    static const int pwmResolution = 8;
    static const int pwmChannel = 0;
    
    // Pin configuration
    uint8_t vibrationPin;
    OutputMode outputMode;
    uint8_t hapticIntensity = 128;  // Default 50% intensity
    
    // Playback state
    PlaybackState playbackState = PlaybackState::IDLE;
    const char* currentMorse = nullptr;
    int currentPosition = 0;
    unsigned long lastStateChange = 0;
    unsigned long currentDuration = 0;
    bool isPlaying = false;
    
    // Private methods
    const char* findMorseCode(char c);
    void setupPWM();
    void updateOutputs(bool state, uint8_t intensity);

public:
    explicit MorseConverter(uint8_t vib_pin, OutputMode mode = OutputMode::LED_ONLY);
    uint8_t getVibrationPin() const;
    void setOutputMode(OutputMode mode);
    OutputMode getOutputMode() const;
    const char* textToMorse(const char* text);
    void startPlayback(const char* morse);  // Non-blocking start
    void updatePlayback();  // Call this from main loop
    bool isPlaybackActive() const;
    void stopPlayback();
    
    // LED control
    void setLED(bool state);  // true = on, false = off (handles active LOW)
    void setPWM(uint8_t value);  // 0-255 for PWM control
    
    // Status LED patterns
    void indicateIdle();        // Steady on
    void indicateProcessing();  // Fast blink (3 times)
    void indicatePlaying();     // Single blink
    void indicateError();       // Rapid blink (5 times)
    void clearStatus();         // Off
};

#endif // MORSE_CONVERTER_H 