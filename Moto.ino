#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>  // vending machine's 16x2 LCD (shares the OLED's I2C bus)

#define BUZZER 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ================================================================
// BITMAPS (unchanged from your original)
// ================================================================
const unsigned char epd_bitmap_logo [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x03, 0xc1, 0x83, 0xc0, 0x03, 0xe1, 0x83, 0xc0, 0x00, 0xf1, 0x8f, 0x80, 
	0x00, 0x79, 0x9f, 0x00, 0x00, 0x3d, 0x9e, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x03, 0xf3, 0xef, 0xc0, 
	0x03, 0xf3, 0xef, 0xc0, 0x0c, 0x0f, 0xf0, 0x10, 0x10, 0x0e, 0x78, 0x08, 0x10, 0x1e, 0x78, 0x08, 
	0x20, 0x0c, 0x19, 0x86, 0x20, 0x00, 0x00, 0x46, 0x20, 0x00, 0x00, 0x46, 0x20, 0x00, 0x00, 0x36, 
	0x20, 0x00, 0x00, 0x36, 0x20, 0x00, 0x00, 0x36, 0x20, 0x00, 0x00, 0x46, 0x20, 0x00, 0x00, 0x06, 
	0x20, 0x00, 0x00, 0x06, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x08, 0x0c, 0x00, 0x00, 0x10, 
	0x03, 0x00, 0x00, 0xe0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x07, 0x00, 0x00, 0x1f, 0xf8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char epd_bitmap_focuss [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x7f, 0x80, 0x01, 
	0xff, 0x81, 0xff, 0x80, 0x01, 0xff, 0xc7, 0xff, 0x80, 0x09, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 
	0xe7, 0xff, 0x98, 0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 
	0xff, 0x90, 0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 0xff, 
	0x90, 0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 0xff, 0x90, 
	0x19, 0xff, 0xe7, 0xff, 0x90, 0x19, 0xff, 0xe7, 0xf0, 0x10, 0x19, 0xff, 0xe7, 0xc0, 0x00, 0x19, 
	0xff, 0xe7, 0x8f, 0xc0, 0x19, 0xff, 0xe7, 0x18, 0x70, 0x19, 0xff, 0xe7, 0x30, 0x18, 0x18, 0x01, 
	0xe6, 0x61, 0x18, 0x18, 0x00, 0x66, 0x41, 0x08, 0x1f, 0xf8, 0x00, 0x43, 0x08, 0x1f, 0xff, 0x00, 
	0x41, 0x08, 0x08, 0x07, 0xc2, 0x40, 0x88, 0x00, 0x00, 0xfe, 0x60, 0x18, 0x00, 0x00, 0x3c, 0x20, 
	0x18, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_games [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x1f, 0x00, 0x01, 0xfe, 0x00, 0x3f, 0xc0, 0x03, 
	0x03, 0xff, 0xe0, 0xe0, 0x06, 0x00, 0x00, 0x00, 0x60, 0x04, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 
	0x00, 0x00, 0x30, 0x0c, 0x30, 0x00, 0x0e, 0x18, 0x08, 0x30, 0x00, 0x0e, 0x18, 0x18, 0x30, 0x00, 
	0x04, 0x18, 0x19, 0xfe, 0x00, 0x71, 0xc8, 0x11, 0xfe, 0x00, 0x71, 0xcc, 0x30, 0xfe, 0x00, 0x71, 
	0xcc, 0x30, 0x30, 0x00, 0x0e, 0x04, 0x30, 0x30, 0x00, 0x0e, 0x06, 0x20, 0x00, 0x00, 0x0e, 0x06, 
	0x20, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x06, 0x60, 0x01, 0xff, 0xc0, 0x06, 0x60, 
	0x0f, 0xff, 0xf0, 0x02, 0x60, 0x0c, 0x00, 0x18, 0x02, 0x60, 0x18, 0x00, 0x08, 0x06, 0x60, 0x30, 
	0x00, 0x0c, 0x06, 0x30, 0x30, 0x00, 0x06, 0x06, 0x38, 0x60, 0x00, 0x07, 0x0c, 0x1f, 0xc0, 0x00, 
	0x01, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char epd_bitmap_muciss [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 
	0x0f, 0xff, 0x80, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 
	0xff, 0x80, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x03, 0xff, 0xf9, 0x80, 0x00, 0x03, 0xff, 0xc1, 
	0x80, 0x00, 0x03, 0xfc, 0x01, 0x80, 0x00, 0x03, 0xf0, 0x01, 0x80, 0x00, 0x03, 0x80, 0x01, 0x80, 
	0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 
	0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 
	0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x3b, 0x00, 0x00, 0x03, 0x00, 0xff, 0x00, 0x00, 0x03, 0x01, 
	0xff, 0x00, 0x00, 0x03, 0x01, 0xff, 0x80, 0x00, 0x03, 0x03, 0xff, 0x80, 0x00, 0xff, 0x03, 0xff, 
	0x00, 0x01, 0xff, 0x03, 0xff, 0x00, 0x03, 0xff, 0x01, 0xff, 0x00, 0x07, 0xff, 0x00, 0xfe, 0x00, 
	0x07, 0xff, 0x00, 0x78, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x03, 
	0xfc, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_settingss [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x70, 0x7f, 0x07, 0x00, 0x00, 0xf9, 
	0xff, 0xcf, 0x80, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 
	0xff, 0xc0, 0x01, 0xff, 0xc3, 0xff, 0xc0, 0x00, 0xff, 0x00, 0x7f, 0x80, 0x00, 0xfe, 0x00, 0x3f, 
	0x80, 0x00, 0xfc, 0x00, 0x1f, 0x80, 0x01, 0xf8, 0x00, 0x0f, 0x80, 0x0f, 0xf0, 0x1c, 0x0f, 0xf8, 
	0x1f, 0xf0, 0x3e, 0x07, 0xfc, 0x1f, 0xf0, 0x7f, 0x07, 0xfc, 0x1f, 0xf0, 0x7f, 0x07, 0xfc, 0x1f, 
	0xf0, 0x7f, 0x07, 0xfc, 0x1f, 0xf0, 0x7f, 0x07, 0xfc, 0x1f, 0xf0, 0x3e, 0x07, 0xfc, 0x01, 0xf8, 
	0x00, 0x0f, 0xc0, 0x00, 0xf8, 0x00, 0x0f, 0x80, 0x00, 0xfc, 0x00, 0x1f, 0x80, 0x00, 0xfe, 0x00, 
	0x3f, 0x80, 0x00, 0xff, 0x80, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 
	0xe0, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x78, 0xff, 0x8f, 0x00, 
	0x00, 0x20, 0x7f, 0x06, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_staticss [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 
	0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x01, 0xc0, 
	0x18, 0x00, 0x00, 0x03, 0xe0, 0x30, 0x00, 0x00, 0x03, 0xf1, 0xe1, 0xf0, 0x00, 0x03, 0xff, 0xe1, 
	0xf0, 0x00, 0x07, 0xcf, 0xe1, 0xf0, 0x00, 0x0c, 0x07, 0xe1, 0xf0, 0x00, 0x18, 0x03, 0xc1, 0xf0, 
	0x01, 0xf0, 0x00, 0x01, 0xf0, 0x03, 0xe0, 0x00, 0x01, 0xf0, 0x07, 0xc0, 0x00, 0x01, 0xf0, 0x07, 
	0xc0, 0x00, 0x01, 0xf0, 0x03, 0xc3, 0xe0, 0x01, 0xf0, 0x00, 0x03, 0xe1, 0xf1, 0xf0, 0x00, 0x03, 
	0xe1, 0xf1, 0xf0, 0x00, 0x03, 0xe1, 0xf1, 0xf0, 0x00, 0x03, 0xe1, 0xf1, 0xf0, 0x00, 0x03, 0xe1, 
	0xf1, 0xf0, 0x03, 0xe3, 0xe1, 0xf1, 0xf0, 0x03, 0xe3, 0xe1, 0xf1, 0xf0, 0x03, 0xe3, 0xe1, 0xf1, 
	0xf0, 0x03, 0xe3, 0xe1, 0xf1, 0xf0, 0x03, 0xe3, 0xe1, 0xf1, 0xf0, 0x03, 0xe3, 0xe1, 0xf1, 0xf0, 
	0x03, 0xe3, 0xe1, 0xf1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 
	0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char* icons[] =
{
  epd_bitmap_focuss,
  epd_bitmap_staticss,
  epd_bitmap_games,
  epd_bitmap_muciss,
  epd_bitmap_settingss
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================================================================
// MENU / FOCUS DATA
// ================================================================
const char *focusOptions[] = { "25 Minutes", "45 Minutes", "60 Minutes", "Custom" };
const int focusMinutesTable[] = { 25, 45, 60, -1 }; // -1 = use customMinutes
int focusIndex = 0;
const int focusCount = sizeof(focusOptions) / sizeof(focusOptions[0]);

const char *menuItems[] = { "Focus", "Statistics", "Games", "Music", "Settings" };
const int menuCount = sizeof(menuItems) / sizeof(menuItems[0]);
int menuIndex = 0;

int confirmIndex = 0; // 0 = YES, 1 = NO
int customMinutes = 30;

// ================================================================
// GAMES MENU DATA
// ================================================================
const char *gameItems[] = { "Snake", "Simon Says" };
const int gameCount = sizeof(gameItems) / sizeof(gameItems[0]);
int gameMenuIndex = 0;

// ================================================================
// PINS
// ================================================================
#define CLK 2
#define DT  3
#define SW  6
#define VENDING_BUTTON_PIN 7  // wire a push-button between this pin and GND
#define SERVO_PIN 8           // signal wire of the vending machine's servo

// ================================================================
// POINTS / VENDING MACHINE
// ================================================================
// These three used to be fixed #defines. They're now editable from the
// Settings screen, so they live as variables with sane defaults below
// (see "SETTINGS / STATISTICS" section) and are loaded from EEPROM at boot.
#define POINTS_PER_SESSION_DEFAULT 5   // earned on a completed focus session, lost on an interrupted one
#define POINTS_PER_VEND_DEFAULT 10     // cost of one dispense (also the minimum balance needed to vend)
#define SERVO_CLOSED_ANGLE 0
#define SERVO_OPEN_ANGLE 120
#define VENDING_OPEN_MS 600    // how long the servo stays open before auto-closing
#define VEND_MSG_MS 1800         // how long the result screen stays up

// EEPROM (flash-emulated on the Pico) — keeps points, stats and settings
// across power cycles. Two independent magic bytes gate two independent
// blocks: MAGIC guards the points slot (unchanged from before, so older
// saved point balances keep working), MAGIC2 guards the newer stats +
// settings block that this update adds. Without a magic byte, a blank/new
// chip would read garbage as valid data.
#define EEPROM_SIZE 32
#define EEPROM_ADDR_MAGIC 0
#define EEPROM_ADDR_POINTS 1        // int, 4 bytes -> occupies 1..4
#define EEPROM_MAGIC_VALUE 0x50

#define EEPROM_ADDR_MAGIC2 5
#define EEPROM_ADDR_SESSIONS 6      // int, 4 bytes -> 6..9
#define EEPROM_ADDR_FOCUS_MIN 10    // long, 4 bytes -> 10..13
#define EEPROM_ADDR_BEST_SNAKE 14   // int, 4 bytes -> 14..17
#define EEPROM_ADDR_BEST_SIMON 22   // int, 4 bytes -> 22..25 (address kept as-is; 18..21 now unused)
#define EEPROM_ADDR_BREAK_MIN 26    // 1 byte
#define EEPROM_ADDR_PTS_SESSION 27  // 1 byte
#define EEPROM_ADDR_PTS_VEND 28     // 1 byte
#define EEPROM_ADDR_SOUND_ON 29     // 1 byte
#define EEPROM_MAGIC2_VALUE 0xA5

Servo vendingServo;
int focusPoints = 0;

bool vendingOpen = false;
unsigned long vendingOpenedAt = 0;

bool vendBtnDown = false;
unsigned long vendBtnDownAt = 0;
unsigned long vendLastEdgeAt = 0;

// ================================================================
// VENDING MACHINE LCD (16x2, I2C) — mounted on top of the vending unit.
// Shares the same I2C bus as the OLED (same SDA/SCL pins), just
// a different address. If your backpack isn't at 0x27, change it here —
// run an I2C scanner sketch once if you're not sure which address yours is.
// ================================================================
#define VENDING_LCD_ADDR 0x27
#define VENDING_LCD_COLS 16
#define VENDING_LCD_ROWS 2
#define VENDING_LCD_ROTATE_MS 2500 // how long each idle message stays up

LiquidCrystal_I2C vendingLcd(VENDING_LCD_ADDR, VENDING_LCD_COLS, VENDING_LCD_ROWS);

// Short motivational lines shown on the LCD's idle rotation. Kept to 16
// chars or less so they never get clipped on a 16-column display.
const char *motivationalQuotes[] = {
  "Focus Wins!",
  "Small Steps Win",
  "Stay Consistent",
  "You Got This!",
  "Earn Your Break",
  "Discipline Wins"
};
const int motivationalQuoteCount = sizeof(motivationalQuotes) / sizeof(motivationalQuotes[0]);

// Top-level state the LCD is currently showing. Higher-priority states
// (dispensing/locked/live countdowns) override the idle rotation.
#define VLCD_MODE_NONE     -1
#define VLCD_MODE_DISPENSE  0
#define VLCD_MODE_LOCKED    1
#define VLCD_MODE_FOCUS     2
#define VLCD_MODE_BREAK     3
#define VLCD_MODE_IDLE      4

int vendingLcdMode = VLCD_MODE_NONE;
int vendingLcdIdleIndex = 0;          // which idle slot (0-3) is showing
unsigned long vendingLcdIdleChangedAt = 0;
unsigned long vendingLcdLastTick = 0; // per-second refresh timer for live countdowns
int vendingLcdQuoteIndex = 0;

// ================================================================
// SCREEN / STATE
// ================================================================
enum Screen {
  SCR_MENU,
  SCR_FOCUS_OPTIONS,
  SCR_CUSTOM_MINUTES,
  SCR_CONFIRM,
  SCR_TIMER_RUNNING,
  SCR_TIMER_DONE,
  SCR_LOCKED_MSG,
  SCR_GAMES_MENU,
  SCR_GAME_SNAKE,
  SCR_GAME_SIMON,
  SCR_MUSIC,
  SCR_MUSIC_PLAYING,
  SCR_BREAK_OVER,
  SCR_VEND_DISPENSE,
  SCR_VEND_LOCKED,
  SCR_STATISTICS,
  SCR_SETTINGS_MENU,
  SCR_SETTINGS_EDIT
};
Screen currentScreen = SCR_MENU;
unsigned long screenEnteredAt = 0;

// ================================================================
// UNLOCK STATE — Games/Music open only after a focus session finishes
// ================================================================
bool gamesMusicUnlocked = false;

// ================================================================
// BREAK TIMER STATE — Games/Music stay unlocked only for this long
// after a focus session finishes, then they auto-lock again.
// The length is now editable from Settings, so BREAK_DURATION_MS is
// computed from the current setting via breakDurationMs() instead of
// being a fixed macro.
// ================================================================
#define BREAK_DURATION_MINUTES_DEFAULT 5

bool breakActive = false;
unsigned long breakStartMs = 0;
unsigned long lastBreakBadgeRefresh = 0;

// ================================================================
// ENCODER STATE (polled, non-blocking)
// ================================================================
int lastCLK;

// ================================================================
// BUTTON STATE (non-blocking debounce + long-press)
// ================================================================
bool btnDown = false;
unsigned long btnDownAt = 0;
const unsigned long LONG_PRESS_MS = 2000;
const unsigned long DEBOUNCE_MS = 50;
unsigned long lastEdgeAt = 0;

// ================================================================
// TIMER STATE
// ================================================================
long focusDurationMs = 0;
unsigned long timerStartMs = 0;

// ================================================================
// SNAKE GAME STATE
// ================================================================
#define SNAKE_CELL 4
#define SNAKE_COLS 32   // 128 / 4
#define SNAKE_ROWS 12   // (64-14) / 4
#define SNAKE_TOP_Y 14
#define SNAKE_MAX_LEN 96
#define SNAKE_MOVE_INTERVAL 180

int snakeX[SNAKE_MAX_LEN];
int snakeY[SNAKE_MAX_LEN];
int snakeLen = 3;
int snakeDirX = 1;
int snakeDirY = 0;
int foodX = 0, foodY = 0;
int snakeScore = 0;
bool snakeGameOver = false;
bool snakeTurnedThisTick = false;
unsigned long snakeLastMove = 0;

// ================================================================
// SIMON SAYS GAME STATE
// ================================================================
#define SIMON_MAX_LEN 100
#define SIMON_FLASH_ON_MS 400
#define SIMON_FLASH_OFF_MS 200

enum SimonState { SIMON_SHOWING, SIMON_INPUT, SIMON_GAMEOVER };
SimonState simonState = SIMON_SHOWING;
int simonSeq[SIMON_MAX_LEN];
int simonLen = 0;
int simonShowIndex = 0;
bool simonFlashOn = false;
unsigned long simonNextActionAt = 0;
int simonInputIndex = 0;
int simonSelected = 0;

const int simonQX[4] = { 4, 68, 4, 68 };
const int simonQY[4] = { 16, 16, 40, 40 };
const int simonQW = 56;
const int simonQH = 20;

// ================================================================
// SETTINGS / STATISTICS
// ================================================================
// Editable settings (loaded from / saved to EEPROM). Defaults match what
// the fixed macros used to be, so behavior is unchanged out of the box.
int settingBreakMinutes = BREAK_DURATION_MINUTES_DEFAULT;
int settingPointsPerSession = POINTS_PER_SESSION_DEFAULT;
int settingPointsPerVend = POINTS_PER_VEND_DEFAULT;
bool settingSoundOn = true;

// Lifetime stats (loaded from / saved to EEPROM)
int totalSessions = 0;
long totalFocusMinutes = 0;
int bestSnakeScore = 0;
int bestSimonScore = 0;

const char *settingsItems[] = { "Break Min", "Pts/Session", "Pts/Vend", "Sound" };
const int settingsCount = sizeof(settingsItems) / sizeof(settingsItems[0]);
int settingsIndex = 0;
int settingsEditingIndex = 0; // which item (0..2) is being edited in SCR_SETTINGS_EDIT

// How long the break window lasts, in ms, based on the current setting.
unsigned long breakDurationMs()
{
  return (unsigned long)settingBreakMinutes * 60000UL;
}

// Plays a tone only if the user hasn't muted the buzzer in Settings.
void beep(unsigned int freq, unsigned int durationMs)
{
  if (settingSoundOn) {
    tone(BUZZER, freq, durationMs);
  }
}

// ================================================================
// SETUP
// ================================================================
void setup()
{
  pinMode(BUZZER, OUTPUT);

  randomSeed(micros());

  // ---------- EEPROM (saved points balance, stats, settings) ----------
  EEPROM.begin(EEPROM_SIZE);
  loadPoints();
  loadStatsAndSettings();

  // ---------- I2C OLED ----------
  Wire.setSDA(4);     // GP0
  Wire.setSCL(5);     // GP1
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (true); // Stop if OLED not found
  }

  // ---------- Rotary Encoder ----------
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);

  // ---------- Vending Machine ----------
  pinMode(VENDING_BUTTON_PIN, INPUT_PULLUP);
  vendingServo.attach(SERVO_PIN);
  vendingServo.write(SERVO_CLOSED_ANGLE);

  // ---------- Vending Machine LCD ----------
  vendingLcd.init();
  vendingLcd.backlight();
  vendingLcd.clear();

  lastCLK = digitalRead(CLK);
  currentScreen = SCR_MENU;

  splashScreen();
  drawMenu();
}

// ================================================================
// MAIN LOOP (fully non-blocking)
// ================================================================
void loop()
{
  handleEncoder();
  handleButton();
  handleVendingButton();
  updateVendingServo();
  updateVendingLCD();

  if (breakActive) {
    updateBreakTimer();

    // Keep the on-screen break countdown ticking on the screens that show it,
    // without interfering with the frame rate of whatever game is running.
    if (millis() - lastBreakBadgeRefresh >= 1000) {
      lastBreakBadgeRefresh = millis();
      if (currentScreen == SCR_MENU) drawMenu();
      else if (currentScreen == SCR_GAMES_MENU) drawGamesMenu();
      else if (currentScreen == SCR_MUSIC) drawMusicScreen();
    }
  }

  if (currentScreen == SCR_TIMER_RUNNING) {
    updateTimerRunning();
  }

  if (currentScreen == SCR_GAME_SNAKE) {
    updateSnakeGame();
  }

  if (currentScreen == SCR_GAME_SIMON) {
    updateSimonGame();
  }

  if (currentScreen == SCR_MUSIC_PLAYING) {
    updateMusicPlayback();
  }

  if (currentScreen == SCR_LOCKED_MSG && millis() - screenEnteredAt > 1200) {
    currentScreen = SCR_MENU;
    drawMenu();
  }

  if (currentScreen == SCR_BREAK_OVER && millis() - screenEnteredAt > 1800) {
    currentScreen = SCR_MENU;
    menuIndex = 0;
    drawMenu();
  }

  if ((currentScreen == SCR_VEND_DISPENSE || currentScreen == SCR_VEND_LOCKED) &&
      millis() - screenEnteredAt > VEND_MSG_MS) {
    currentScreen = SCR_MENU;
    drawMenu();
  }

  delay(5); // small pacing, keeps encoder + button responsive
}

// ================================================================
// ENCODER HANDLING — behavior depends on current screen
// ================================================================
void handleEncoder()
{
  int currentCLK = digitalRead(CLK);

  if (currentCLK != lastCLK && currentCLK == LOW)
  {
    int dir = (digitalRead(DT) != currentCLK) ? 1 : -1; // +1 = CW, -1 = CCW

    switch (currentScreen)
    {
      case SCR_MENU:
        menuIndex = (menuIndex + dir + menuCount) % menuCount;
        drawMenu();
        break;

      case SCR_FOCUS_OPTIONS:
        focusIndex = (focusIndex + dir + focusCount) % focusCount;
        drawFocusMenu();
        break;

      case SCR_CUSTOM_MINUTES:
        customMinutes += dir;
        customMinutes = constrain(customMinutes, 1, 180);
        drawCustomMinutes();
        break;

      case SCR_CONFIRM:
        confirmIndex = (confirmIndex + dir + 2) % 2;
        drawConfirmScreen();
        break;

      case SCR_GAMES_MENU:
        gameMenuIndex = (gameMenuIndex + dir + gameCount) % gameCount;
        drawGamesMenu();
        break;

      case SCR_GAME_SNAKE:
        // Relative turning: CW = turn right, CCW = turn left (like classic snake)
        // Only one turn is accepted per movement tick — this stops a fast/noisy
        // encoder spin from queuing two turns in a row, which would reverse
        // the snake 180 degrees and make it instantly collide with itself.
        if (!snakeGameOver && !snakeTurnedThisTick) {
          int nx, ny;
          if (dir == 1) { nx = -snakeDirY; ny = snakeDirX; }   // turn right
          else          { nx = snakeDirY;  ny = -snakeDirX; }  // turn left
          snakeDirX = nx;
          snakeDirY = ny;
          snakeTurnedThisTick = true;
        }
        break;

      case SCR_GAME_SIMON:
        if (simonState == SIMON_INPUT) {
          simonSelected = (simonSelected + dir + 4) % 4;
          drawSimonQuadrants(simonSelected, false);
        }
        break;

      case SCR_SETTINGS_MENU:
        settingsIndex = (settingsIndex + dir + settingsCount) % settingsCount;
        drawSettingsMenu();
        break;

      case SCR_SETTINGS_EDIT:
        if (settingsEditingIndex == 0) {
          settingBreakMinutes = constrain(settingBreakMinutes + dir, 1, 60);
        } else if (settingsEditingIndex == 1) {
          settingPointsPerSession = constrain(settingPointsPerSession + dir, 1, 50);
        } else if (settingsEditingIndex == 2) {
          settingPointsPerVend = constrain(settingPointsPerVend + dir, 1, 100);
        }
        drawSettingsEdit();
        break;

      default:
        break; // no encoder action on timer/done/statistics/music screens
    }
  }

  lastCLK = currentCLK;
}

// ================================================================
// BUTTON HANDLING — non-blocking debounce + long press
// ================================================================
void handleButton()
{
  bool reading = (digitalRead(SW) == LOW);
  unsigned long now = millis();

  if (reading && !btnDown && (now - lastEdgeAt > DEBOUNCE_MS))
  {
    btnDown = true;
    btnDownAt = now;
    lastEdgeAt = now;
  }
  else if (!reading && btnDown)
  {
    // released -> was it a short press?
    unsigned long heldFor = now - btnDownAt;
    btnDown = false;
    lastEdgeAt = now;

    if (heldFor < LONG_PRESS_MS) {
      onShortPress();
    }
    // long press already handled live below, nothing to do on release
  }
  else if (reading && btnDown)
  {
    // still holding — check for long press threshold crossed
    if (now - btnDownAt > LONG_PRESS_MS) {
      onLongPress();
      btnDown = false; // consume, avoid re-triggering short press on release
    }
  }
}

void onLongPress()
{
  // If a focus session was running and got interrupted early, dock the points
  // that would have been earned and show a sad-face notice.
  if (currentScreen == SCR_TIMER_RUNNING) {
    focusPoints -= settingPointsPerSession;
    if (focusPoints < 0) focusPoints = 0;
    savePoints();
    drawSessionStopped();
    delay(1200);
  }

  // Cut the buzzer immediately if the song was playing — otherwise it would
  // keep ringing the current note after we've already left the screen.
  stopMusicPlayback();

  // Reset to main menu from anywhere. Skip the full splash animation
  // when bailing out of a game/menu so exiting feels instant.
  bool cameFromGame = (currentScreen == SCR_GAME_SNAKE ||
                        currentScreen == SCR_GAME_SIMON ||
                        currentScreen == SCR_GAMES_MENU ||
                        currentScreen == SCR_MUSIC ||
                        currentScreen == SCR_MUSIC_PLAYING);

  currentScreen = SCR_MENU;
  menuIndex = 0;

  if (!cameFromGame) {
    splashScreen();
  }
  drawMenu();
}

void onShortPress()
{
  switch (currentScreen)
  {
    case SCR_MENU:
      if (menuIndex == 0) { // Focus
        currentScreen = SCR_FOCUS_OPTIONS;
        drawFocusMenu();
      } else if (menuIndex == 1) { // Statistics
        currentScreen = SCR_STATISTICS;
        drawStatistics();
      } else if (menuIndex == 2) { // Games
        if (gamesMusicUnlocked) {
          currentScreen = SCR_GAMES_MENU;
          gameMenuIndex = 0;
          drawGamesMenu();
        } else {
          currentScreen = SCR_LOCKED_MSG;
          screenEnteredAt = millis();
          drawLockedMsg();
        }
      } else if (menuIndex == 3) { // Music
        if (gamesMusicUnlocked) {
          currentScreen = SCR_MUSIC;
          drawMusicScreen();
        } else {
          currentScreen = SCR_LOCKED_MSG;
          screenEnteredAt = millis();
          drawLockedMsg();
        }
      } else if (menuIndex == 4) { // Settings
        currentScreen = SCR_SETTINGS_MENU;
        settingsIndex = 0;
        drawSettingsMenu();
      }
      break;

    case SCR_STATISTICS:
      currentScreen = SCR_MENU;
      drawMenu();
      break;

    case SCR_SETTINGS_MENU:
      if (settingsIndex == settingsCount - 1) { // "Sound" — toggle directly, no edit screen
        settingSoundOn = !settingSoundOn;
        saveSettings();
        drawSettingsMenu();
      } else {
        settingsEditingIndex = settingsIndex;
        currentScreen = SCR_SETTINGS_EDIT;
        drawSettingsEdit();
      }
      break;

    case SCR_SETTINGS_EDIT:
      saveSettings();
      currentScreen = SCR_SETTINGS_MENU;
      drawSettingsMenu();
      break;

    case SCR_MUSIC:
      if (!settingSoundOn) {
        drawMusicMutedNotice();
        delay(1000);
        drawMusicScreen();
      } else {
        startMusicSong();
      }
      break;

    case SCR_MUSIC_PLAYING:
      // Whether it's still playing or just finished, a click takes you
      // back to the Music screen. Cuts the buzzer immediately either way.
      stopMusicPlayback();
      currentScreen = SCR_MUSIC;
      drawMusicScreen();
      break;

    case SCR_GAMES_MENU:
      if (gameMenuIndex == 0) {
        initSnakeGame();
        currentScreen = SCR_GAME_SNAKE;
      } else if (gameMenuIndex == 1) {
        initSimonGame();
        currentScreen = SCR_GAME_SIMON;
      }
      break;

    case SCR_GAME_SNAKE:
      if (snakeGameOver) {
        initSnakeGame(); // retry
      }
      break;

    case SCR_GAME_SIMON:
      if (simonState == SIMON_INPUT) {
        handleSimonInput();
      } else if (simonState == SIMON_GAMEOVER) {
        initSimonGame(); // retry
      }
      break;

    case SCR_FOCUS_OPTIONS:
      if (focusIndex == focusCount - 1) { // "Custom" selected
        currentScreen = SCR_CUSTOM_MINUTES;
        drawCustomMinutes();
      } else {
        currentScreen = SCR_CONFIRM;
        confirmIndex = 0;
        drawConfirmScreen();
      }
      break;

    case SCR_CUSTOM_MINUTES:
      currentScreen = SCR_CONFIRM;
      confirmIndex = 0;
      drawConfirmScreen();
      break;

    case SCR_CONFIRM:
      if (confirmIndex == 0) { // YES
        startTimer();
      } else { // NO
        currentScreen = SCR_FOCUS_OPTIONS;
        drawFocusMenu();
      }
      break;

    case SCR_TIMER_DONE:
      currentScreen = SCR_MENU;
      drawMenu();
      break;

    default:
      break; // no short-press action while timer is running
  }
}

// ================================================================
// TIMER LOGIC
// ================================================================
void startTimer()
{
  int mins = focusMinutesTable[focusIndex];
  if (mins < 0) mins = customMinutes; // Custom
  focusDurationMs = (long)mins * 60000L;

  // Starting a fresh focus session always cancels any break in progress —
  // games/music re-lock immediately and stay locked until this session ends.
  breakActive = false;
  gamesMusicUnlocked = false;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 20);
  display.print("Starting");
  display.display();
  delay(700);

  timerStartMs = millis();
  currentScreen = SCR_TIMER_RUNNING;
  drawTimerRunning(focusDurationMs); // draw immediately so it's not blank
}

void updateTimerRunning()
{
  unsigned long elapsed = millis() - timerStartMs;
  long remaining = focusDurationMs - (long)elapsed;

  if (remaining <= 0) {
    remaining = 0;
    finishTimer();
    return;
  }

  drawTimerRunning(remaining);
}

void finishTimer()
{
  currentScreen = SCR_TIMER_DONE;
  gamesMusicUnlocked = true;

  // Reward points for a completed session — these build up toward a vend.
  focusPoints += settingPointsPerSession;
  savePoints();

  // Track lifetime stats.
  totalSessions++;
  totalFocusMinutes += focusDurationMs / 60000L;
  saveStats();

  // Start the break window — Games/Music stay unlocked only for this long.
  breakActive = true;
  breakStartMs = millis();
  lastBreakBadgeRefresh = millis();

  beep(988, 150);
  delay(180);
  beep(1319, 250);
  drawTimerDone();
}

// ================================================================
// BREAK TIMER LOGIC — Games/Music auto-lock again once the break ends.
// If the person is inside a game or the music screen when the break
// ends, they get bounced straight back to the locked main menu.
// ================================================================
void updateBreakTimer()
{
  if (!breakActive) return;

  unsigned long elapsed = millis() - breakStartMs;
  if (elapsed >= breakDurationMs()) {
    endBreak();
  }
}

void endBreak()
{
  breakActive = false;
  gamesMusicUnlocked = false;

  // A song mid-playback shouldn't keep ringing after the break locks it out.
  stopMusicPlayback();

  currentScreen = SCR_BREAK_OVER;
  screenEnteredAt = millis();
  drawBreakOver();
}

// Small "Pts:N" balance badge, drawn in the icon's empty top-left corner.
void drawPointsBadge()
{
  char buf[12];
  sprintf(buf, "Pts:%d", focusPoints);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2, 16);
  display.print(buf);
}

// Small "B mm:ss" countdown badge, drawn in an empty corner of a screen.
void drawBreakBadge(int x, int y)
{
  if (!breakActive) return;

  long remaining = (long)breakDurationMs() - (long)(millis() - breakStartMs);
  if (remaining < 0) remaining = 0;

  int totalSec = remaining / 1000;
  int mm = totalSec / 60;
  int ss = totalSec % 60;

  char buf[10];
  sprintf(buf, "B%02d:%02d", mm, ss);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.print(buf);
}

// ================================================================
// EEPROM — persists points, stats and settings across power cycles
// ================================================================
void loadPoints()
{
  uint8_t magic = EEPROM.read(EEPROM_ADDR_MAGIC);

  if (magic == EEPROM_MAGIC_VALUE) {
    EEPROM.get(EEPROM_ADDR_POINTS, focusPoints);
    if (focusPoints < 0) focusPoints = 0; // guard against corrupted/blank data
  } else {
    // First time ever running on this chip (or magic byte missing) —
    // start at 0 and write it so future boots find valid data.
    focusPoints = 0;
    savePoints();
  }
}

void savePoints()
{
  EEPROM.write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_VALUE);
  EEPROM.put(EEPROM_ADDR_POINTS, focusPoints);
  EEPROM.commit();
}

// Loads lifetime stats + user settings. Gated by its own magic byte so it
// doesn't disturb the pre-existing points slot on boards upgrading from
// an older version of this sketch.
void loadStatsAndSettings()
{
  uint8_t magic2 = EEPROM.read(EEPROM_ADDR_MAGIC2);

  if (magic2 == EEPROM_MAGIC2_VALUE) {
    EEPROM.get(EEPROM_ADDR_SESSIONS, totalSessions);
    EEPROM.get(EEPROM_ADDR_FOCUS_MIN, totalFocusMinutes);
    EEPROM.get(EEPROM_ADDR_BEST_SNAKE, bestSnakeScore);
    EEPROM.get(EEPROM_ADDR_BEST_SIMON, bestSimonScore);
    settingBreakMinutes = EEPROM.read(EEPROM_ADDR_BREAK_MIN);
    settingPointsPerSession = EEPROM.read(EEPROM_ADDR_PTS_SESSION);
    settingPointsPerVend = EEPROM.read(EEPROM_ADDR_PTS_VEND);
    settingSoundOn = EEPROM.read(EEPROM_ADDR_SOUND_ON) != 0;

    // Guard against corrupted/out-of-range data.
    if (totalSessions < 0) totalSessions = 0;
    if (totalFocusMinutes < 0) totalFocusMinutes = 0;
    if (bestSnakeScore < 0) bestSnakeScore = 0;
    if (bestSimonScore < 0) bestSimonScore = 0;
    if (settingBreakMinutes < 1 || settingBreakMinutes > 60) settingBreakMinutes = BREAK_DURATION_MINUTES_DEFAULT;
    if (settingPointsPerSession < 1 || settingPointsPerSession > 50) settingPointsPerSession = POINTS_PER_SESSION_DEFAULT;
    if (settingPointsPerVend < 1 || settingPointsPerVend > 100) settingPointsPerVend = POINTS_PER_VEND_DEFAULT;
  } else {
    // First boot with this feature — start fresh with defaults and persist them.
    totalSessions = 0;
    totalFocusMinutes = 0;
    bestSnakeScore = 0;
    bestSimonScore = 0;
    settingBreakMinutes = BREAK_DURATION_MINUTES_DEFAULT;
    settingPointsPerSession = POINTS_PER_SESSION_DEFAULT;
    settingPointsPerVend = POINTS_PER_VEND_DEFAULT;
    settingSoundOn = true;
    saveStats();
    saveSettings();
  }
}

void saveStats()
{
  EEPROM.write(EEPROM_ADDR_MAGIC2, EEPROM_MAGIC2_VALUE);
  EEPROM.put(EEPROM_ADDR_SESSIONS, totalSessions);
  EEPROM.put(EEPROM_ADDR_FOCUS_MIN, totalFocusMinutes);
  EEPROM.put(EEPROM_ADDR_BEST_SNAKE, bestSnakeScore);
  EEPROM.put(EEPROM_ADDR_BEST_SIMON, bestSimonScore);
  EEPROM.commit();
}

void saveSettings()
{
  EEPROM.write(EEPROM_ADDR_MAGIC2, EEPROM_MAGIC2_VALUE);
  EEPROM.write(EEPROM_ADDR_BREAK_MIN, (uint8_t)settingBreakMinutes);
  EEPROM.write(EEPROM_ADDR_PTS_SESSION, (uint8_t)settingPointsPerSession);
  EEPROM.write(EEPROM_ADDR_PTS_VEND, (uint8_t)settingPointsPerVend);
  EEPROM.write(EEPROM_ADDR_SOUND_ON, settingSoundOn ? 1 : 0);
  EEPROM.commit();
}

// ================================================================
// VENDING MACHINE — separate button + servo, works independently of
// whatever screen is currently showing (menu, game, etc).
// ================================================================
void handleVendingButton()
{
  bool reading = (digitalRead(VENDING_BUTTON_PIN) == LOW);
  unsigned long now = millis();

  if (reading && !vendBtnDown && (now - vendLastEdgeAt > DEBOUNCE_MS))
  {
    vendBtnDown = true;
    vendBtnDownAt = now;
    vendLastEdgeAt = now;
  }
  else if (!reading && vendBtnDown)
  {
    vendBtnDown = false;
    vendLastEdgeAt = now;
    onVendingButtonPress();
  }
}

void onVendingButtonPress()
{
  if (vendingOpen) return; // already dispensing — ignore extra presses

  if (focusPoints >= settingPointsPerVend) {
    focusPoints -= settingPointsPerVend;
    if (focusPoints < 0) focusPoints = 0;
    savePoints();
    openVendingServo();
  } else {
    currentScreen = SCR_VEND_LOCKED;
    screenEnteredAt = millis();
    drawVendLocked();
  }
}

void openVendingServo()
{
  vendingServo.write(SERVO_OPEN_ANGLE);
  vendingOpen = true;
  vendingOpenedAt = millis();
  beep(1500, 100);

  currentScreen = SCR_VEND_DISPENSE;
  screenEnteredAt = millis();
  drawVendDispense();
}

void updateVendingServo()
{
  if (vendingOpen && millis() - vendingOpenedAt >= VENDING_OPEN_MS) {
    vendingServo.write(SERVO_CLOSED_ANGLE);
    vendingOpen = false;
  }
}

// Writes exactly one 16-char row, padded with spaces, WITHOUT calling
// clear() first — clear() blanks the whole display for a moment which
// causes a visible flicker. Overwriting with padded text is flicker-free.
void lcdWriteLine(int row, const char *text)
{
  char buf[VENDING_LCD_COLS + 1];
  int len = strlen(text);
  if (len > VENDING_LCD_COLS) len = VENDING_LCD_COLS;
  memset(buf, ' ', VENDING_LCD_COLS);
  memcpy(buf, text, len);
  buf[VENDING_LCD_COLS] = '\0';
  vendingLcd.setCursor(0, row);
  vendingLcd.print(buf);
}

// Refreshes just the countdown on line 2 for the live FOCUS/BREAK modes —
// called every second so the mm:ss ticks down without redrawing line 1.
void drawVendingLcdLine2()
{
  char buf[17];
  long remaining;

  if (vendingLcdMode == VLCD_MODE_FOCUS) {
    remaining = focusDurationMs - (long)(millis() - timerStartMs);
  } else if (vendingLcdMode == VLCD_MODE_BREAK) {
    remaining = (long)breakDurationMs() - (long)(millis() - breakStartMs);
  } else {
    return;
  }

  if (remaining < 0) remaining = 0;
  int totalSec = remaining / 1000;
  sprintf(buf, "%02d:%02d left", totalSec / 60, totalSec % 60);
  lcdWriteLine(1, buf);
}

// Draws one of the 4 idle slots that rotate when nothing more urgent
// (dispensing/locked/focus/break) is happening.
void drawVendingLcdIdleSlot()
{
  char buf[17];

  switch (vendingLcdIdleIndex) {
    case 0: // current points balance
      lcdWriteLine(0, "Your Points:");
      sprintf(buf, "%d pts", focusPoints);
      lcdWriteLine(1, buf);
      break;

    case 1: // whether there's enough to vend right now
      lcdWriteLine(0, "Status:");
      lcdWriteLine(1, (focusPoints >= settingPointsPerVend) ? "Ready to Vend!" : "Keep Focusing!");
      break;

    case 2: // lifetime stat
      lcdWriteLine(0, "Lifetime Stats:");
      sprintf(buf, "%d sessions", totalSessions);
      lcdWriteLine(1, buf);
      break;

    case 3: // motivational quote (re-randomized each time we land here)
      lcdWriteLine(0, "FocusBuddy Says:");
      lcdWriteLine(1, motivationalQuotes[vendingLcdQuoteIndex]);
      break;
  }
}

// Full two-line (re)draw for whatever vendingLcdMode currently is.
void drawVendingLcdMode()
{
  switch (vendingLcdMode) {
    case VLCD_MODE_DISPENSE:
      lcdWriteLine(0, "Dispensing...");
      lcdWriteLine(1, "Enjoy your item!");
      break;

    case VLCD_MODE_LOCKED: {
      lcdWriteLine(0, "Locked!");
      char buf[17];
      sprintf(buf, "Need %d points", settingPointsPerVend);
      lcdWriteLine(1, buf);
      break;
    }

    case VLCD_MODE_FOCUS:
      lcdWriteLine(0, "Focusing...");
      drawVendingLcdLine2();
      break;

    case VLCD_MODE_BREAK:
      lcdWriteLine(0, "On a Break!");
      drawVendingLcdLine2();
      break;

    case VLCD_MODE_IDLE:
      drawVendingLcdIdleSlot();
      break;
  }
}

// Drives the 16x2 LCD mounted on the vending machine. Non-blocking, runs
// every loop(). Priority order (highest first):
//   1) Actively dispensing right now        -> "Dispensing..."
//   2) Just got locked out (not enough pts) -> "Locked! Need N points"
//   3) A focus session is running            -> live "Focusing... mm:ss left"
//   4) A break is active                     -> live "On a Break! mm:ss left"
//   5) Otherwise, idly rotate every ~2.5s through:
//        Points balance -> Vend-ready status -> Lifetime stats -> Quote
void updateVendingLCD()
{
  unsigned long now = millis();

  int desiredMode;
  if (vendingOpen) desiredMode = VLCD_MODE_DISPENSE;
  else if (currentScreen == SCR_VEND_LOCKED) desiredMode = VLCD_MODE_LOCKED;
  else if (currentScreen == SCR_TIMER_RUNNING) desiredMode = VLCD_MODE_FOCUS;
  else if (breakActive) desiredMode = VLCD_MODE_BREAK;
  else desiredMode = VLCD_MODE_IDLE;

  bool modeChanged = (desiredMode != vendingLcdMode);

  if (modeChanged) {
    vendingLcdMode = desiredMode;
    vendingLcdLastTick = now;

    if (desiredMode == VLCD_MODE_IDLE) {
      vendingLcdIdleIndex = 0;
      vendingLcdIdleChangedAt = now;
    }

    drawVendingLcdMode(); // full two-line redraw for the new mode
    return;
  }

  // Same mode as last loop — only refresh the parts that actually change.
  switch (vendingLcdMode) {
    case VLCD_MODE_FOCUS:
    case VLCD_MODE_BREAK:
      if (now - vendingLcdLastTick >= 1000) {
        vendingLcdLastTick = now;
        drawVendingLcdLine2(); // just the countdown, no flicker
      }
      break;

    case VLCD_MODE_IDLE:
      if (now - vendingLcdIdleChangedAt >= VENDING_LCD_ROTATE_MS) {
        vendingLcdIdleChangedAt = now;
        vendingLcdIdleIndex = (vendingLcdIdleIndex + 1) % 4;
        if (vendingLcdIdleIndex == 3) {
          vendingLcdQuoteIndex = random(0, motivationalQuoteCount);
        }
        drawVendingLcdMode();
      }
      break;

    default:
      break; // DISPENSE/LOCKED are static, nothing to refresh
  }
}

// ================================================================
// SIMPLE FACE ICON (happy / sad) drawn with primitives — no bitmap needed
// ================================================================
void drawFace(int cx, int cy, int r, bool happy)
{
  // Face outline
  display.drawCircle(cx, cy, r, WHITE);

  // Eyes
  int eyeOffsetX = r / 2;
  int eyeY = cy - r / 4;
  display.fillCircle(cx - eyeOffsetX, eyeY, 2, WHITE);
  display.fillCircle(cx + eyeOffsetX, eyeY, 2, WHITE);

  // Mouth — plotted as a curve using a parabola, no arc() in Adafruit_GFX
  int halfW = (int)(r * 0.6);
  int depth = r / 3;
  int mouthBaseY = cy + r / 3;

  int prevX = -1000, prevY = -1000;
  for (int x = -halfW; x <= halfW; x++) {
    float t = (float)x / (float)halfW;      // -1..1
    int offset = (int)(depth * (1.0 - t * t)); // max at center, 0 at ends
    int y;
    if (happy) {
      y = mouthBaseY + offset;   // center dips down, ends rise -> smile (u)
    } else {
      y = mouthBaseY + depth - offset; // center rises, ends droop -> frown (n)
    }
    int px = cx + x;
    if (prevX != -1000) {
      display.drawLine(prevX, prevY, px, y, WHITE);
    }
    prevX = px;
    prevY = y;
  }
}

 void splashScreen()
{
  beep(784, 120);
  delay(140);
  beep(988, 120);
  delay(140);
  beep(1319, 180);

  display.clearDisplay();
  display.drawBitmap(48, 0, epd_bitmap_logo, 32, 32, WHITE);
  display.display();
  delay(500);

  display.clearDisplay();
  display.drawBitmap(48, 6, epd_bitmap_logo, 32, 32, WHITE);
  display.display();
  delay(500);

  display.clearDisplay();
  display.drawBitmap(48, 8, epd_bitmap_logo, 32, 32, WHITE);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(34, 45);
  display.print("FocusBuddy");
  display.fillCircle(40, 12, 1, WHITE);
  display.fillCircle(88, 12, 1, WHITE);
  display.fillCircle(64, 2, 1, WHITE);
  display.display();
  delay(1500);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(18, 10);
  display.print("Welcome!");
  display.drawLine(20, 30, 108, 30, WHITE);
  display.setTextSize(1);
  display.setCursor(18, 40);
  display.print(" Study Companion");
  display.display();
  delay(1500);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 10);
  display.print("Loading");
  display.drawRect(10, 42, 108, 10, WHITE);
  for (int i = 0; i <= 104; i += 4) {
    display.fillRect(12, 44, i, 6, WHITE);
    display.display();
    delay(40);
  }
  delay(500);
}

void drawMenu()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(4, 2);
  display.print("<");

  int titleX = (128 - strlen("FocusBuddy") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("FocusBuddy");

  display.setCursor(118, 2);
  display.print(">");

  display.drawLine(0, 12, 127, 12, WHITE);
  display.drawBitmap(44, 14, icons[menuIndex], 40, 40, WHITE);

  int x = (128 - strlen(menuItems[menuIndex]) * 6) / 2;
  display.setCursor(x, 56);
  display.print(menuItems[menuIndex]);

  drawPointsBadge();      // balance sits in the icon's empty top-left corner
  drawBreakBadge(86, 16); // countdown sits in the icon's empty top-right corner

  display.display();
}

void drawFocusMenu()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(2, 2);
  display.print("<");
  display.setCursor(28, 2);
  display.print("Focus Timer");
  display.drawLine(0, 14, 127, 14, WHITE);

  for (int i = 0; i < focusCount; i++)
  {
    int y = 18 + i * 11;
    if (i == focusIndex) {
      display.fillRect(0, y - 1, 128, 10, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }
    display.setCursor(8, y);
    display.print(focusOptions[i]);
  }

  display.display();
}

void drawCustomMinutes()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Header
  display.setCursor(2, 2);
  display.print("<");
  int titleX = (128 - strlen("Custom Duration") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("Custom Duration");
  display.drawLine(0, 13, 127, 13, WHITE);

  // Down arrow (left) + Up arrow (right) either side of the number box
  display.fillTriangle(14, 34, 22, 26, 22, 42, WHITE); // left-pointing (decrease)
  display.fillTriangle(114, 34, 106, 26, 106, 42, WHITE); // right-pointing (increase)

  // Boxed number
  display.drawRoundRect(34, 20, 60, 28, 4, WHITE);
  display.setTextSize(3);
  char buf[8];
  sprintf(buf, "%d", customMinutes);
  int numW = strlen(buf) * 18;
  int numX = 34 + (60 - numW) / 2;
  display.setCursor(numX, 26);
  display.print(buf);

  // "minutes" label
  display.setTextSize(1);
  int lblX = (128 - strlen("minutes") * 6) / 2;
  display.setCursor(lblX, 51);
  display.print("minutes");

  

  display.display();
}

void drawConfirmScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(20, 2);
  display.print("Start Session?");
  display.drawLine(0, 12, 127, 12, WHITE);

  display.setTextSize(2);
  display.setCursor(20, 18);
  if (focusIndex == focusCount - 1) {
    char buf[16];
    sprintf(buf, "%d Minutes", customMinutes);
    display.print(buf);
  } else {
    display.print(focusOptions[focusIndex]);
  }

  display.setTextSize(1);

  if (confirmIndex == 0) {
    display.fillRect(12, 50, 40, 10, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }
  display.setCursor(22, 52);
  display.print("YES");

  if (confirmIndex == 1) {
    display.fillRect(76, 50, 40, 10, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }
  display.setCursor(90, 52);
  display.print("NO");

  display.display();
}

void drawTimerRunning(long remainingMs)
{
  int totalSec = remainingMs / 1000;
  int mm = totalSec / 60;
  int ss = totalSec % 60;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  int titleX = (128 - strlen("Focus Session") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("Focus Session");
  display.drawLine(0, 12, 127, 12, WHITE);

  char buf[8];
  sprintf(buf, "%02d:%02d", mm, ss);
  display.setTextSize(3);
  int x = (128 - strlen(buf) * 18) / 2;
  display.setCursor(x, 24);
  display.print(buf);

  long pct = 100 - (remainingMs * 100 / (focusDurationMs > 0 ? focusDurationMs : 1));
  pct = constrain(pct, 0, 100);
  display.drawRect(14, 52, 100, 8, WHITE);
  display.fillRect(14, 52, map(pct, 0, 100, 0, 100), 8, WHITE);

  display.display();
}

void drawTimerDone()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Happy face (shrunk slightly to leave room for the points line)
  drawFace(64, 16, 12, true);

  display.setTextSize(1);
  int titleX = (128 - (int)strlen("Great Job!") * 6) / 2;
  display.setCursor(titleX, 32);
  display.print("Great Job!");

  char ptsBuf[16];
  sprintf(ptsBuf, "+%d Points!", settingPointsPerSession);
  int ptsX = (128 - (int)strlen(ptsBuf) * 6) / 2;
  display.setCursor(ptsX, 42);
  display.print(ptsBuf);

  int l1X = (128 - (int)strlen("Games & Music") * 6) / 2;
  display.setCursor(l1X, 52);
  display.print("Games & Music");

  display.display();
}

void drawSessionStopped()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Sad face (shrunk slightly to leave room for the points line)
  drawFace(64, 14, 12, false);

  display.setTextSize(1);
  int l1X = (128 - (int)strlen("Session Stopped") * 6) / 2;
  display.setCursor(l1X, 30);
  display.print("Session Stopped");

  char ptsBuf[16];
  sprintf(ptsBuf, "-%d Points", settingPointsPerSession);
  int ptsX = (128 - (int)strlen(ptsBuf) * 6) / 2;
  display.setCursor(ptsX, 40);
  display.print(ptsBuf);

  int l2X = (128 - (int)strlen("Try again later") * 6) / 2;
  display.setCursor(l2X, 50);
  display.print("Try again later");

  display.display();
}

void drawLockedMsg()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  int titleX = (128 - strlen("Locked!") * 12) / 2;
  display.setCursor(titleX, 12);
  display.print("Locked!");

  display.setTextSize(1);
  int l1X = (128 - strlen("Complete a focus") * 6) / 2;
  display.setCursor(l1X, 38);
  display.print("Complete a focus");
  int l2X = (128 - strlen("session first") * 6) / 2;
  display.setCursor(l2X, 48);
  display.print("session first");

  display.display();
}

