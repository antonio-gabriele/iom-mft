#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include <EasyButton.h>
#include <ESP_EEPROM.h>

#define PIN_LED 5
#define PIN_BTN_STA 0
#define PIN_SCL 4
#define PIN_SDA 2
#define MAGIC_NUMBER 0xAA
#define MODE_WRK 0
#define MODE_PRG 1

EasyButton button(PIN_BTN_STA, 200);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

struct Config
{
  uint8_t magic;
  uint8_t durations[4];
  uint8_t cycle;

} config;

uint8_t mode = MODE_WRK;

void save()
{
  Serial.println(F("> Save."));
  EEPROM.put(0, config);
  EEPROM.commit();
  Serial.println(F("< Save."));
}

void load()
{
  Serial.println(F("> Load."));
  EEPROM.get(0, config);
  if (config.magic != MAGIC_NUMBER)
  {
    config.magic = MAGIC_NUMBER;
    config.cycle = config.durations[0] = config.durations[1] = config.durations[2] = config.durations[3] = 0;
    save();
  } 
  Serial.println(F("< Load."));
}

void display()
{
  lcd.clear();
  char buffer[16];
  sprintf(buffer, "M:%s C:%d T:%d", (mode == MODE_PRG ? "PRG" : "RUN"), (config.cycle + 1), config.durations[config.cycle]);
  lcd.setCursor(0, 0);
  lcd.print(buffer);
  sprintf(buffer, "Lavori in corso");
  lcd.setCursor(0, 1);
  lcd.print(buffer);
}

void onShortPressed()
{
  Serial.println(F("> Short."));
  if (mode == MODE_WRK)
  {
    config.cycle = (config.cycle + 1) % sizeof(config.durations);
  }
  else
  {
    config.durations[config.cycle] = (config.durations[config.cycle] + 5) % 100;
  }
  display();
  Serial.println(F("< Short."));
}

void onLongPressed()
{
  Serial.println(F("> Long."));
  if (mode == MODE_PRG)
  {
    mode = MODE_WRK;
    save();
  }
  else
  {
    mode = MODE_PRG;
  }
  display();
  Serial.println(F("< Long."));
}

void setup()
{
  delay(1000);
  Serial.begin(74880);
  pinMode(PIN_LED, OUTPUT);
  EEPROM.begin(sizeof(struct Config) + 1);
  button.begin();
  button.onPressed(onShortPressed);
  button.onPressedFor(4000, onLongPressed);
  while (lcd.begin(16, 2, LCD_5x8DOTS, PIN_SDA, PIN_SCL) != 1) // colums - 20, rows - 4
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);
  }
  lcd.backlight();
  lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
  load();
  display();
}

void loop()
{
  button.read();
  // delay(10);
}
