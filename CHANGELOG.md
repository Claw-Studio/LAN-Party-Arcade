# Changelog

All notable changes to LAN Party Arcade will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-02-09

### Added
- **Touchscreen Support** - Tap anywhere on screen to toggle between views
  - Connection screen (WiFi QR codes + network details)
  - System stats screen (memory, uptime, client counts)
  - XPT2046 resistive touch controller via bitbang library
  - 500ms touch debouncing to prevent accidental toggles
- Display manager with two screen modes
- Auto-refresh system stats every 2 seconds when in stats view
- BMP logo support for customizable header image
- Dual QR codes (WiFi connection + URL)
- SD card support with FAT32 filesystem
- WebSocket relay server for real-time multiplayer (port 81)
- HTTP web server serving games from SD card (port 80)
- DNS server with wildcard redirect
- mDNS responder (`http://[hostname].local`)
- WiFi Access Point with configurable SSID/password
- JSON configuration from SD card (`/config.json`)
- Modular architecture with separated concerns:
  - Display subsystem
  - Network layer (WiFi, DNS, HTTP, WebSocket)
  - Storage layer (SD card, config management)
  - Utility helpers

### Technical Details
- **Platform**: ESP32-2432S028 ("Cheap Yellow Display")
- **Display**: 240x320 TFT (ILI9341) in portrait mode
- **Touch**: XPT2046 resistive touchscreen (separate SPI bus, bitbang mode)
- **Framework**: Arduino on ESP32, PlatformIO build system
- **Libraries**:
  - TFT_eSPI 2.5.43+ (display)
  - XPT2046_Bitbang 2.0.1+ (touch)
  - ArduinoJson 7.4.2+ (config)
  - WebSockets 2.7.3+ (relay)
  - QRCode 0.0.1+ (QR generation)

### Architecture
- **Client-heavy design**: ESP32 is a "dumb relay" - all game logic runs on client phones
- **Hot-swappable games**: Games loaded from SD card, no firmware updates needed
- **Nintendo cartridge philosophy**: ESP32 = console, SD card = game cartridge, phones = controllers

### Known Limitations
- Touch calibration may need adjustment for different board revisions
- Stats screen updates every 2 seconds (intentional to reduce flicker)  
- Display is informational only - games render on client devices

### Documentation
- Full architecture documentation in `docs/ARCHITECTURE.md`
- Hardware details in `docs/HARDWARE.md`
- SD card setup guide in `MiniSD/SD_CARD_GUIDE.md`
- Example game in `MiniSD/games/_example_dice_roller/`

---

## [Unreleased]

### Planned for V2.0
- LVGL UI framework integration
- Game selection menu on display
- Admin controls (kick players, restart game)
- Real-time game state visualization
- Touch-based game interactions (beyond screen toggling)
- Capacitive touch support for alternative displays

---

**Legend:**
- `Added`: New features
- `Changed`: Changes in existing functionality  
- `Deprecated`: Soon-to-be removed features
- `Removed`: Removed features
- `Fixed`: Bug fixes
- `Security`: Security fixes
