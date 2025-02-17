# MorseCodify

A Morse code communication system using the Seeed XIAO nRF52840 Sense and Flutter mobile app. This project enables bidirectional Morse code communication through:
- Text-to-Morse conversion with haptic feedback
- (Future) Morse-to-text input using the built-in IMU
- Bluetooth Low Energy (BLE) connectivity
- Cross-platform mobile app interface

## Project Structure
```
MorseCodify/
├── firmware/           # Arduino/PlatformIO project
│   ├── src/
│   │   ├── main.cpp   # Main Arduino application
│   │   ├── morse_code.h    # Morse code lookup tables
│   │   ├── ble_service.h   # BLE service definitions
│   │   └── imu_reader.h    # IMU interface (future)
│   └── platformio.ini # PlatformIO configuration
└── mobile/            # Flutter mobile application
    └── lib/
        ├── models/    # Data models
        ├── services/  # BLE services
        └── ui/        # UI components
```

## Implementation Phases

### Phase 1: Core Firmware Setup
- [x] Basic Morse code conversion
- [ ] BLE Service Setup
  - Create a custom BLE service for Morse code communication
  - Characteristics:
    - Text Input (Write)
    - Morse Code Output (Notify)
    - Haptic Feedback Control (Write)
    - Device Status (Read/Notify)
- [ ] Haptic Feedback Implementation
  - Connect vibration motor to GPIO pin
  - Implement PWM control for variable intensity
  - Define timing constants for Morse code patterns

### Phase 2: Mobile App Development
- [ ] Port existing Flutter BLE framework
  - Update device name filter to "MorseCodify"
  - Implement BLE service UUID handling
- [ ] UI Components
  - Text input field for message composition
  - Morse code visualization
  - Connection status indicator
  - Haptic feedback controls
  - Settings panel for:
    - Morse code timing
    - Vibration intensity
    - Connection preferences

### Phase 3: Bidirectional Communication
- [ ] Implement reliable BLE data transfer
  - Handle message queuing
  - Implement acknowledgment system
  - Add error recovery
- [ ] Add real-time feedback
  - Visual morse code representation
  - Haptic feedback synchronization
  - Connection quality indicators

### Phase 4: IMU-Based Input (Future)
- [ ] IMU Integration
  - Initialize LSM6DS3 sensor
  - Implement tap detection
  - Configure gesture recognition
- [ ] Morse Code Input Processing
  - Tap timing analysis
  - Pattern recognition
  - Input validation
- [ ] Mobile App Updates
  - Add input visualization
  - Implement training mode
  - Add input sensitivity controls

## Technical Specifications

### Hardware Requirements
- Seeed XIAO nRF52840 Sense
- Vibration motor (3.3V compatible)
- Battery (optional for portable use)

### BLE Service Specification
```
Service UUID: "19B10000-E8F2-537E-4F6C-D104768A1214"
Characteristics:
- Text Input:     "19B10001-E8F2-537E-4F6C-D104768A1214" (Write)
- Morse Output:   "19B10002-E8F2-537E-4F6C-D104768A1214" (Notify)
- Haptic Control: "19B10003-E8F2-537E-4F6C-D104768A1214" (Write)
- Device Status:  "19B10004-E8F2-537E-4F6C-D104768A1214" (Read/Notify)
```

### Morse Code Timing (Configurable)
- Dot duration: 100ms (base unit)
- Dash duration: 300ms (3x dot)
- Symbol space: 100ms (1x dot)
- Letter space: 300ms (3x dot)
- Word space: 700ms (7x dot)

### Power Management
- BLE advertising interval: 100ms-500ms (dynamic based on connection)
- Sleep mode when inactive
- Wake on:
  - BLE connection
  - IMU movement (when implemented)
  - Serial command

## Development Setup

### Firmware Development
1. Install PlatformIO IDE
2. Clone this repository
3. Open the firmware folder in PlatformIO
4. Install required libraries:
   - ArduinoBLE
   - Seeed_Arduino_LSM6DS3
5. Configure `platformio.ini` for your environment
6. Build and upload

### Mobile Development
1. Install Flutter SDK
2. Install required dependencies:
   ```bash
   cd mobile
   flutter pub get
   ```
3. Configure BLE permissions in:
   - Android: `android/app/src/main/AndroidManifest.xml`
   - iOS: `ios/Runner/Info.plist`
4. Run the app:
   ```bash
   flutter run
   ```

## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License
MIT License - See LICENSE file for details 