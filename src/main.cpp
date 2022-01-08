/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

#define PIN_LED 5
#define PIN_SCL 4
#define PIN_SDA 2

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);

  while (lcd.begin(16, 2, LCD_5x8DOTS, PIN_SDA, PIN_SCL) != 1) // colums - 20, rows - 4
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);
  }

  lcd.backlight();
  lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
  delay(2000);

  lcd.clear();

  /* prints static text */
  lcd.setCursor(0, 0); // set 1-st colum & 2-nd row, 1-st colum & row started at zero
  lcd.print(F("Ciclo: 10"));

  lcd.setCursor(0, 1);
  lcd.print(F("Random number:"));
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(PIN_LED, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(PIN_LED, LOW);
  // wait for a second
  delay(1000);
  /* print dynamic text */
  lcd.setCursor(14, 1);
  lcd.print(random(10, 1000));
}
