#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// pins connections: 
// LCD | Arduino
// GND -> GND
// VCC -> 5V
// SDA -> SDA
// SCL -> SCL

#define I2C_ADDR 0x27 // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup() {
  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  
  lcd.print("Good Morning!");
}

void loop() {
}
