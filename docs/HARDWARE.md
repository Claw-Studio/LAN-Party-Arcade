# ESP32-2432S028 Hardware Reference

## Board Overview

The **ESP32-2432S028** (also known as "Cheap Yellow Display" or CYD) is an integrated development board featuring:

- **ESP32-WROOM-32** module (Wi-Fi + Bluetooth)
- **2.8" ILI9341 TFT LCD** (320×240 resolution)
- **XPT2046 Resistive Touch Controller**
- **MicroSD Card Slot** (TF card)
- **RGB LED** (common cathode)
- **Audio Amplifier** (speaker output)
- **USB-C & Micro-USB** ports (power + programming)
- **I²C Expansion Headers**

**Key Feature:** Dual SPI buses (HSPI for display, VSPI for SD card) allow simultaneous operation without conflicts.

---

## 🖥️ Display (ILI9341 TFT LCD)

### Specifications
- **Resolution:** 320×240 pixels
- **Controller:** ILI9341
- **Interface:** HSPI (Hardware SPI)
- **Colors:** 16-bit RGB (65,536 colors)

### Pin Connections (HSPI Bus)
| Function | GPIO | Notes |
|----------|------|-------|
| **CS** (Chip Select) | **15** | Display select line |
| **DC** (Data/Command) | **2** | Command vs data mode |
| **MOSI** (Data Out) | **13** | SPI data to display |
| **MISO** (Data In) | **12** | SPI data from display (rarely used) |
| **SCK** (Clock) | **14** | SPI clock signal |
| **RESET** | External button | Hardware reset (not GPIO-controlled) |
| **Backlight** | **21** | LED backlight control (active HIGH) |

### Display Configuration
```cpp
// TFT_eSPI User_Setup.h settings
#define ILI9341_DRIVER
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  -1  // Not connected to GPIO
#define TFT_BL   21  // Backlight control
```

### Rotation Modes
```cpp
tft.setRotation(0);  // Portrait (240×320)
tft.setRotation(1);  // Landscape (320×240)
tft.setRotation(2);  // Portrait inverted
tft.setRotation(3);  // Landscape inverted
```

---

## 👆 Touch Screen (XPT2046)

### Specifications
- **Controller:** XPT2046 (compatible with TI TSC2046)
- **Type:** 4-wire resistive touch
- **Interface:** Shared SPI with separate CS
- **Resolution:** 12-bit ADC (0-4095 per axis)

### Pin Connections
| Function | GPIO | Notes |
|----------|------|-------|
| **T_CS** (Touch CS) | **33** | Touch controller select |
| **T_IRQ** (Interrupt) | **36** | Touch press detection (input only) |
| **T_DIN** (MOSI) | **32** | SPI data to touch controller |
| **T_DO** (MISO) | **39** | SPI data from touch (input only) |
| **T_CLK** (Clock) | **25** | SPI clock for touch |

### Touch Calibration (Portrait Mode - Rotation 0)
```cpp
uint16_t calData[5] = {275, 3620, 264, 3532, 1};
tft.setTouch(calData);
```

### Important Notes
- **GPIO 36 & 39** are **input-only** (no pull-ups, ADC capable)
- Touch uses **separate SPI pins** from display (no bus conflicts)
- IRQ pin triggers LOW when screen is pressed
- Calibration values are **orientation-specific**

### Touch Reading
```cpp
uint16_t x, y;
bool pressed = tft.getTouch(&x, &y);
if (pressed) {
    Serial.printf("Touch at: (%d, %d)\n", x, y);
}
```

---

## 💾 MicroSD Card Slot

### Specifications
- **Interface:** VSPI (Hardware SPI)
- **Format:** FAT32 recommended
- **Max Size:** 32GB (SDHC standard)
- **Speed:** Up to 25MHz SPI clock

### Pin Connections (VSPI Bus)
| Function | GPIO | SD Pin | Notes |
|----------|------|--------|-------|
| **CS** | **5** | DAT3/CS | Card select |
| **MOSI** | **23** | CMD/DI | Data to card |
| **MISO** | **19** | DAT0/DO | Data from card |
| **SCK** | **18** | CLK | SPI clock |

