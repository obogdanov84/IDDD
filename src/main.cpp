
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Melopero_RV3028.h>
#include "Keypad.h"
#include <I2C_eeprom.h>

Melopero_RV3028 rtc;

//-------------Screen init and defines 128x32-----------------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define CHAR_HEIGHT_1 8 // pixel height of font size 1 in screen
#define CHAR_WIDTH_1 6 // pixel width of font size 1 in screen


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int xPosition = 0;
int yPosition = 0;
int moveSpeed = 1;
int moveDirection = 1;

void displayTime(int xPos, int yPos, int hour, int minutes, int seconds);
void keypadMoveStep(void);

//-------------EEPROM 2LC64 init and defines-----------------
//for hex display uncomment below two lines, eeprom
#define DISPLAY_HEX
#define BLOCK_TO_LENGTH 16

#define MEMORY_SIZE 0x2000 //total bytes can be accessed 24LC64 = 0x2000 (maximum address = 0x1FFF)

I2C_eeprom ee(0x50, MEMORY_SIZE);

uint32_t start, diff, totals = 0;



//-------------Keypab 4x4 init and defines-----------------

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'4','8','C','G'},
  {'3','7','B','F'},
  {'2','6','A','E'},
  {'1','5','9','D'}
};

byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {15, 14, 16, 10}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int keyPress = 0;

void setup() {
  Serial.begin(9600);

  ee.begin();


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);

   // Clear the buffer
  display.clearDisplay();
  
  // First initialize and create the rtc device
  rtc.initDevice();
 
  // Set the device to use the 24hour format (default) instead of the 12 hour format
  rtc.set24HourMode();

  // Set the date and time:
  rtc.setTime(2020, 9, 3, 30, 15, 20, 0);
  // year, month, weekday, date, hour, minute, second
  // Note: time is always set in 24h format
  // Note: month value ranges from 1 (Jan) to 12 (Dec)
  // Note: date value ranges from 1 to 31



}
void loop() {


keypadMoveStep();

displayTime(xPosition,yPosition,rtc.getHour(),rtc.getMinute(),rtc.getSecond());

delay(10);

}


void displayTime(int xPos, int yPos, int hour, int minutes, int seconds)
{
    display.clearDisplay();
    // set up text properites
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true);
    
    //set positions and what to display, based on char width/height with a right justified positions and a padding 0
    if(hour<10)
    {
      display.setCursor(xPos,yPos); 
      display.print("0");
      display.setCursor(xPos+CHAR_WIDTH_1,yPos);
    }
    else
    {
      display.setCursor(xPos,yPos);
    }
            // Use full 256 char 'Code Page 437' font
    display.print(hour);

    display.setCursor(xPos+(CHAR_WIDTH_1*2),yPos);
    display.print(":");

    if(minutes<10)
    {
      display.setCursor(xPos+(CHAR_WIDTH_1*3),yPos); 
      display.print("0");
      display.setCursor(xPos+(CHAR_WIDTH_1*4),yPos);
    }
    else
    {
      display.setCursor(xPos+(CHAR_WIDTH_1*3),yPos);
    }
    display.print(minutes);
    
    display.setCursor(xPos+(CHAR_WIDTH_1*5),yPos);
    display.print(":");

    if(seconds<10)
    {
      display.setCursor(xPos+(CHAR_WIDTH_1*6),yPos); 
      display.print("0");
      display.setCursor(xPos+(CHAR_WIDTH_1*7),yPos);
    }
    else
    {
      display.setCursor(xPos+(CHAR_WIDTH_1*6),yPos);
    }
    display.print(seconds);

    display.display();
    delay(10);
}

void keypadMoveStep(void)
{
  switch (customKeypad.getKey())
{
  case('7'): //up
  
    xPosition+=4;
    break;

  case('5'): // down
    xPosition-=4;
    break;

  case('2'): //left
    yPosition-=4;
    break;

  case('A'): //right
    yPosition+=4;
    break;
}
}