void drawBreakOver()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Sad face — break's over, back to work
  drawFace(64, 18, 14, false);

  display.setTextSize(1);
  int l1X = (128 - (int)strlen("Break Over!") * 6) / 2;
  display.setCursor(l1X, 40);
  display.print("Break Over!");

  int l2X = (128 - (int)strlen("Games Locked") * 6) / 2;
  display.setCursor(l2X, 50);
  display.print("Games Locked");

  display.display();
}

void drawVendDispense()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  drawFace(64, 18, 14, true);

  display.setTextSize(1);
  int l1X = (128 - (int)strlen("Enjoy!") * 6) / 2;
  display.setCursor(l1X, 40);
  display.print("Enjoy!");

  char buf[16];
  sprintf(buf, "-%d Points", settingPointsPerVend);
  int l2X = (128 - (int)strlen(buf) * 6) / 2;
  display.setCursor(l2X, 50);
  display.print(buf);

  display.display();
}

void drawVendLocked()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  int titleX = (128 - (int)strlen("Locked!") * 12) / 2;
  display.setCursor(titleX, 8);
  display.print("Locked!");

  display.setTextSize(1);
  char buf[24];
  sprintf(buf, "Need %d points", settingPointsPerVend);
  int l1X = (128 - (int)strlen(buf) * 6) / 2;
  display.setCursor(l1X, 36);
  display.print(buf);

  char buf2[24];
  sprintf(buf2, "You have %d", focusPoints);
  int l2X = (128 - (int)strlen(buf2) * 6) / 2;
  display.setCursor(l2X, 46);
  display.print(buf2);

  display.display();
}