### SD Card Initialization
```cpp
#define SD_CS 5
if (SD.begin(SD_CS, SPI, 25000000)) {
    Serial.println("SD card mounted");
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("Card Size: %lluMB\n", cardSize);
}
```

### File Operations
```cpp
// Read file
File file = SD.open("/config.json", FILE_READ);
if (file) {
    String content = file.readString();
    file.close();
}

// Write file
File file = SD.open("/data.txt", FILE_WRITE);
if (file) {
    file.println("Hello World");
    file.close();
}

// Check if file exists
bool exists = SD.exists("/test.html");
```

---

## 🎨 RGB LED

### Specifications
- **Type:** Common cathode RGB LED
- **Control:** Direct GPIO (active HIGH)
- **Current:** ~20mA per channel

### Pin Connections
| Color | GPIO | Notes |
|-------|------|-------|
| **Red** | **17** | Digital on/off or PWM |
| **Green** | **16** | Digital on/off or PWM |
| **Blue** | **4** | Digital on/off or PWM |

### Usage Examples
```cpp
// Digital control
pinMode(17, OUTPUT);  // Red
pinMode(16, OUTPUT);  // Green
pinMode(4, OUTPUT);   // Blue

digitalWrite(17, HIGH);  // Red ON
digitalWrite(16, LOW);   // Green OFF
digitalWrite(4, HIGH);   // Blue ON

// PWM control (brightness)
ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit
ledcAttachPin(17, 0);
ledcWrite(0, 128);      // 50% brightness
```

---

## 🔊 Audio Output

### Specifications
- **Amplifier:** Onboard PAM8403 (or similar)
- **Power:** 1.5W @ 4Ω
- **Connector:** 2-pin JST (labeled "SPEAK")

### Pin Connection
| Function | GPIO | Notes |
|----------|------|-------|
| **Audio Out** | **26** | DAC or PWM audio |

### Usage Examples
```cpp
// Simple tone (PWM)
ledcSetup(1, 1000, 8);  // Channel 1, 1kHz tone
ledcAttachPin(26, 1);
ledcWrite(1, 128);      // 50% duty cycle

// DAC output (0-255)
dacWrite(26, 128);      // Analog output

// No tone
ledcWrite(1, 0);
```

---

## 🔌 Expansion Headers

### IO1 Header (4-pin, 1.25mm JST)
| Pin | GPIO | Capabilities |
|-----|------|--------------|
| GND | - | Ground |
| IO | **35** | Input only, ADC1_CH7 |
| SCL | **22** | I²C clock |
| SDA | **21** | I²C data / Backlight (shared!) |

⚠️ **Note:** GPIO 21 is shared with TFT backlight!

### IO2 Header (4-pin, 1.25mm JST)
| Pin | GPIO | Capabilities |
|-----|------|--------------|
| GND | - | Ground |
| SCL | **22** | I²C clock (duplicate) |
| IO | **27** | TOUCH7, ADC2_CH7 |
| 3.3V | - | Power output |

### I²C Configuration
```cpp
#include <Wire.h>

Wire.begin(21, 22);  // SDA=21, SCL=22
Wire.setClock(100000);  // 100kHz standard mode
```

---

## 🔋 Power & Programming

### USB Ports
| Port | Purpose | Notes |
|------|---------|-------|
| **USB-C** | Power + Serial | Primary programming port |
| **Micro-USB** | Power + Serial | Alternative programming port |

**Both ports work simultaneously** - useful for power + serial debugging.

### Power Specifications
- **Input Voltage:** 5V USB
- **Logic Level:** 3.3V
- **Max Current:** ~500mA typical, 1A max

### Serial Programming
| Signal | GPIO | Notes |
|--------|------|-------|
| **TX** | 1 | ESP32 transmit |
| **RX** | 3 | ESP32 receive |
| **Baud Rate** | 115200 | Default |

### Serial Header (JST)
Pinout: **VIN | TX | RX | GND**

---

## 🎛️ Buttons

### BOOT Button
- **Function:** GPIO 0 (pull-down)
- **Purpose:** Enter bootloader mode (hold during reset)

### RST Button
- **Function:** Hardware reset
- **Purpose:** Restart ESP32

---

## 📊 Pin Usage Summary

