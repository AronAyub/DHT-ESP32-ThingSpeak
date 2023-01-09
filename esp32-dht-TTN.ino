#include "DHT.h"
#include "Arduino.h"
#include "WiFi.h"
#include "ThingSpeak.h"

WiFiClient client;

#define WIFI_SSID "use your wifi ID"
#define WIFI_PASSWORD "Your Wifi Password"


//#define TS_CHANNEL_ID your_thingspeak_channel_id

unsigned long channelnumber = Yourchannelnumber; 
#define TS_WRITE_API_KEY "Your channel API Key"

#define DHT_PIN 22
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.writeFields(channelnumber, TS_WRITE_API_KEY);
  Serial.println("Data sent to Thingspeak");

  delay(30000);  // send data every 30 seconds
}