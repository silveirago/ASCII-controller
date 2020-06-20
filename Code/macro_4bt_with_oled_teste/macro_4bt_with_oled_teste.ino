#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#include <Keyboard.h>
int buttonPin1 = 7;

int buttonPin2 = 6;

int buttonPin3 = 5;

int buttonPin4 = 18;// Set a button to any pin

void setup() {

  pinMode(buttonPin1, INPUT_PULLUP);  
  pinMode(buttonPin2, INPUT_PULLUP); 
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 31, WHITE);
  display.drawPixel(127, 31, WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("4 N D R E S O L I S");
  display.setCursor(4,20);
  display.print("             DESIGN");
  
  

  // update display with all of the above graphics
  display.display();
}

void loop()
{
  if (digitalRead(buttonPin1) == 0)  // if the button goes low
  {
    Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('z'); 
  delay(100);
  Keyboard.releaseAll();
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("Ctrl Z");
   display.display();
    delay(1200);
    
  }
  if (digitalRead(buttonPin2) == 0)  // if the button goes low
  {
    Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('c'); 
  delay(100);
  Keyboard.releaseAll();    // send 
  display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("Ctrl C");
   display.display();
    delay(1200);
    
  }
  if (digitalRead(buttonPin3) == 0)  // if the button goes low
  {
    Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('v'); 
  delay(100);
  Keyboard.releaseAll();    // send 
  display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("Ctrl V");
   display.display();
    delay(1200);
 
  }
  if (digitalRead(buttonPin4) == 0)  // if the button goes low
  {
    Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('8'); 
  delay(100);
  Keyboard.releaseAll();    // send 
  display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("Ctrl 8 - Normal");
   display.display();
    delay(1200);
    
  }
  else {
    display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("4 N D R E S O L I S");
  display.setCursor(4,20);
  display.print("             DESIGN");
  display.display();
  }
}