### Reserved/Used Pins
| GPIO | Function | Direction | Notes |
|------|----------|-----------|-------|
| 2 | TFT DC | Output | Display command/data |
| 4 | RGB Blue | Output | LED control |
| 5 | SD CS | Output | SD card select |
| 12 | TFT MISO | Input | Display SPI (rarely used) |
| 13 | TFT MOSI | Output | Display SPI data |
| 14 | TFT SCK | Output | Display SPI clock |
| 15 | TFT CS | Output | Display select |
| 16 | RGB Green | Output | LED control |
| 17 | RGB Red | Output | LED control |
| 18 | SD SCK | Output | SD SPI clock |
| 19 | SD MISO | Input | SD SPI data |
| 21 | TFT Backlight | Output | Also on IO1 header |
| 22 | I²C SCL | I/O | Expansion headers |
| 23 | SD MOSI | Output | SD SPI data |
| 25 | Touch CLK | Output | Touch SPI clock |
| 26 | Audio | Output | Speaker output |
| 27 | Expansion | I/O | IO2 header |
| 32 | Touch MOSI | Output | Touch SPI data |
| 33 | Touch CS | Output | Touch select |
| 35 | Expansion | Input only | IO1 header |
| 36 | Touch IRQ | Input only | Touch interrupt |
| 39 | Touch MISO | Input only | Touch SPI data |

### Available GPIO
**Free for custom use:** 0, 34

**Strapping pins (use with caution):** 0, 2, 5, 12, 15

---

## ⚠️ Important Constraints

### Input-Only Pins
**GPIO 34, 35, 36, 39** cannot be used as outputs or have pull-ups enabled.

### Strapping Pins
**GPIO 0, 2, 5, 12, 15** affect boot behavior. Avoid external pull-ups/downs.

### Shared Resources
- **GPIO 21:** Backlight + IO1 header (SDA)
- **GPIO 22:** I²C on both IO1 and IO2 headers
- **HSPI vs VSPI:** Display and SD use separate buses (no conflicts)

### SPI Bus Architecture
```
HSPI (Display & Touch):
├── Display: CS=15, DC=2, MOSI=13, MISO=12, SCK=14
└── Touch:   CS=33, MOSI=32, MISO=39, SCK=25

VSPI (SD Card):
└── SD Card: CS=5, MOSI=23, MISO=19, SCK=18
```

---

## 🔧 Troubleshooting

### Display Not Working
- Check TFT_CS (GPIO 15) connection
- Verify backlight is HIGH (GPIO 21)
- Confirm HSPI pins (12-15) configured correctly
- Test with `tft.fillScreen(TFT_RED)`

### Touch Not Responding
- Verify touch calibration for current rotation
- Check Touch CS (GPIO 33) is properly configured
- GPIO 36 (IRQ) must be input-only
- Test raw touch values with `tft.getTouchRaw(&x, &y)`

### SD Card Not Detected
- Ensure card is FAT32 formatted
- Try lower SPI speed: `SD.begin(SD_CS, SPI, 4000000)`
- Check VSPI pins (5, 18, 19, 23)
- Verify SD_CS (GPIO 5) is OUTPUT

### Power Issues
- USB-C and Micro-USB can both provide power
- Check for brownouts if using power-hungry peripherals
- RGB LED and speaker can draw significant current

---

## 📚 Additional Resources

- **TFT_eSPI Library:** https://github.com/Bodmer/TFT_eSPI
- **ESP32 Arduino Core:** https://github.com/espressif/arduino-esp32
- **Official ESP32 Datasheet:** https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
- **ILI9341 Display Controller:** https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf
- **XPT2046 Touch Controller:** http://www.buydisplay.com/download/ic/XPT2046.pdf

---

## 🎯 Quick Start Checklist

- [ ] Install TFT_eSPI library
- [ ] Configure `User_Setup.h` with correct pins
- [ ] Format SD card as FAT32
- [ ] Upload test sketch via USB-C
- [ ] Verify display shows output
- [ ] Test touch calibration
- [ ] Check SD card file operations
- [ ] Test RGB LED
- [ ] Verify audio output (if needed)

---

**Last Updated:** February 2026  
**Hardware Version:** ESP32-2432S028 (Standard CYD)  
**Firmware:** Arduino-ESP32 Core 2.x / 3.x
