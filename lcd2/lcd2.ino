/**
 * @author Yasara JLP
 * LCD Display
 * 
 */
 
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte newChar[8] = {
        B00000,
        B00100,
        B00100,
        B01010,
        B01010,
        B00100,
        B00100,
        B00000
};

void setup() {
    lcd.createChar(0, newChar);
    lcd.begin(16, 2);
    lcd.write(byte(0));
    lcd.autoscroll();
    lcd.setCursor(0,1); //(column,row) (zero based)
    lcd.print("x");
}

void loop() {}
