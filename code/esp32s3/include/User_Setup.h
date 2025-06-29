// User_Setup.h cho ESP32-S3 Waveshare Board với ST7789 172x320
// Cấu hình dựa trên pin mapping guide và schematic

#define USER_SETUP_LOADED 1

// Driver cho màn hình ST7789
#define ST7789_DRIVER      // Sử dụng driver ST7789

// Kích thước màn hình theo tài liệu: 172x320 pixels
#define TFT_WIDTH  172
#define TFT_HEIGHT 320

// Cấu hình đặc biệt cho ST7789
#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
//#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// Cấu hình offset cho màn hình (cần thiết cho ST7789)
#define TFT_MISO -1            // Không cần MISO cho write-only display

// Cấu hình GPIO pins theo pin mapping chính xác từ ảnh
#define TFT_MOSI 45  // SPI MOSI
#define TFT_SCLK 40  // SPI SCLK  
#define TFT_CS   42  // LCD_CS - Chip select control pin
#define TFT_DC   41  // LCD_DC - Data Command control pin
#define TFT_RST  39  // LCD_RST - Reset pin
#define TFT_BL   46  // LCD_BL - LED back-light

// Sử dụng HSPI port
#define USE_HSPI_PORT

// Font options - tải các font cần thiết
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Tần số SPI cho ESP32-S3
#define SPI_FREQUENCY       40000000  // 40MHz
#define SPI_READ_FREQUENCY  20000000  // 20MHz
#define SPI_TOUCH_FREQUENCY  2500000  // 2.5MHz

// Smooth fonts
#define SMOOTH_FONT
