#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "RYLANSLAPTOP";          // Set Wifi Name
const char* password = "Onyourleft100";     // Set Password

WiFiClient client;

int    HTTP_PORT   = 80;                    // Set to Port 80
String HTTP_METHOD = "GET"; // or "POST"    // Using GET for this. Either would work
char   HOST_NAME[] = "192.168.137.195";     // hostname/ip of web server

unsigned char echoPin = D1;                 // Set Echo Pin
unsigned char trigPin = D2;                 // Set Trig Pin

long duration;                              // sensor duration measurement
int distance1 = 0;                          // Current distance measurement
int distance2 = 0;                          // Previous distance measurement 2
int distance3 = 0;                          // Previous distance measurement 3
int distance4 = 0;                          // Previous distance measurement 4
int distance5 = 0;                          // Previous distance measurement 5
int avgDistance = 0;                        // Sensor duration measurement
bool greenOn = false;                       // Green status
bool yellowOn = false;                      // Yellow status
bool redOn = false;                         // Red status
bool blinkOn = false;                       // Blink status

// Open Green page to change green LED
void cycleGreen() {
  // If a successful connection, open page
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to arduino server");
    // send HTTP request header of /Green
    client.println(HTTP_METHOD + " " + "/Green" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    Serial.println("Navigated to Green Page");
  } else {
    Serial.println("connection for Green failed");
  }
}

// Open Yellow page to change yellow LED
void cycleYellow() {
  // If a successful connection, open page
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to arduino server");
    // send HTTP request header of /Yellow
    client.println(HTTP_METHOD + " " + "/Yellow" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    Serial.println("Navigated to Yellow Page");
  } else {
    Serial.println("connection for Yellow failed");
  }
}

// Open Red page to change red LED
void cycleRed() {
  // If a successful connection, open page
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to arduino server");
    // send HTTP request header of /Red
    client.println(HTTP_METHOD + " " + "/Red" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    Serial.println("Navigated to Red Page");
  } else {
    Serial.println("connection for Red failed");
  }
}

// Open Blink page to start red LED blink cycle
void startBlink() {
  // If a successful connection, open page
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to arduino server");
    // send HTTP request header of /Blink
    client.println(HTTP_METHOD + " " + "/Blink" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    Serial.println("Navigated to Blink Page");
  } else {
    Serial.println("connection for Blink failed");
  }
}

// Open Off page to turn off all LEDs
void lightsOff() {
  // If a successful connection, open page
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to arduino server");
    // send HTTP request header of /Off
    client.println(HTTP_METHOD + " " + "/Off" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    Serial.println("Navigated to Off Page");
  } else {
    Serial.println("connection for Off failed");
  }
}

void setup() {
  // Setup Serial Monitor with 9600
  Serial.begin(9600);

  // Create Wifi connection and delay until connected
  WiFi.mode(WIFI_STA);
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
  
  pinMode(trigPin, OUTPUT);                 // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);                  // Sets the echoPin as an Input

  lightsOff();                              // Makes sure all the lights are off to start
}

void loop() {
  // Sensor distance checking
  digitalWrite(trigPin, LOW);               // Clears the trigPin
  delayMicroseconds(2);                     // Small delay
  digitalWrite(trigPin, HIGH);              // Sets the trigPin on HIGH state
  delayMicroseconds(10);                    // For 10 microseconds
  digitalWrite(trigPin, LOW);               // trigPin back to LOW state
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the measured distance in inches. Remove "* 0.393701" to get cm.
  distance1 = duration * 0.034 / 2 * 0.393701;

  // If a bad distance read, just print a ` and don't add the new distance
  if (distance1 >= 120) {
    Serial.print("`");
  }
  else {
    // add delay to be measuring roughly 9-10 times a second
    delay(100);

    // Average the previous 5 distances together to get a rolling average
    avgDistance = (distance1 + distance2 + distance3 + distance4 + distance5) / 5;

    // Shift all of the measured distances once
    distance5 = distance4;
    distance4 = distance3;
    distance3 = distance2;
    distance2 = distance1;

    // Prints the avgDistance on the Serial Monitor
    Serial.print("Average Distance (inch): ");
    Serial.println(avgDistance);
  }

  // Now to check the distance and set the appropriate LEDs with Get requests

  // Distance between 120 and 20 inches, set LED to green if not already green
  if ((avgDistance <= 120) && (avgDistance > 20) && greenOn == false) {
    // Lights Off then Green on
    lightsOff();
    cycleGreen();

    // All status false but green
    greenOn = true;
    yellowOn = false;
    redOn = false;
    blinkOn = false;
  }
  // Distance between 20 and 12 inches, set LED to yellow if not already yellow
  else if ((avgDistance <= 20) && (avgDistance > 12) && yellowOn == false) {
    // Lights Off then Yellow on
    lightsOff();
    cycleYellow();

    // All status false but yellow
    greenOn = false;
    yellowOn = true;
    redOn = false;
    blinkOn = false;
  }
  // Distance between 12 and 5 inches, set LED to red if not already red
  else if ((avgDistance <= 12) && (avgDistance > 5) && redOn == false) {
    // Lights Off then Red on
    lightsOff();
    cycleRed();

    // All status false but red
    greenOn = false;
    yellowOn = false;
    redOn = true;
    blinkOn = false;
  }
  // Distance between 5 and 0 inches, set LED to blinking red if not already blinking
  else if ((avgDistance <= 5) && (avgDistance > 0) && blinkOn == false) {
    // Lights Off then Blink Red on
    lightsOff();
    startBlink();

    // All status false but blink
    greenOn = false;
    yellowOn = false;
    redOn = false;
    blinkOn = true;
  }
}
