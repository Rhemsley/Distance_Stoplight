#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Needed variables for LED Pin association
unsigned char greenLed = D3;        // Create greenLed Label for pin 3
unsigned char yellowLed = D2;       // Create yellowLed Label for pin 2
unsigned char redLed = D1;          // Create redLed Label for pin 1

// Needed variables for looping
unsigned long previousTime = 0;     // Create previousTime int for looping
unsigned long startTime = 0;        // Create startTime int for looping
bool runStoplight = false;          // Create runStoplight bool for looping
bool runBlink = false;
bool redStatus = false;             // Create Red bool for cycling red
bool yellowStatus = false;          // Create Yellow bool for cycling yellow
bool greenStatus = false;           // Create Green bool for cycling green

// wifi and server config
const char* ssid = "RYLANSLAPTOP";          // Set Wifi Name
const char* password = "Onyourleft100";     // Set Password
ESP8266WebServer server(80);                // Set server to port 80

// Simple HTML saved in a string with the different page references in buttons
const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <title>Home Webpage</title>
    <h1>Home Webpage</h1>
    <h2>Stoplight</h2>
  </head>
  <body>
    <a href="/Off"><button style="background-color:grey">Off</button></a>
    <a href="/Red"><button style="background-color:red">Red</button></a>
    <a href="/Yellow"><button style="background-color:yellow">Yellow</button></a>
    <a href="/Green"><button style="background-color:lightGreen">Green</button></a>
    <a href="/Loop"><button style="background-color:lightGrey">Loop</button></a>
  </body>
</html>
)=====";

// Handle default page call
void handleRoot() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user
}

// Handle Off call and reset needed variables and LEDs
void handleOff() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user
  digitalWrite(greenLed, LOW);              // Turn Green LED off
  digitalWrite(yellowLed, LOW);             // Turn Yellow LED off
  digitalWrite(redLed, LOW);                // Turn Red LED off
  greenStatus = false;                      // Indicate green is off
  yellowStatus = false;                     // Indicate yellow is off
  redStatus = false;                        // Indicate red is off
  runStoplight = false;                     // Stop stoplight looping
  runBlink = false;                         // Stop blink looping
  previousTime = 0;                         // Reset previousTime
  startTime = 0;                            // Reset startTime
  Serial.println("Turning Off LEDs");
}

// Handle Red call and cycle red LED depending on status
void handleRed() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user
  
  // If off turn on and if on turn off
  if (redStatus == false) {
    digitalWrite(redLed, HIGH);             // Turn Red LED on
    redStatus = true;                       // Flip status
  }
  else {
    digitalWrite(redLed, LOW);              // Turn Red LED off
    redStatus = false;                      // Flip status
  }
  Serial.println("Cycling Red");
}

// Handle Yellow call and cycle yellow LED depending on status
void handleYellow() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user
  
  // If off turn on and if on turn off
  if (yellowStatus == false) {
    digitalWrite(yellowLed, HIGH);          // Turn Yellow LED on
    yellowStatus = true;                    // Flip status
  }
  else {
    digitalWrite(yellowLed, LOW);           // Turn Yellow LED off
    yellowStatus = false;                   // Flip status
  }
  Serial.println("Cycling Yellow");
}

// Handle Green call and cycle green LED depending on status
void handleGreen() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user
  
  // If off turn on and if on turn off
  if (greenStatus == false) {
    digitalWrite(greenLed, HIGH);           // Turn Green LED on
    greenStatus = true;                     // Flip status
  }
  else {
    digitalWrite(greenLed, LOW);            // Turn Green LED off
    greenStatus = false;                    // Flip status
  }
  Serial.println("Cycling Green");
}

// Handle Loop call and setup needed variables
void handleLoop() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user
  
  digitalWrite(greenLed, LOW);              // Turn Green LED off
  digitalWrite(yellowLed, LOW);             // Turn Yellow LED off
  digitalWrite(redLed, LOW);                // Turn Red LED off

  greenStatus = false;                      // Indicate green is off
  yellowStatus = false;                     // Indicate yellow is off
  redStatus = false;                        // Indicate red is off
  
  runStoplight = true;                      // Start stoplight looping
  previousTime = millis();                  // Set previousTime to current runtime
  startTime = millis();                     // Then set startTime to current runtime
  Serial.println("Starting Stoplight Loop");
}

