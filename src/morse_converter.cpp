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

void MorseConverter::setLED(bool state) {
    digitalWrite(LED_PIN, !state);  // Active LOW
}

MorseConverter::MorseConverter(uint8_t vib_pin, OutputMode mode) 
    : vibrationPin(vib_pin), outputMode(mode) {
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // Active LOW, so HIGH = OFF
    
    // Startup sequence - three quick blinks
    for (int i = 0; i < 3; i++) {
        setLED(true);
        delay(100);
        setLED(false);
        delay(100);
    }

    if (true) {  // Toggle this to false to disable PWM test
        // Test all pins with PWM
        Serial.println("Testing all pins with PWM...");
        
        // ESP32S3 PWM setup
        const int freq = 5000;
        const int resolution = 8;  // 8-bit resolution (0-255)
        const int pwmChannel = 0;  // Use channel 0 for testing
        
        // Configure timer first
        ledcSetup(pwmChannel, freq, resolution);
        
        // Test each pin one at a time
        for (int pin = 0; pin <= 21; pin++) {
            Serial.printf("Testing pin %d with PWM\n", pin);
            
            // Configure channel by attaching pin
            ledcAttachPin(pin, pwmChannel);
            
            // Set PWM to 80 (about 31% duty cycle)
            ledcWrite(pwmChannel, 90);
            delay(1000);

            // ledcWrite(pwmChannel, 0);
            // delay(1000);
            
        }
    }
    
    // Set up vibration pin for normal operation
    // pinMode(vibrationPin, OUTPUT);
    // digitalWrite(vibrationPin, LOW);
}

uint8_t MorseConverter::getVibrationPin() const {
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
            case '.': {
                if (outputMode != OutputMode::LED_ONLY) {
                    digitalWrite(vibrationPin, HIGH);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setLED(true);
                }
                delay(DOT_DURATION);
                if (outputMode != OutputMode::LED_ONLY) {
                    digitalWrite(vibrationPin, LOW);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setLED(false);
                }
                delay(SYMBOL_SPACE);
                break;
            }
            case '-': {
                if (outputMode != OutputMode::LED_ONLY) {
                    digitalWrite(vibrationPin, HIGH);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setLED(true);
                }
                delay(DASH_DURATION);
                if (outputMode != OutputMode::LED_ONLY) {
                    digitalWrite(vibrationPin, LOW);
                }
                if (outputMode != OutputMode::VIBRATION_ONLY) {
                    setLED(false);
                }
                delay(SYMBOL_SPACE);
                break;
            }
            case ' ': {
                delay(LETTER_SPACE);
                break;
            }
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
        setLED(true);  // Steady on
    }
}

void MorseConverter::indicateProcessing() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        // Fast blink pattern
        for (int i = 0; i < 3; i++) {
            setLED(true);
            delay(50);
            setLED(false);
            delay(50);
        }
    }
}

void MorseConverter::indicatePlaying() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        // Single blink
        setLED(true);
        delay(200);
        setLED(false);
    }
}

void MorseConverter::indicateError() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        // Rapid blink pattern
        for (int i = 0; i < 5; i++) {
            setLED(true);
            delay(30);
            setLED(false);
            delay(30);
        }
    }
}

void MorseConverter::clearStatus() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setLED(false);  // Off
    }
} 