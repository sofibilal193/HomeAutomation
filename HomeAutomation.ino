#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Espalexa.h>
// define the GPIO connected with Relays
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 0  //D3
#define RelayPin4 16  //D0

#define wifiPin 2 //D4

#define btn1 14  //D5
#define btn2 12  //D6
#define btn3 13  //D7
#define btn4 15  //D8

//Let's say you have your push button on pin 4

int PreviousButtonState_1 = HIGH;
int PreviousButtonState_2 = HIGH;
int PreviousButtonState_3 = HIGH;
int PreviousButtonState_4 = HIGH;

int relayState_1 = LOW;
int relayState_2 = LOW;
int relayState_3 = LOW;
int relayState_4 = LOW;

// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fifthLightChanged(uint8_t brightness);

// WiFi Credentials
const char* ssid = "ICEN506_Hotspot";
const char* password = "Bas@3313";

// device names
String Device_1 = "Light 1";
String Device_2 = "Light 2";
String Device_3 = "Light 3";
String Device_4 = "Light 4";

boolean wifiConnected = false;

Espalexa espalexa;

void setup() {
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(btn1, INPUT_PULLUP);  // push button
  pinMode(btn2, INPUT_PULLUP);  // push button
  pinMode(btn3, INPUT_PULLUP);  // push button
  pinMode(btn4, INPUT_PULLUP);  // push button


  digitalWrite(RelayPin1, relayState_1);  // relayPin1 as LOW

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    // Define your devices here.
    espalexa.addDevice(Device_1, firstLightChanged);  //simplest definition, default state off
    espalexa.addDevice(Device_2, secondLightChanged);
    espalexa.addDevice(Device_3, thirdLightChanged);
    espalexa.addDevice(Device_4, fourthLightChanged);

    espalexa.begin();
  } else {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
  }
}



void loop() {
  espalexa.loop();
  delay(1);

  checkButton1();
  delay(1);
}

//our callback functions
void firstLightChanged(uint8_t brightness) {
  //Control the device
  Serial.println(" ");
  if (brightness == 255) {
    digitalWrite(RelayPin1, LOW);
    Serial.println("Device1 ON");
  //  relayState_1 = LOW;
  } else {
    digitalWrite(RelayPin1, HIGH);
    Serial.println("Device1 OFF");
 //   relayState_1 = HIGH;
  }

   relayState_1 = !relayState_1;

  Serial.print("relayState-1: ");
  Serial.println(relayState_1);
   
}

void checkButton1() {
  // Read the current button state
  int buttonState = digitalRead(btn1);

  // Check if the button state has changed
  if (buttonState != PreviousButtonState_1) {
    // Button state has changed, debounce by waiting a short time
    delay(50);

    // Read the button state again
    buttonState = digitalRead(btn1);

    // If the button is pressed (LOW), toggle the relay state
    if (buttonState == LOW) {
     // relayState_1 = !relayState_1;
      //digitalWrite(RelayPin1, relayState_1);

      firstLightChanged(!relayState_1?255:0);
    }
  }

  // Store the current button state for the next loop iteration
  PreviousButtonState_1 = buttonState;
}

void secondLightChanged(uint8_t brightness) {
  //Control the device
  if (brightness == 255) {
    digitalWrite(RelayPin2, LOW);
    Serial.println("Device2 ON");
  } else {
    digitalWrite(RelayPin2, HIGH);
    Serial.println("Device2 OFF");
  }
}

void thirdLightChanged(uint8_t brightness) {
  //Control the device
  if (brightness == 255) {
    digitalWrite(RelayPin3, LOW);
    Serial.println("Device3 ON");
  } else {
    digitalWrite(RelayPin3, HIGH);
    Serial.println("Device3 OFF");
  }
}

void fourthLightChanged(uint8_t brightness) {
  //Control the device
  if (brightness == 255) {
    digitalWrite(RelayPin4, LOW);
    Serial.println("Device4 ON");
  } else {
    digitalWrite(RelayPin4, HIGH);
    Serial.println("Device4 OFF");
  }
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}
