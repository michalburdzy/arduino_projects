#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

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

int counter = 60;
String weatherDescription = "";
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;

WiFiClient client;
char servername[] = "api.openweathermap.org"; // remote server we will connect to
String result;
const char *ssid = "";     // SSID of local network
const char *password = ""; // Password on network
char blunkAuthToken[] = "";
String openWeatherApiKey = "";

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
    terminal.print("\nEcho:");
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

  terminal.print("X:Y ");
  terminal.println(x);
  terminal.println(y);
  terminal.flush();
}

void setup()
{
  Serial.begin(115200);

  Blynk.begin(blunkAuthToken, ssid, password);
  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type 'Foo' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
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
  delay(1000);
}

void loop()
{
  Blynk.run();

  if (counter == 60) //Get new data every 10 minutes
  {
    counter = 0;
    displayGettingData();
    delay(1000);
    getWeatherData();
  }
  else
  {
    counter++;
    displayWeather(weatherLocation, weatherDescription);
    delay(5000);
    displayConditions(Temperature, Humidity, Pressure);
    delay(5000);
  }
}

void getWeatherData() //client function to send/receive GET request data.
{
  if (client.connect(servername, 80))
  { //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?q=Jelenia Góra&units=metric&APPID=" + openWeatherApiKey + "&lang=pl");
  }
  else
  {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available())
    delay(1); //waits for data
  while (client.connected() || client.available())
  {                         //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    result = result + c;
  }

  client.stop(); //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);

  char jsonArray[result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';

  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

  String location = root["name"];
  String country = root["sys"]["country"];
  float temperature = root["main"]["temp"];
  float humidity = root["main"]["humidity"];
  String weather = root["weather"]["main"];
  String description = root["weather"]["description"];
  float pressure = root["main"]["pressure"];

  weatherDescription = description;
  weatherLocation = location;
  Country = country;
  Temperature = temperature;
  Humidity = humidity;
  Pressure = pressure;

  terminal.println(location);
  terminal.println(description);
  terminal.println(temperature);
  terminal.println("-----------");
  terminal.flush();
}

void displayWeather(String location, String description)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(location);
  lcd.setCursor(0, 1);
  lcd.print(description);
  delay(1000);
  if (description.length() > 16)
  {
    for (int positionCounter = 0; positionCounter <= description.length() - 16; positionCounter++)
    {
      lcd.scrollDisplayLeft();
      lcd.setCursor(positionCounter + 1, 0);
      lcd.print(location);
      delay(500);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(location);
    lcd.setCursor(0, 1);
    lcd.print(description);
  }
  else
  {
    lcd.print(description);
  }
}

void displayConditions(float Temperature, float Humidity, float Pressure)
{
  lcd.clear();
  lcd.print("Teraz: ");
  lcd.print(Temperature, 1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);

  //Printing Humidity
  lcd.print("H:");
  lcd.print(Humidity, 0);
  lcd.print("% ");

  //Printing Pressure
  lcd.print(Pressure, 1);
  lcd.print(" hPa");
}

void displayGettingData()
{
  lcd.clear();
  lcd.print("Pobieram dane...");
  terminal.println("Pobieram dane...");
  terminal.flush();
}
