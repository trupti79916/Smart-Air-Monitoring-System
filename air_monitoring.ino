#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

DHT dht(D5, DHT11);

WiFiClient client;

int redled = D1;
int threshold =550 ;
int alert_time = 250;

long myChannelNumber = 1679303;
const char myWriteAPIKey[] = "XJS5OZIBV4YCJCA7";

const char *ssid =  "Redmi";     // Enter your WiFi Name
const char *pass =  "trupti123"; // Enter your WiFi Password


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  dht.begin();
  pinMode(redled, OUTPUT);
  ThingSpeak.begin(client);
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int sensor_value = analogRead(A0);
  
    
  Serial.print("Temperature: " + (String) t);
  Serial.println("°C");
  Serial.print("Humidity: " + (String) h);
  Serial.println("%");
  Serial.println("Air Quality : " + (String) sensor_value);

  if(sensor_value > threshold)
  {
    
    digitalWrite(redled,HIGH);
    delay(alert_time);
    digitalWrite(redled,LOW);
    delay(alert_time);
  
  }
  
  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, sensor_value, myWriteAPIKey);
  delay(2000);
}
