#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FastLED.h>

// WIFI Set up for getting time
// Replace with your network credentials
const char* ssid     = "***";
const char* password = "***";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
int hoursOnly;
int minsOnly;
int currentCount;
int secondHand;

// 2.4g set up for master coms
RF24 radio(4, 2); // CE, CSN on ESP32
//RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  //Serial.begin(9600); // rate for Nano
  Serial.begin(115200); // rate for ESP32

// WIFI Set up for getting time
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
    {
    delay(500);
    Serial.print(".");
    }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(-18000);
  
  //2.4g set up
  radio.begin();
  radio.openWritingPipe(address);
  radio.setChannel(120);
  radio.setPALevel(RF24_PA_MAX); //changed to 
  radio.stopListening();
  radio.setDataRate(RF24_250KBPS);           // Lower the data rate to increase transmission distance
}

void loop() {
  EVERY_N_MILLISECONDS(300) {
    timeClient.update();
    hoursOnly = timeClient.getHours();
    minsOnly = timeClient.getMinutes();
    if( minsOnly != minsOnly ) {
    Serial.print("HOURS:");
    Serial.println(hoursOnly);
    Serial.print("MINUTES:");
    Serial.println(minsOnly);
    }
    } //this pulls time and inputs into variables
  if ( (minsOnly > 0 && minsOnly < 5) || (millis() < 115000) ) 
        // Top of the hour for approx 2 cycles and for one min when it turns on 
  //if ( minsOnly > 0) // for testing
    {sendCounter();}
    else
    {sendStopper();}
  }

void sendCounter() {
  EVERY_N_MILLISECONDS(1000) {secondHand++;}
  static uint8_t lastSecond = 110;
  if (lastSecond == secondHand) {secondHand = 0;}
  radio.write(&secondHand, sizeof(secondHand));
  Serial.println(secondHand);
}

void sendStopper() {
  if (secondHand != 0) {
  for (int i = 0; i < 100; i++) {
    secondHand = 0;
    radio.write(&secondHand, sizeof(secondHand));
    Serial.println(secondHand);
    delay(10);
    } //end for
  } //end if
}
