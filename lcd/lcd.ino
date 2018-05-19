#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);//create a lcd object (rs,e,d4,d5,d6,d7)

void setup() {
  lcd.begin(16,2);                    // columns, rows. size of display
  lcd.clear();                        // clear the screen
  lcd.setCursor(0,0);                 // set cursor to column 0, row 0 (first row)
  lcd.print("yasara jayaweera");       // input your text here
  lcd.setCursor(0,1);                 // move cursor down one
  lcd.print("*** ucsc ***");  

}

void loop() {
  lcd.setCursor(1,16); // set the cursor outside the display count
  lcd.autoscroll();    // set the display to automatically scroll:
  lcd.print(" ");      // print empty character
  delay(500);  

}
