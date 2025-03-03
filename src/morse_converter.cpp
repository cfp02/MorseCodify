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

void MorseConverter::setupPWM() {
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(vibrationPin, pwmChannel);
    ledcWrite(pwmChannel, 0);  // Initialize PWM to 0
}

void MorseConverter::updateOutputs(bool state, uint8_t intensity) {
    if (outputMode != OutputMode::LED_ONLY) {
        ledcWrite(pwmChannel, state ? intensity : 0);
    }
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setLED(state);
    }
}

void MorseConverter::setPWM(uint8_t value) {
    hapticIntensity = value;
    ledcWrite(pwmChannel, value);
}

MorseConverter::MorseConverter(uint8_t vib_pin, OutputMode mode) 
    : vibrationPin(vib_pin), outputMode(mode) {
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // Active LOW, so HIGH = OFF
    
    // Set up PWM for vibration
    setupPWM();
    
    // Startup sequence - three quick blinks
    for (int i = 0; i < 3; i++) {
        setLED(true);
        delay(100);
        setLED(false);
        delay(100);
    }

    if (false) {  // Toggle this to false to disable PWM test
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

void MorseConverter::startPlayback(const char* morse) {
    currentMorse = morse;
    currentPosition = 0;
    isPlaying = true;
    playbackState = PlaybackState::SYMBOL_ON;
    lastStateChange = millis();
    
    // Calculate initial duration based on first symbol
    if (morse && morse[0] != '\0') {
        currentDuration = (morse[0] == '.') ? DOT_DURATION : DASH_DURATION;
        updateOutputs(true, hapticIntensity);
    }
}

void MorseConverter::updatePlayback() {
    if (!isPlaying || !currentMorse) return;
    
    unsigned long now = millis();
    if (now - lastStateChange < currentDuration) return;
    
    switch (playbackState) {
        case PlaybackState::SYMBOL_ON:
            updateOutputs(false, hapticIntensity);
            playbackState = PlaybackState::SYMBOL_OFF;
            currentDuration = SYMBOL_SPACE;
            break;
            
        case PlaybackState::SYMBOL_OFF:
        case PlaybackState::LETTER_SPACE:
            currentPosition++;
            if (currentMorse[currentPosition] == '\0') {
                stopPlayback();
                return;
            }
            
            if (currentMorse[currentPosition] == ' ') {
                playbackState = PlaybackState::LETTER_SPACE;
                currentDuration = LETTER_SPACE;
            } else {
                playbackState = PlaybackState::SYMBOL_ON;
                currentDuration = (currentMorse[currentPosition] == '.') ? DOT_DURATION : DASH_DURATION;
                updateOutputs(true, hapticIntensity);
            }
            break;
            
        default:
            break;
    }
    
    lastStateChange = now;
}

void MorseConverter::stopPlayback() {
    isPlaying = false;
    currentMorse = nullptr;
    currentPosition = 0;
    playbackState = PlaybackState::IDLE;
    updateOutputs(false, hapticIntensity);
}

bool MorseConverter::isPlaybackActive() const {
    return isPlaying;
}

void MorseConverter::indicateIdle() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setLED(false);  // Steady on
    }
}

void MorseConverter::indicateProcessing() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setLED(true);
        delay(50);
        setLED(false);
    }
}

void MorseConverter::indicatePlaying() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        setLED(true);
        delay(50);
        setLED(false);
    }
}

void MorseConverter::indicateError() {
    if (outputMode != OutputMode::VIBRATION_ONLY) {
        for (int i = 0; i < 3; i++) {
            setLED(true);
            delay(30);
            setLED(false);
            delay(30);
        }
    }
}

void MorseConverter::clearStatus() {
    updateOutputs(false, hapticIntensity);
} 