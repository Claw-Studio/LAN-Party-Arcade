#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <XPT2046_Bitbang.h>
#include "storage/config.h"
#include "storage/sd_card.h"

enum class Screen {
  CONNECTION,
  STATS
};

class DisplayManager {
public:
  // Initialize display
  static void init();
  
  // Set backlight brightness (0-255)
  static void setBrightness(uint8_t level);
  
  // Show connection information screen with QR codes
  static void showConnectionScreen(const SystemConfig& config, const String& actualSSID);
  
  // Show system stats screen
  static void showStatsScreen(int wifiClients, int wsClients, bool sdMounted, 
                              const SystemConfig& config, const String& actualSSID);
  
  // Toggle between screens
  static void toggleScreen();
  
  // Check for touch input
  static bool checkTouch(uint16_t& x, uint16_t& y);
  
  // Get current screen
  static Screen getCurrentScreen();

private:
  static TFT_eSPI tft;
  static Screen currentScreen;
  static const uint8_t BACKLIGHT_PIN = 21;
  
  // Touch screen - bitbang mode (no SPI)
  static XPT2046_Bitbang touch;
  static const uint8_t TOUCH_IRQ = 36;
  static const uint8_t TOUCH_MOSI = 32;
  static const uint8_t TOUCH_MISO = 39;
  static const uint8_t TOUCH_CLK = 25;
  static const uint8_t TOUCH_CS = 33;
};

#endif
