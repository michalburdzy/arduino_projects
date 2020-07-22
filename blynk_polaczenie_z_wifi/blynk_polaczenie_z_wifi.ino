#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "x_d6mt31XiwC9WfGmEB98MqUN1x3EU2u";
char ssid[] = "dej_mie_te_internety";
char pass[] = "2020nowehaslo";

WidgetTerminal terminal(V1);
BLYNK_WRITE(V1)
{
  if (String("Foo") == param.asStr()) {
    terminal.println("You said: 'Foo'") ;
    terminal.println("I said: 'Bar'") ;
  } else {
    terminal.print("\nYou said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  terminal.flush();
}

BLYNK_WRITE(V3) {
  int x = param[0].asInt();
  int y = param[1].asInt();

  terminal.print("X:Y ");
  terminal.println(x);
  terminal.println(y);
  terminal.flush();
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  terminal.clear();

  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type 'Marco' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
  terminal.flush();
}

void loop()
{
  Blynk.run();
}
