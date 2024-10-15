Template for using 4D Systems gen4 ESP32 displays using PlatformIO with the Arduino framework and LVGL.

Uses 4dsystems/GFX4dESP32 for initializing the display.  (Code largely based on this discussion: https://forum.4dsystems.com.au/node/80632)

Example display code taken from: https://docs.lvgl.io/master/examples.html


Long story short, https://github.com/pioarduino/platform-espressif32 is the hero here, allowing compatibility that was not possible with the official platform (https://github.com/platformio/platform-espressif32).

This template contains a couple other minor fixes to make life easy, namely including SdFat and initializing the backlight pin.

