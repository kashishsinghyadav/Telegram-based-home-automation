#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>

#include <UniversalTelegramBot.h>

#include <DHT.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "OakOLED.h"
OakOLED oled;
#define DHTPIN D0

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define relay1 D5
#define relay2 D6
#define relay3 D7
#define relay4 D8

#define PIR D4

bool pirbutton=0;
// Initialize WiFi connection

char ssid[] = "Prachi19"; // your network SSID (name)
char password[] = "11111111"; // your network password

// Initialize Telegram BOT

#define BOTtoken "5545196864:AAG4CNzWl6Wwq3j73gZEEDxFTeMk0cO2KEs" // your telegram bot token

WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 1 second.

int botRequestDelay = 1000;

unsigned long lastTimeBotRan;

void handleNewMessages(int numNewMessages) {

  Serial.println("handleNewMessages");

  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = String(bot.messages[i].chat_id);

    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;

    // Your Chat ID get it from here: https://t.me/chatidx_bot

    if (chat_id == "1322189103" || chat_id=="5174212592") {


      if (text == "/temperature") {

        int t = dht.readTemperature();

        String temp = "Temperature : ";

        temp += int(t);

        Serial.println(from_name);

        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/humidity") {

        int h = dht.readHumidity();

        String temp = "Humidity: ";

        temp += int(h);

        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Motion ") {
        if (pirbutton == 1) {
        bool value = digitalRead(PIR);
        if (value == 0) {
          bot.sendMessage(chat_id, "No Motion Detected", "");
          }
        else if (value == 1) {
          bot.sendMessage(chat_id, "Motion Detected", "");
        }
        }

      if (text == "/AllRelayON") {

        String temp = "AllRelayON: ";
        digitalWrite(D5,LOW);
        digitalWrite(D6,LOW);
        digitalWrite(D7,LOW);
        digitalWrite(D8,LOW);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/AllRelayOFF") {

        String temp = "AllRelayOFF: ";
        digitalWrite(D5,HIGH);
        digitalWrite(D6,HIGH);
        digitalWrite(D7,HIGH);
        digitalWrite(D8,HIGH);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Relay1ON") {

        String temp = "Relay1_ON: ";
        digitalWrite(D8,LOW);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Relay1OFF") {

        String temp = "Relay1_OFF: ";
        digitalWrite(D8,HIGH);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Relay2ON") {

        String temp = "Relay2 ON: ";
        digitalWrite(relay2,LOW);
        bot.sendMessage(chat_id, temp, "");

      }
      if (text == "/Relay2OFF") {

        String temp = "Relay2 OFF: ";
        digitalWrite(relay2,HIGH);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Relay3ON") {

        String temp = "Relay3 ON: ";
        digitalWrite(relay3,LOW);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Relay3OFF") {

        String temp = "Relay3 OFF: ";
        digitalWrite(relay3,HIGH);
        bot.sendMessage(chat_id, temp, "");

      }

      if (text == "/Relay4ON") {

        String temp = "Relay4 ON: ";
        digitalWrite(relay4,LOW);
        bot.sendMessage(chat_id, temp, "");

      }


      if (text == "/Relay4OFF") {

        String temp = "Relay4 OFF: ";
        digitalWrite(relay4,HIGH);
        bot.sendMessage(chat_id, temp, "");

      }

    } else {
      bot.sendMessage(chat_id, "Unauthorized User", "");
    }

    if (text == "/start") {

      String welcome = "Welcome  " + from_name + "\n\nThis bot is Built by @mcnaveen to find room temperature\n\n Choose your option\n";

      welcome += "/temperature : Get Temperature\n";

      welcome += "/humidity : Get Humiditiy\n";

      welcome += "/Motion : Detect Motion\n";

      welcome += "/AllRelayON: TURNING ON ALL RELAYS\n";
      
      welcome += "/AllRelayOFF : TURNING ON ALL RELAYS\n";

      welcome += "/Relay1ON : TURNING ON RELAY1\n";

      welcome += "/Relay1OFF : TURNING OFF RELAY1\n";

      welcome += "/Relay2ON : TURNING ON RELAY2\n";

      welcome += "/Relay2OFF : TURNING OFF RELAY2\n";

      welcome += "/Relay3ON : TURNING ON RELAY3\n";

      welcome += "/Relay3OFF : TURNING OFF RELAY3\n";
      
      welcome += "/Relay4ON : TURNING ON RELAY4\n";

      welcome += "/Relay4OFF : TURNING OFF RELAY4\n";
      

      

      

      bot.sendMessage(chat_id, welcome, "Markdown");

    }

  }
  }}



void setup() {

  Serial.begin(9600);

  dht.begin();
  pinMode(PIR,OUTPUT);
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
 
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  
  client.setInsecure();

  // WiFo Connected

  WiFi.mode(WIFI_STA);

  WiFi.disconnect();

  delay(100);

  // Attempt to connect to WiFi network:

  Serial.print("Connecting Wifi: ");

  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");

    delay(500);

oled.begin();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(1);
  oled.setCursor(0, 0);
  
  }

  Serial.println("");

  Serial.println("WiFi connected");

  Serial.print("IP address: ");

  Serial.println(WiFi.localIP());

}

 
void loop() {

  int t = dht.readTemperature();

  int h = dht.readHumidity();

  

  if (millis() > lastTimeBotRan + botRequestDelay) {

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {

      Serial.println("Got Response");

      handleNewMessages(numNewMessages);

      numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    }

    lastTimeBotRan = millis();

  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(1);
  oled.setCursor(0,0);
 oled.setCursor(0, 32);
  oled.print("HUMIDITY: ");
  oled.println(h);
  oled.print("TEMPERATURE: ");
  oled.println(t);
  
  oled.display();


}