void drawGamesMenu()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(2, 2);
  display.print("<");
  int titleX = (128 - strlen("Games") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("Games");
  display.drawLine(0, 14, 127, 14, WHITE);

  for (int i = 0; i < gameCount; i++)
  {
    int y = 18 + i * 14;
    if (i == gameMenuIndex) {
      display.fillRect(0, y - 1, 128, 12, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }
    display.setCursor(8, y);
    display.print(gameItems[i]);
  }

  display.setTextColor(WHITE);
  drawBreakBadge(90, 2); // countdown in the free space right of the title

  display.display();
}

// ================================================================
// MUSIC — "Happy Birthday" on the passive buzzer, non-blocking playback
// ================================================================
#include "pitch.h"

// "Happy Birthday to You" is public domain in the US as of 2016 (its
// copyright claim was struck down in court), so this melody is safe to
// ship — no licensing concerns.
//
// Each duration number is a standard "note division": 8 = eighth note,
// 4 = quarter note, 2 = half note. Actual ms length is derived from the
// tempo below at playback time (see startMusicSong).

const char *musicSongName = "Happy Birthday";
const int musicTempo = 100; // beats per minute

const int musicMelody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};
const int musicDurations[] = {
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2
};
const int musicLength = sizeof(musicMelody) / sizeof(musicMelody[0]);

