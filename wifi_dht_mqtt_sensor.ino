
 /* WiFi DHT Sensor with MQTT and Deep-sleep
 *
 * Connects to wifi and reads from the onboard sensor, sleeps for $TIMEOUT seconds and repeats
 *
 * Physical connections needed:
 * D0 to RST to enable deepsleep
 * DHT connections to 5V, GROUND and $DHTPIN
 *
 * If you cant reprogram as the ESP is sleeping, disconnect D0 - RST and try again
 */


#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "<Your SSID>"
#define WIFI_PASSWORD "<Your WiFi password>"

#define USERNAME "<MQTT Username ('homeassistant' for HASS)"
#define PASSWORD "<MQTT Password (same as HTTP for HASS)"

#define TIMEOUT 10 //Time between polls
#define WIFI_RETRY 5 //Time until trying again if wifi fails

// DHT
#define DHTPIN 2
#define DHTTYPE DHT22

char mqttServer[] = "192.168.0.119"; //Adress of the MQTTservice

WiFiClient wifiClient;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  client.setServer( mqttServer, 1883 );
  connect();
  getAndSendTemperatureAndHumidityData();
  Serial.println("Entering deepsleep.");
  ESP.deepSleep(TIMEOUT * 1000000);
}

void loop()
{}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  String temperature = String(t);
  String humidity = String(h);


  // Just debug messages
  Serial.print( "Sending temperature and humidity : [" );
  Serial.print( temperature ); Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperature\":"; payload += temperature; payload += ",";
  payload += "\"humidity\":"; payload += humidity;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "home/balcony/temperature", attributes );
  delay(500);
  Serial.println( attributes );

}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void connect() {
  int tries = 0;
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED && tries < 20) {
      delay(500);
      Serial.print(".");
      tries++;
    }
    Serial.println("Connected to AP");
  }
  Serial.print("Connecting to MQTT node ...");
  // Attempt to connect (clientId, username, password)
  if ( client.connect("ESP8266 Device", USERNAME, PASSWORD) ) {
    Serial.println( "[DONE]" );
  } else {
    Serial.print( "[FAILED] [ rc = " );
    Serial.print( client.state() );
    Serial.println( "Entering deepsleep." );
    ESP.deepSleep(WIFI_RETRY * 1000000);
  }
}