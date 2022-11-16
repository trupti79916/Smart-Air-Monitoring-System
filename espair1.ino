#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <EEPROM.h>//wifi library
#include "ThingSpeak.h"  //server library
#include <DHT.h> //dht 11 humidity & temp sensor 
#include <MQ135.h>

//const char *ssid =  "hitesh";     // Enter your WiFi Name
//const char *pass =  "hitesh123"; // Enter your WiFi Password

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiClient  client;   // post request 

long myChannelNumber = 1677233;
const char myWriteAPIKey[] = "VJHSHP407EBDI8B4";

void alert(void);

#define mq135_pin 35
#define mq4_pin 34

DHT dht(4, DHT11);

float Humidity;
float Temperature;

int mq135_value;
int mq135_threshold =550;

int mq4_value;
int mq4_threshold = 550;




void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  WiFi.begin("hitesh","hitesh123");
  Serial.print("Connecting to ");
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  dht.begin();
  ThingSpeak.begin(client);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  mq135_value = analogRead(mq135_pin)/10;
  mq4_value = analogRead(mq4_pin)/10;;

  
  Serial.print("Temperature: " + (String) Temperature);
  Serial.println("°C");
  Serial.print("Humidity: " + (String) Humidity);
  Serial.println("%");
  Serial.println("Air Quality: " + (String) mq135_value);
  display.clearDisplay();

   // display temperature
  display.setTextSize(1.5);
  display.setCursor(0,0);
  display.print(Temperature);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("C");
  
  // display humidity
  display.setTextSize(1.5);
  display.setCursor(50, 0);
  display.print(Humidity);
  display.print(" %"); 

  if(mq135_value>0&&mq135_value<=150){
    display.setTextSize(1.5);
    display.setCursor(0,25);
    display.print("Air Quality:");
    display.setTextSize(2);
    display.setCursor(0,35);
    display.print("Good");
  }
  else if(mq135_value>150 && mq135_value<=250)
  {
    display.setTextSize(1.5);
    display.setCursor(0,25);
    display.print("Air Quality");
    display.setTextSize(2);
    display.setCursor(0,35);
    display.print("Moderate");
  }
  else 
  {
    display.setTextSize(1.5);
    display.setCursor(0,25);
    display.print("Air Quality:");
    display.setTextSize(2);
    display.setCursor(0,35);
    display.print("Bad");
  }
  display.display(); 

  ThingSpeak.writeField(myChannelNumber, 1, Temperature, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, Humidity, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, mq135_value, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4, mq4_value, myWriteAPIKey);

  delay(2000);
}
