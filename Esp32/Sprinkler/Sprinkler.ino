/** This example will show how to access the RTDB in Test Mode (no authentication).
 */
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

/* 2. Define the RTDB URL */
#define DATABASE_URL "YOUR_DATABASE_URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define RELAY_PIN 32
#define SENSOR_PIN 36

const int AirValue = 310;   //you need to replace this value with Value_1
const int WaterValue = 620;
int soilmoisturepercent=0;

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif
void sendReadings(){
  long int timestamp = timeClient.getEpochTime();
  int thresholdVal;
  int humidityVal = analogRead(SENSOR_PIN);
   soilmoisturepercent = map(humidityVal,4095,0,0,100); //value of the effective soil humidity percentage
  Firebase.RTDB.getInt(&fbdo, "/sensorData/threshold",&thresholdVal);
  Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, "/sensorData/humidity", soilmoisturepercent) ? "ok" : fbdo.errorReason().c_str());  
  Serial.println(soilmoisturepercent);
  if(thresholdVal>soilmoisturepercent){
    
    Serial.printf("Set string... %s\n", Firebase.RTDB.setInt(&fbdo, "/sensorData/time", timestamp) ? "ok" : fbdo.errorReason().c_str());
    digitalWrite(RELAY_PIN, LOW);
    delay(50000); // 50 seconds
    digitalWrite(RELAY_PIN, HIGH);
    Firebase.RTDB.setBool(&fbdo, "/sensorData/pumpStatus", 0);
  }
  Serial.printf("Set string... %s\n", Firebase.RTDB.setInt(&fbdo, "/sensorData/time", timestamp) ? "ok" : fbdo.errorReason().c_str());
  Firebase.RTDB.setBool(&fbdo, "/sensorData/readingStatus", 0);
  Firebase.RTDB.setString(&fbdo, "/sensorData/timeStamp", timeClient.getFormattedTime());
}

void setup()
{

    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the certificate file (optional) */
    // config.cert.file = "/cert.cer";
    // config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL(required) */
    config.database_url = DATABASE_URL;

    config.signer.test_mode = true;

    /**
     Set the database rules to allow public read and write.

       {
          "rules": {
              ".read": true,
              ".write": true
          }
        }

    */

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    config.wifi.clearAP();
    config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    // Or use legacy authenticate method
    // Firebase.begin(DATABASE_URL, DATABASE_SECRET);
    timeClient.begin();
}

void loop()
{
  digitalWrite(RELAY_PIN, HIGH);
  long int lastTimestamp;
  timeClient.update();
  
  Firebase.RTDB.getInt(&fbdo, "/sensorData/time",&lastTimestamp);
  Serial.println(timeClient.getEpochTime());
  if(lastTimestamp+(8*60*60)<timeClient.getEpochTime()){
    sendReadings();
  }
  bool read;
  bool start;  
  Firebase.RTDB.getBool(&fbdo, "/sensorData/readingStatus",&read);  
  Firebase.RTDB.getBool(&fbdo, "/sensorData/pumpStatus",&start);
  Serial.println(read);
  Serial.println(start);
  if(read==1){
    sendReadings();
  } 

  if(start==1){
    digitalWrite(RELAY_PIN, LOW);
    delay(50000); // 50 seconds
    Firebase.RTDB.setBool(&fbdo, "/sensorData/pumpStatus", 0);
    digitalWrite(RELAY_PIN, HIGH);
  } 
}
