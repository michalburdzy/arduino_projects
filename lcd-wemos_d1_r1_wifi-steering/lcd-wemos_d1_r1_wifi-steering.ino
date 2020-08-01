#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

int redPin = D7;
int greenPin = D6;
int bluePin = D5;
int groundPin = D8;
int redLevel = HIGH;
int greenLevel = HIGH;
int blueLevel = HIGH;

byte topArrow[] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte downArrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100
};
byte leftArrow[] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};
byte rightArrow[] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};
byte topRightArrow[] = {
  B00000,
  B01111,
  B00011,
  B00101,
  B01001,
  B10000,
  B00000,
  B00000
};
byte topLeftArrow[] = {
  B00000,
  B11110,
  B11000,
  B10100,
  B10010,
  B00001,
  B00000,
  B00000
};
byte downRightArrow[] = {
  B00000,
  B00000,
  B10000,
  B01001,
  B00101,
  B00011,
  B01111,
  B00000
};
byte downLeftArrow[] = {
  B00000,
  B00000,
  B00001,
  B10010,
  B10100,
  B11000,
  B11110,
  B00000
};
byte centerArrow[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000
};

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
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

WiFiClient client;
const char *ssid = "";     // SSID of local network
const char *password = ""; // Password on network
char blunkAuthToken[] = "";

WidgetTerminal terminal(V1);
BLYNK_WRITE(V1)
{
  if (String("Foo") == param.asStr())
  {
    terminal.println("You said: 'Foo'?");
    terminal.println("I said: 'Bar'");
  }
  else
  {
    terminal.print("\nEcho: ");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  terminal.flush();
}

// joystick
BLYNK_WRITE(V3)
{
  int x = param[0].asInt();
  int y = param[1].asInt();

  move(x,y);
}

void move(int x, int y){
   lcd.setCursor(0, 1);
   lcd.print("x, y ");
   lcd.print(x);
  int margin = 150;
    if(x > 512 + margin){
      if(y > 512 + margin){
          printChar(topRightArrow);
          lcd.print("top right");
      } else if (y < 512 - margin) {
          printChar(downRightArrow);
           lcd.setCursor(0, 1);
           lcd.print("down right");
      } else {
          printChar(rightArrow);
           lcd.setCursor(0, 1);
           lcd.print("right");
      }
    } else if (x < 512 - margin){
       if(y > 512 + margin){
         printChar(topLeftArrow);
           lcd.setCursor(0, 1);
           lcd.print("top left");
      } else if (y < 512 - margin) {
         printChar(downLeftArrow);
           lcd.setCursor(0, 1);
           lcd.print("down right");
      } else {
         printChar(leftArrow);
           lcd.setCursor(0, 1);
           lcd.print("left");
      }
    } else if (y < 512 - margin) {
        printChar(downArrow);
           lcd.setCursor(0, 1);
           lcd.print("down");
      } else if (y > 512 + margin) {
        printChar(topArrow);
           lcd.setCursor(0, 1);
           lcd.print("top");
      } else {
        printChar(centerArrow);  
    }
}

void printChar(byte character[]){
  lcd.clear();
  lcd.setCursor(0, 0);
lcd.createChar(0, character);
  lcd.home();
  lcd.write(0);
}

void setup()
{
  Serial.begin(19200);
  Serial.println("hello");
  Blynk.begin(blunkAuthToken, ssid, password);
  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.flush();
  
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Dzien Dobry!");
  terminal.println("Dzien Dobry!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Laczenie...");
  terminal.println("Laczenie...");
  terminal.flush();
  lcd.setCursor(0, 1);
  int cursorPosition = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    lcd.print(".");
    cursorPosition++;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Polaczono! :)");
  terminal.println("Polaczono!");
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(groundPin, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("Piny gotowe! :)");

  digitalWrite(redPin, redLevel);
  digitalWrite(greenPin, greenLevel);
  digitalWrite(bluePin, blueLevel);
  digitalWrite(groundPin, HIGH);  
  lcd.setCursor(0, 0);
  lcd.print("Jest moc! :)");
  terminal.flush();
 }

void loop()
{
    Blynk.run();
}