bool musicPlaying = false;
int musicNoteIndex = 0;
unsigned long musicNextNoteAt = 0;
unsigned long musicWholeNoteMs = 0;

// Starts playback and switches to the "Now Playing" screen. Plays the
// very first note immediately so there's no blank beat at the start.
void startMusicSong()
{
  musicNoteIndex = 0;
  musicPlaying = true;
  musicWholeNoteMs = (60000UL * 4) / musicTempo;
  musicNextNoteAt = millis();

  currentScreen = SCR_MUSIC_PLAYING;
  updateMusicPlayback();
}

// Silences the buzzer and clears playback state. Safe to call any time,
// even if nothing is currently playing.
void stopMusicPlayback()
{
  noTone(BUZZER);
  musicPlaying = false;
}

// Non-blocking melody sequencer — called every loop() while
// currentScreen == SCR_MUSIC_PLAYING. Advances to the next note only once
// the current note's duration has elapsed, using tone()'s own built-in
// duration so the buzzer doesn't need to be polled in between.
void updateMusicPlayback()
{
  if (!musicPlaying) return;

  unsigned long now = millis();
  if (now < musicNextNoteAt) return;

  if (musicNoteIndex >= musicLength) {
    musicPlaying = false;
    noTone(BUZZER);
    beep(1568, 120); // little "ta-da" to mark the end
    drawMusicPlaying();
    return;
  }

  int freq = musicMelody[musicNoteIndex];
  int durCode = musicDurations[musicNoteIndex];
  int noteDuration = musicWholeNoteMs / durCode;

  if (settingSoundOn && freq > 0) {
    tone(BUZZER, freq, (int)(noteDuration * 0.85)); // slight gap = articulation
  }

  musicNoteIndex++;
  musicNextNoteAt = now + noteDuration;

  drawMusicPlaying();
}

