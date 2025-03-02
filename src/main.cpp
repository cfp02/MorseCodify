#include <Arduino.h>
#include <ArduinoBLE.h>
#include "morse_converter.h"

// BLE UUIDs - must match Flutter app
#define MORSE_SERVICE_UUID        "19B10000-E8F2-537E-4F6C-D104768A1214"
#define TEXT_INPUT_UUID          "19B10001-E8F2-537E-4F6C-D104768A1214"
#define MORSE_OUTPUT_UUID        "19B10002-E8F2-537E-4F6C-D104768A1214"
#define HAPTIC_CONTROL_UUID      "19B10003-E8F2-537E-4F6C-D104768A1214"
#define DEVICE_STATUS_UUID       "19B10004-E8F2-537E-4F6C-D104768A1214"

// Pin definitions
const int VIBRATION_PIN = 2;  // GPIO6 for D6 on XIAO ESP32S3
const int DEFAULT_HAPTIC_INTENSITY = 128;  // 50% intensity

// Status codes - must match Flutter app
enum DeviceStatus {
    IDLE = 0,
    PROCESSING = 1,
    PLAYING = 2,
    ERROR = 3
};

// Advertising state
bool isConnected = false;
unsigned long lastBlink = 0;
bool blinkState = false;

// BLE Service and Characteristics
BLEService morseService(MORSE_SERVICE_UUID);
BLECharacteristic textInputChar(TEXT_INPUT_UUID, BLEWrite, 100);
BLECharacteristic morseOutputChar(MORSE_OUTPUT_UUID, BLERead | BLENotify, 400);
BLECharacteristic hapticControlChar(HAPTIC_CONTROL_UUID, BLEWrite, sizeof(int));
BLECharacteristic deviceStatusChar(DEVICE_STATUS_UUID, BLERead | BLENotify, sizeof(int));

// Morse code converter - start with LED only mode
MorseConverter morse(VIBRATION_PIN, OutputMode::BOTH);
DeviceStatus currentStatus = IDLE;
int hapticIntensity = DEFAULT_HAPTIC_INTENSITY;

void updateStatus(DeviceStatus status) {
    currentStatus = status;
    int statusValue = static_cast<int>(status);
    deviceStatusChar.writeValue(&statusValue, sizeof(statusValue));
    
    // Update LED status
    switch (status) {
        case IDLE:
            morse.indicateIdle();
            break;
        case PROCESSING:
            morse.indicateProcessing();
            break;
        case PLAYING:
            morse.indicatePlaying();
            break;
        case ERROR:
            morse.indicateError();
            break;
    }
}

void handleTextInput(BLEDevice central, BLECharacteristic characteristic) {
    // Get the text input
    String text;
    const int dataLength = characteristic.valueLength();
    const byte* data = characteristic.value();
    
    if (!data || dataLength == 0) {
        updateStatus(ERROR);
        return;
    }
    
    text.reserve(dataLength);
    for (int i = 0; i < dataLength; i++) {
        text += static_cast<char>(data[i]);
    }

    // Update status
    updateStatus(PROCESSING);

    // Convert to Morse code
    const char* morseCode = morse.textToMorse(text.c_str());
    if (!morseCode || morseCode[0] == '\0') {
        updateStatus(ERROR);
        return;
    }
    
    // Send Morse code back through BLE
    if (!morseOutputChar.writeValue(morseCode)) {
        updateStatus(ERROR);
        return;
    }

    // Update status and play
    updateStatus(PLAYING);
    
    // Play the Morse code
    morse.playMorseCode(morseCode);
    
    // Reset status
    updateStatus(IDLE);
}

void handleHapticControl(BLEDevice central, BLECharacteristic characteristic) {
    const byte* data = characteristic.value();
    if (!data) {
        return;
    }
    
    hapticIntensity = data[0];
    
    // If intensity is 0, switch to LED only mode
    // If intensity is non-zero, switch to both LED and vibration
    morse.setOutputMode(hapticIntensity == 0 ? OutputMode::LED_ONLY : OutputMode::BOTH);
    
    if (hapticIntensity > 0) {
        analogWrite(morse.getVibrationPin(), hapticIntensity);
    }
}

void blePeripheralConnectHandler(BLEDevice central) {
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());
    updateStatus(IDLE);
}

void blePeripheralDisconnectHandler(BLEDevice central) {
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    morse.clearStatus();
}

void setup() {
    Serial.begin(115200);
    
    // Don't wait for Serial in production
    #ifdef DEBUG
    while (!Serial);
    #endif

    // Initialize BLE
    if (!BLE.begin()) {
        Serial.println(F("Failed to initialize BLE!"));
        updateStatus(ERROR);
        return;
    }

    // Set up the BLE device
    BLE.setLocalName("MorseCodify");
    BLE.setAdvertisedService(morseService);

    // Add characteristics to service
    morseService.addCharacteristic(textInputChar);
    morseService.addCharacteristic(morseOutputChar);
    morseService.addCharacteristic(hapticControlChar);
    morseService.addCharacteristic(deviceStatusChar);

    // Add service
    BLE.addService(morseService);

    // Set initial values
    updateStatus(IDLE);

    // Set up event handlers
    textInputChar.setEventHandler(BLEWritten, handleTextInput);
    hapticControlChar.setEventHandler(BLEWritten, handleHapticControl);
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

    // Start advertising
    BLE.advertise();
    Serial.println(F("MorseCodify device ready!"));
}

void loop() {
    // Listen for BLE peripherals
    BLEDevice central = BLE.central();

    if (central) {
        if (!isConnected) {
            isConnected = true;
            morse.indicateIdle();  // Steady LED when connected
            Serial.print(F("Connected to central: "));
            Serial.println(central.address());
        }

        while (central.connected()) {
            BLE.poll();
        }

        // When disconnected
        isConnected = false;
        morse.clearStatus();
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    } else {
        // Blink LED while advertising
        unsigned long now = millis();
        if (now - lastBlink > 500) {  // Blink every 500ms
            blinkState = !blinkState;
            morse.setLED(blinkState);  // Use the proper LED control method
            lastBlink = now;
        }
        BLE.poll();
    }
} 