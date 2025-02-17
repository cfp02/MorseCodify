#include "morse_converter.h"

const char* MorseConverter::findMorseCode(char c) {
    char upperChar = toupper(c);
    
    for (int i = 0; i < MORSE_TABLE_SIZE; i++) {
        if (MORSE_TABLE[i].character == upperChar) {
            return MORSE_TABLE[i].code;
        }
    }
    return "";
}

MorseConverter::MorseConverter(pin_size_t vib_pin, OutputMode mode) 
    : vibrationPin(vib_pin), outputMode(mode) {
    // Initialize vibration pin if needed
    if (mode != OutputMode::LED_ONLY) {
        pinMode(vibrationPin, OUTPUT);
        analogWrite(vibrationPin, 0);
    }
    
    // Initialize LED pins
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    
    // Turn all LEDs off initially
    setRGB(false, false, false);
    
    // Startup sequence
    setRGB(false, false, true);  // Blue
    delay(300);
    setRGB(true, false, false);  // Red
    delay(300);
    setRGB(false, true, false);  // Green
    delay(300);
    setRGB(false, false, false); // All off
}

void MorseConverter::setRGB(bool r, bool g, bool b) {
    digitalWrite(LEDR, !r);  // Active LOW
    digitalWrite(LEDG, !g);
    digitalWrite(LEDB, !b);
}

pin_size_t MorseConverter::getVibrationPin() const {
    return vibrationPin;
}

void MorseConverter::setOutputMode(OutputMode mode) {
    outputMode = mode;
}

OutputMode MorseConverter::getOutputMode() const {
    return outputMode;
}

const char* MorseConverter::textToMorse(const char* text) {
    morseBuffer[0] = '\0';
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (i > 0) {
            strcat(morseBuffer, " ");
        }
        
        const char* morseChar = findMorseCode(text[i]);
        strcat(morseBuffer, morseChar);
    }
    
    return morseBuffer;
}

void MorseConverter::playMorseCode(const char* morse) {
    for (int i = 0; morse[i] != '\0'; i++) {
        switch (morse[i]) {
            case '.':
                if (outputMode != OutputMode::LED_ONLY) {
                    analogWrite(vibrationPin, 255);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setRGB(false, false, true);  // Blue on for dot
                }
                delay(DOT_DURATION);
                if (outputMode != OutputMode::LED_ONLY) {
                    analogWrite(vibrationPin, 0);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setRGB(false, false, false); // All off
                }
                delay(SYMBOL_SPACE);
                break;
                
            case '-':
                if (outputMode != OutputMode::LED_ONLY) {
                    analogWrite(vibrationPin, 255);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setRGB(false, false, true);  // Blue on for dash
                }
                delay(DASH_DURATION);
                if (outputMode != OutputMode::LED_ONLY) {
                    analogWrite(vibrationPin, 0);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setRGB(false, false, false); // All off
                }
                delay(SYMBOL_SPACE);
                break;
                
            case ' ':
                delay(LETTER_SPACE);
                break;
                
            default:
                break;
        }
    }
}

void MorseConverter::playText(const char* text) {
    const char* morse = textToMorse(text);
    playMorseCode(morse);
}

void MorseConverter::indicateIdle() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setRGB(false, false, true);  // Blue
    }
}

void MorseConverter::indicateProcessing() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setRGB(true, true, false);  // Yellow (Red + Green)
    }
}

void MorseConverter::indicatePlaying() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setRGB(false, true, false);  // Green
    }
}

void MorseConverter::indicateError() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setRGB(true, false, false);  // Red
    }
}

void MorseConverter::clearStatus() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setRGB(false, false, false);  // All off
    }
} 