// Small animated eighth-note icon. Bobs up and down between notes so the
// screen visibly "moves" even though nothing else on it changes much.
void drawMusicNoteIcon(int x, int y)
{
  display.fillCircle(x, y, 4, WHITE);
  display.drawLine(x + 4, y, x + 4, y - 16, WHITE);
  display.drawLine(x + 4, y - 16, x + 9, y - 12, WHITE);
  display.drawLine(x + 4, y - 13, x + 9, y - 9, WHITE);
}

void drawMusicPlaying()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  int titleX = (128 - (int)strlen(musicSongName) * 6) / 2;
  display.setCursor(titleX, 2);
  display.print(musicSongName);
  display.drawLine(0, 12, 127, 12, WHITE);

  int bounce = (musicNoteIndex % 2 == 0) ? 0 : 3;
  drawMusicNoteIcon(58, 34 - bounce);

  int pct = (musicLength > 0) ? (musicNoteIndex * 100) / musicLength : 0;
  pct = constrain(pct, 0, 100);
  display.drawRect(14, 46, 100, 7, WHITE);
  display.fillRect(14, 46, map(pct, 0, 100, 0, 100), 7, WHITE);

  display.setTextColor(WHITE);
  if (musicPlaying) {
    int l1X = (128 - (int)strlen("Click=Stop") * 6) / 2;
    display.setCursor(l1X, 56);
    display.print("Click=Stop");
  } else {
    display.setCursor(2, 56);
    display.print("Finished! Click=Back");
  }

  display.display();
}

