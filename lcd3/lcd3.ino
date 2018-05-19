// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(12,11,5,4,3,2);

void setup() {
  lcd.begin(16,2);                    // columns, rows. size of display
  lcd.clear();                        // clear the screen
  lcd.setCursor(0,0);                 // set cursor to column 0, row 0 (first row)
  lcd.print("yasara jayaweera");       // input your text here
  lcd.setCursor(0,1);                 // move cursor down one
  lcd.print("*** ucsc ***"); 
}

void loop() {
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(350);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(350);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(350);
  }

  // delay at the end of the full loop:
  delay(1000);



}