void handleBlink() {
  String page = MAIN_page;                  // Store HTML in a string
  server.send(200, "text/html", page);      // Send HTML to user

  digitalWrite(greenLed, LOW);              // Turn Green LED off
  digitalWrite(yellowLed, LOW);             // Turn Yellow LED off
  digitalWrite(redLed, LOW);                // Turn Red LED off

  greenStatus = false;                      // Indicate green is off
  yellowStatus = false;                     // Indicate yellow is off
  redStatus = false;                        // Indicate red is off
  runBlink = true;                          // Start blink looping
}

void setup() {
  // Setup Serial Monitor with 9600
  Serial.begin(9600);

  // Create Wifi connection and delay until connected
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {    // Loop until connected to WiFi
    delay(500);
    Serial.print(".");
  }

  // If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());           // Show your assigned IP address

  // Setup all 3 LED pins and make sure they are off
  pinMode(greenLed, OUTPUT);                // Set Green LED to Output pin
  pinMode(yellowLed, OUTPUT);               // Set Yellow LED to Output pin
  pinMode(redLed, OUTPUT);                  // Set Red LED to Output pin
  digitalWrite(greenLed, LOW);              // Turn Green LED off
  digitalWrite(yellowLed, LOW);             // Turn Yellow LED off
  digitalWrite(redLed, LOW);                // Turn Red LED off

  // Setup the needed Handles for the client operations
  server.on("/", handleRoot);
  server.on("/Off", handleOff);
  server.on("/Red", handleRed);
  server.on("/Yellow", handleYellow);
  server.on("/Green", handleGreen);
  server.on("/Loop", handleLoop);
  server.on("/Blink", handleBlink);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Run server client requests
  server.handleClient();
  
  // Stoplight Controlling only runs when set to true
  if (runStoplight == true) {
    // compare last run to starting time, turn to seconds, then 
    // check if within first second of three second loop
    if ((((previousTime - startTime) / 1000)%3) == 0) {
        digitalWrite(greenLed, HIGH);         // Turn Green LED on
        digitalWrite(yellowLed, LOW);         // Turn Yellow LED off
        digitalWrite(redLed, LOW);            // Turn Red LED off
        Serial.println("Loop Green ON");
        previousTime = millis();              // Set previous time to current time
    }
    // compare last run to starting time, turn to seconds, then 
    // check if within second second of three second loop
    else if ((((previousTime - startTime) / 1000)%3) == 1) {
        digitalWrite(greenLed, LOW);          // Turn Green LED off
        digitalWrite(yellowLed, HIGH);        // Turn Yellow LED on
        digitalWrite(redLed, LOW);            // Turn Red LED off
        Serial.println("Loop Yellow ON");
        previousTime = millis();              // Set previous time to current time
    }
    // compare last run to starting time, turn to seconds, then 
    // check if within third second of three second loop
    else if ((((previousTime - startTime) / 1000)%3) == 2) {
        digitalWrite(greenLed, LOW);          // Turn Green LED off
        digitalWrite(yellowLed, LOW);         // Turn Yellow LED off
        digitalWrite(redLed, HIGH);           // Turn Red LED on
        Serial.println("Loop Red ON");
        previousTime = millis();              // Set previous time to current time
    } 
  }

  // Blink operation only runs when set to true
  if (runBlink == true) {
    // If off turn on and if on turn off
    if (redStatus == false) {
      digitalWrite(redLed, HIGH);             // Turn Red LED on
      redStatus = true;                       // Flip status
    }
    else {
      digitalWrite(redLed, LOW);              // Turn Red LED off
      redStatus = false;                      // Flip status
    }
    Serial.println("Cycling Red");
    delay(100);                               // Delay for 100ms
  }
}