void drawMusicMutedNotice()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  int l1X = (128 - (int)strlen("Sound is Off") * 6) / 2;
  display.setCursor(l1X, 26);
  display.print("Sound is Off");

  int l2X = (128 - (int)strlen("Enable in Settings") * 6) / 2;
  display.setCursor(l2X, 36);
  display.print("Enable in Settings");

  display.display();
}

void drawMusicScreen()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  int titleX = (128 - strlen("Music") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("Music");
  display.drawLine(0, 12, 127, 12, WHITE);

  drawMusicNoteIcon(58, 38);

  int l1X = (128 - (int)strlen(musicSongName) * 6) / 2;
  display.setCursor(l1X, 44);
  display.print(musicSongName);

  if (!settingSoundOn) {
    int l2X = (128 - (int)strlen("Sound is Off!") * 6) / 2;
    display.setCursor(l2X, 54);
    display.print("Sound is Off!");
  } else {
    int l2X = (128 - (int)strlen("Click to Play") * 6) / 2;
    display.setCursor(l2X, 54);
    display.print("Click to Play");
  }

  drawBreakBadge(90, 2); // countdown in the free space right of the title

  display.display();
}

// ================================================================
// STATISTICS SCREEN
// ================================================================
void drawStatistics()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.setCursor(2, 2);
  display.print("<");
  int titleX = (128 - (int)strlen("Statistics") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("Statistics");
  display.drawLine(0, 12, 127, 12, WHITE);

  char buf[24];

  sprintf(buf, "Sessions: %d", totalSessions);
  display.setCursor(6, 16);
  display.print(buf);

  sprintf(buf, "Focus Min: %ld", totalFocusMinutes);
  display.setCursor(6, 26);
  display.print(buf);

  sprintf(buf, "Best Snake: %d", bestSnakeScore);
  display.setCursor(6, 36);
  display.print(buf);

  sprintf(buf, "Best Simon: %d", bestSimonScore);
  display.setCursor(6, 46);
  display.print(buf);

  display.display();
}

