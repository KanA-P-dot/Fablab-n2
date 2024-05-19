#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

const char *ssid = "Hv4djF6c638"; // name of wifi
const char *password = "12345678"; // password of wifi
const String serverName = "http://192.168.145.207:5000/post-json"; // endpoint for send data

const int buttonBad = D6; 
const int buttonNotBad = D2; 
const int buttonGood = D3; 

const int ledBad = D7;
const int ledNotBad = D8;
const int ledGood = D9;

const unsigned long antiAfkTimeout = 5000; // 5 seconds
const unsigned long shortDelay = 200; // 200 milliseconds

LiquidCrystal_I2C lcd(0x27, 16, 2);

int lastButtonStateBad = 0;
int lastButtonStateNotBad = 0;
int lastButtonStateGood = 0;

int stateQuestion = 0;
unsigned long antiAfkPreviousTime = 0;
int antiAfk = 0;
String responseQuestion1 = ""; // first question on the screen
String responseQuestion2 = ""; // second question on the screen

void saveChoice() {
  if (WiFi.status() == WL_CONNECTED) { // check wifi is online
    HTTPClient http; // init httpClient
    String serverPath = serverName + "?response1=" + responseQuestion1 + "&response2=" + responseQuestion2; // create endpoint with params
    http.begin(serverPath);
    
    int httpResponseCode = http.GET(); // send get request

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload); // print payload response
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void eval(const char* choice) { // event on button click
  if (strcmp(choice, "good") == 0) { // if is good button
    if (stateQuestion == 0) { // first question
      responseQuestion1 = "Bien";
    } else { // second question
      responseQuestion2 = "Bien";
    }
    digitalWrite(ledGood, HIGH); // on led good
    delay(shortDelay);
    digitalWrite(ledGood, LOW); // off led good
  } else if (strcmp(choice, "notBad") == 0) {
    if (stateQuestion == 0) {
      responseQuestion1 =  "Moyen";
    } else {
      responseQuestion2 = "Moyen";
    }
    digitalWrite(ledNotBad, HIGH);
    delay(shortDelay);
    digitalWrite(ledNotBad, LOW);
  } else if (strcmp(choice, "bad") == 0) {
    if (stateQuestion == 0) {
      responseQuestion1 =  "Mauvais";
    } else {
      responseQuestion2 = "Mauvais";
    }
    digitalWrite(ledBad, HIGH);
    delay(shortDelay);
    digitalWrite(ledBad, LOW);
  }
  Serial.println(choice);
  stateQuestion++; // switch to next question
  showQuestion();
}

void showQuestion() { // show default menu
  lcd.clear();
  lcd.cursor_on();
  lcd.blink_on();
  lcd.setCursor(0,0);
  if (stateQuestion == 0) {
    lcd.print("> Avis prof ?");
  } else if (!responseQuestion1.isEmpty()) {
    lcd.print("Avis prof: ");
    lcd.print(responseQuestion1);
  } else {
    lcd.print("Avis prof ?");
  }
  lcd.setCursor(0,1);
  if (stateQuestion == 1) {
    lcd.print("> Avis Cours ?");
  } else if (!responseQuestion2.isEmpty()) {
    lcd.print("Avis Cours: ");
    lcd.print(responseQuestion2);
  } else {
    lcd.print("Avis Cours ?");
  }
  delay(shortDelay);
  lcd.cursor_off();
  lcd.blink_off();
}

void setup() {
  if (!i2CAddrTest(0x27)) { // if i2c not found change address
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();
  pinMode(ledBad, OUTPUT);
  pinMode(ledNotBad, OUTPUT);
  pinMode(ledGood, OUTPUT);
  pinMode(buttonBad, INPUT_PULLUP);
  pinMode(buttonNotBad, INPUT_PULLUP);
  pinMode(buttonGood, INPUT_PULLUP);

  lastButtonStateBad = digitalRead(buttonBad);
  lastButtonStateNotBad = digitalRead(buttonNotBad);
  lastButtonStateGood = digitalRead(buttonGood);

  showQuestion();
}

void loop() {
  unsigned long currentTime = millis();
  int buttonStateBad = digitalRead(buttonBad);
  int buttonStateNotBad = digitalRead(buttonNotBad);
  int buttonStateGood = digitalRead(buttonGood);

  if (buttonStateBad == HIGH && lastButtonStateBad == LOW) { eval("bad"); } // detect button bad press
  if (buttonStateNotBad == HIGH && lastButtonStateNotBad == LOW) { eval("notBad"); }
  if (buttonStateGood == HIGH && lastButtonStateGood == LOW) { eval("good"); }

  if (stateQuestion == 2) { // if is the last question, reset and display thx and send to server
    displayThankYou();
    saveChoice();
    delay(1000);
    resetQuestionnaire();
  }

  if (stateQuestion == 1 && currentTime - antiAfkPreviousTime >= 1000) { // for anti afk, if not response to question 2 in five seconds, reset
    antiAfk++;
    antiAfkPreviousTime = currentTime;
    if (antiAfk * 1000 > antiAfkTimeout) {
      resetQuestionnaire();
    }
  }

  lastButtonStateBad = buttonStateBad;
  lastButtonStateNotBad = buttonStateNotBad;
  lastButtonStateGood = buttonStateGood;
}

void displayThankYou() {
  lcd.clear();
  lcd.cursor_on();
  lcd.blink_on();
  lcd.setCursor(5,0);
  lcd.print("Merci !");
  lcd.setCursor(1,1);
  lcd.print("Bonne journee!");
  delay(shortDelay);
  lcd.cursor_off();
  lcd.blink_off();
}

void resetQuestionnaire() {
  stateQuestion = 0;
  responseQuestion1 = "";
  responseQuestion2 = "";
  antiAfk = 0;
  showQuestion();
}

bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}
