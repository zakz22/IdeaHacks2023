#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Firebase_ESP_Client.h>
#include "Website.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "steps.h"

// enter your network credentials
#define WIFI_SSID "IEEE"
#define WIFI_PASSWORD "Ilovesolder"

// Firebase project API Key and URL here
#define API_KEY "AIzaSyB-KixGYjdAPbbJMKj58F0BEUF_eum8HC0"
#define DATABASE_URL "https://ideahacks2023-default-rtdb.firebaseio.com/" 

void handleNotFound(AsyncWebServerRequest *request);
void handleWrite();
void handleRead();

//Steps
steps s;

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// declare web server on port 80
AsyncWebServer server(80);

// parameter names from website form
const char* PARAM_INPUT_1 = "key";
const char* PARAM_INPUT_2 = "val";
const char* PARAM_INPUT_3 = "btn";
String key = ""; // stores database entry key
int val = 0;     // stores value on form or read from database

// called if an invalid URL is requested
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  // start Serial connection
  Serial.begin(115200);

  // connect to desired WiFi network in station mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi connection failed");
    return;
  }
  Serial.println();

  // print IP address to connect to
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", PAGE_MAIN);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // process key parameter
    if (request->hasParam(PARAM_INPUT_1)) {
      key = request->getParam(PARAM_INPUT_1)->value();
    }
    
    // process value parameter
    if (request->hasParam(PARAM_INPUT_2)) {
      val = request->getParam(PARAM_INPUT_2)->value().toInt();
    }
    
    // determine which button was pressed
    if (request->hasParam(PARAM_INPUT_3)) {
      String inputMessage = request->getParam(PARAM_INPUT_3)->value();

      if(inputMessage == "READ") handleRead();
      else if(inputMessage == "WRITE") handleWrite();
    }

    // refresh page after pressing button
    request->send_P(200, "text/html", PAGE_MAIN);
  });

  server.onNotFound(handleNotFound);

  // start web server
  server.begin();

  // assign API key
  config.api_key = API_KEY;

  // assign RTDB URL
  config.database_url = DATABASE_URL;

  // sign up
  if (!Firebase.signUp(&config, &auth, "", "")){
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  // start Firebase connection
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Firebase.RTDB.setInt(&fbdo, "Steps", 0);
}

void loop() {
  Firebase.RTDB.setInt(&fbdo, "Steps", s.getSteps());
  s.run();
  delay(100);
}

// reads value of entry with a given key
void handleRead() {
  Serial.print("Reading key ");
  Serial.print(key);
  Serial.print(" from database. Value: ");
  // read an int value from the database path given by key
  if (Firebase.RTDB.getInt(&fbdo, key)) {
    if (fbdo.dataType() == "int") {
      val = fbdo.intData();
      Serial.println(val);
    }
  }
  else {
    Serial.println(fbdo.errorReason());
  }
}

// writes value to entry given by key
void handleWrite() {
  Serial.println("Writing ");
  Serial.print(val);
  Serial.print(" to key ");
  Serial.print(key);
  Serial.print(" in database");
  // Write an Int number on the database path given by key
  if (!Firebase.RTDB.setInt(&fbdo, key, val)){
    Serial.println(fbdo.errorReason());
  }
}

// sends 404 page when invalid URL requested
void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}