// ================================================================
// SETTINGS SCREENS
// ================================================================
void drawSettingsMenu()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(2, 2);
  display.print("<");
  int titleX = (128 - strlen("Settings") * 6) / 2;
  display.setCursor(titleX, 2);
  display.print("Settings");
  display.drawLine(0, 14, 127, 14, WHITE);

  for (int i = 0; i < settingsCount; i++)
  {
    int y = 18 + i * 11;
    char line[24];

    if (i == 0) {
      sprintf(line, "%s: %d", settingsItems[i], settingBreakMinutes);
    } else if (i == 1) {
      sprintf(line, "%s: %d", settingsItems[i], settingPointsPerSession);
    } else if (i == 2) {
      sprintf(line, "%s: %d", settingsItems[i], settingPointsPerVend);
    } else {
      sprintf(line, "%s: %s", settingsItems[i], settingSoundOn ? "On" : "Off");
    }

    if (i == settingsIndex) {
      display.fillRect(0, y - 1, 128, 10, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }
    display.setCursor(8, y);
    display.print(line);
  }

  display.display();
}

void drawSettingsEdit()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(2, 2);
  display.print("<");
  const char *label = settingsItems[settingsEditingIndex];
  int titleX = (128 - (int)strlen(label) * 6) / 2;
  display.setCursor(titleX, 2);
  display.print(label);
  display.drawLine(0, 13, 127, 13, WHITE);

  // Down arrow (left) + Up arrow (right) either side of the number box
  display.fillTriangle(14, 34, 22, 26, 22, 42, WHITE); // decrease
  display.fillTriangle(114, 34, 106, 26, 106, 42, WHITE); // increase

  display.drawRoundRect(34, 20, 60, 28, 4, WHITE);
  display.setTextSize(3);

  int val = 0;
  if (settingsEditingIndex == 0) val = settingBreakMinutes;
  else if (settingsEditingIndex == 1) val = settingPointsPerSession;
  else if (settingsEditingIndex == 2) val = settingPointsPerVend;

  char buf[8];
  sprintf(buf, "%d", val);
  int numW = strlen(buf) * 18;
  int numX = 34 + (60 - numW) / 2;
  display.setCursor(numX, 26);
  display.print(buf);

  display.setTextSize(1);
  const char *unit = (settingsEditingIndex == 0) ? "minutes" : "points";
  int lblX = (128 - (int)strlen(unit) * 6) / 2;
  display.setCursor(lblX, 51);
  display.print(unit);

  display.display();
}

// ================================================================
// SNAKE
// ================================================================
bool isSnakeCell(int gx, int gy)
{
  for (int i = 0; i < snakeLen; i++) {
    if (snakeX[i] == gx && snakeY[i] == gy) return true;
  }
  return false;
}

void placeSnakeFood()
{
  do {
    foodX = random(0, SNAKE_COLS);
    foodY = random(0, SNAKE_ROWS);
  } while (isSnakeCell(foodX, foodY));
}

void initSnakeGame()
{
  snakeLen = 3;
  int startCol = SNAKE_COLS / 2;
  int startRow = SNAKE_ROWS / 2;
  for (int i = 0; i < snakeLen; i++) {
    snakeX[i] = startCol - i;
    snakeY[i] = startRow;
  }
  snakeDirX = 1;
  snakeDirY = 0;
  snakeScore = 0;
  snakeGameOver = false;
  snakeTurnedThisTick = false;
  placeSnakeFood();
  snakeLastMove = millis();
  drawSnakeGame();
}

// Ends the run: plays the game-over tone, updates + persists the best
// score if this run beat it, then draws the game-over overlay.
void endSnakeGame()
{
  snakeGameOver = true;
  beep(220, 300);

  if (snakeScore > bestSnakeScore) {
    bestSnakeScore = snakeScore;
    saveStats();
  }

  drawSnakeGame();
}

void updateSnakeGame()
{
  if (snakeGameOver) return;

  unsigned long now = millis();
  if (now - snakeLastMove < SNAKE_MOVE_INTERVAL) return;
  snakeLastMove = now;
  snakeTurnedThisTick = false; // a new tick starts — allow one fresh turn again

  int newX = snakeX[0] + snakeDirX;
  int newY = snakeY[0] + snakeDirY;

  if (newX < 0 || newX >= SNAKE_COLS || newY < 0 || newY >= SNAKE_ROWS) {
    endSnakeGame();
    return;
  }

  if (isSnakeCell(newX, newY)) {
    endSnakeGame();
    return;
  }

  bool grew = false;
  if (newX == foodX && newY == foodY) {
    grew = true;
    snakeScore++;
    beep(1200, 60);
  }

  int growLen = grew ? snakeLen + 1 : snakeLen;
  if (growLen > SNAKE_MAX_LEN) growLen = SNAKE_MAX_LEN;

  for (int i = growLen - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  snakeX[0] = newX;
  snakeY[0] = newY;
  snakeLen = growLen;

  if (grew) placeSnakeFood();

  drawSnakeGame();
}

void drawSnakeGame()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.setCursor(2, 2);
  display.print("Snake");

  char buf[16];
  sprintf(buf, "Score:%d", snakeScore);
  display.setCursor(128 - (int)strlen(buf) * 6 - 2, 2);
  display.print(buf);

  display.drawLine(0, 12, 127, 12, WHITE);

  display.fillRect(foodX * SNAKE_CELL, SNAKE_TOP_Y + foodY * SNAKE_CELL, SNAKE_CELL - 1, SNAKE_CELL - 1, WHITE);

  for (int i = 0; i < snakeLen; i++) {
    display.fillRect(snakeX[i] * SNAKE_CELL, SNAKE_TOP_Y + snakeY[i] * SNAKE_CELL, SNAKE_CELL - 1, SNAKE_CELL - 1, WHITE);
  }

  if (snakeGameOver) {
    display.fillRect(14, 22, 100, 24, BLACK);
    display.drawRect(14, 22, 100, 24, WHITE);
    display.setCursor(30, 27);
    display.print("Game Over");
    display.setCursor(20, 37);
    display.print("Click=Retry");
  }

  display.display();
}

// ================================================================
// SIMON SAYS
// ================================================================
void addSimonStep()
{
  simonSeq[simonLen] = random(0, 4);
  simonLen++;
}

void initSimonGame()
{
  simonLen = 0;
  addSimonStep();
  simonState = SIMON_SHOWING;
  simonShowIndex = 0;
  simonFlashOn = false;
  simonNextActionAt = millis() + 300;
  drawSimonQuadrants(-1, false);
}

void drawSimonQuadrants(int index, bool filled)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  char buf[16];
  sprintf(buf, "Simon  Len:%d", simonLen);
  display.setCursor(2, 2);
  display.print(buf);
  display.drawLine(0, 12, 127, 12, WHITE);

  for (int i = 0; i < 4; i++) {
    if (i == index) {
      if (filled) {
        display.fillRect(simonQX[i], simonQY[i], simonQW, simonQH, WHITE);
      } else {
        display.drawRect(simonQX[i], simonQY[i], simonQW, simonQH, WHITE);
        display.drawRect(simonQX[i] + 3, simonQY[i] + 3, simonQW - 6, simonQH - 6, WHITE);
      }
    } else {
      display.drawRect(simonQX[i], simonQY[i], simonQW, simonQH, WHITE);
    }
  }

  display.display();
}

void updateSimonGame()
{
  if (simonState != SIMON_SHOWING) return;

  unsigned long now = millis();
  if (now < simonNextActionAt) return;

  if (!simonFlashOn) {
    drawSimonQuadrants(simonSeq[simonShowIndex], true);
    beep(500 + simonSeq[simonShowIndex] * 120, 150);
    simonFlashOn = true;
    simonNextActionAt = now + SIMON_FLASH_ON_MS;
  } else {
    drawSimonQuadrants(-1, false);
    simonFlashOn = false;
    simonShowIndex++;

    if (simonShowIndex >= simonLen) {
      simonState = SIMON_INPUT;
      simonInputIndex = 0;
      simonSelected = 0;
      drawSimonQuadrants(simonSelected, false);
    } else {
      simonNextActionAt = now + SIMON_FLASH_OFF_MS;
    }
  }
}

void handleSimonInput()
{
  if (simonSelected == simonSeq[simonInputIndex]) {
    simonInputIndex++;

    if (simonInputIndex >= simonLen) {
      // round complete
      if (simonLen < SIMON_MAX_LEN) {
        addSimonStep();
      }
      simonState = SIMON_SHOWING;
      simonShowIndex = 0;
      simonFlashOn = false;
      simonNextActionAt = millis() + 500;
      beep(1000, 150);
    } else {
      beep(700, 80);
    }
  } else {
    simonState = SIMON_GAMEOVER;
    beep(220, 400);

    int finalScore = simonLen - 1;
    if (finalScore > bestSimonScore) {
      bestSimonScore = finalScore;
      saveStats();
    }

    drawSimonGameOver();
  }
}

void drawSimonGameOver()
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  drawFace(64, 18, 14, false);

  display.setTextSize(1);
  int titleX = (128 - (int)strlen("Game Over") * 6) / 2;
  display.setCursor(titleX, 38);
  display.print("Game Over");

  char buf[16];
  sprintf(buf, "Score:%d", simonLen - 1);
  int sX = (128 - (int)strlen(buf) * 6) / 2;
  display.setCursor(sX, 48);
  display.print(buf);

  int rX = (128 - (int)strlen("Click=Retry") * 6) / 2;
  display.setCursor(rX, 57);
  display.print("Click=Retry");

  display.display();
}
